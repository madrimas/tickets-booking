#include "Tadmin.h"
#include "Tklient.h"
#include "Tsala.h"
#include <fstream>
#include <Windows.h>
#include <algorithm>
#include <iomanip>

template<class typ>
ostream & operator<<(ostream & screen, Tsala<typ>& object);

int ile_filmow(fstream&); // sprawdza ile filmow w repertuarze
int ile_godzin(fstream&); //sprawdza ile godzin wyswietlania danego filmu + miejsce na nazwe filmu
void uzu_rep(fstream&, string**, int*, int); //uzupelnia tablice repertuaru
void wys_rep(fstream&, string**, int*, int); //wyswietla repertuar
void data(); //wyœwietla aktualn¹ datê + 1 dzieñ
void (Tadmin::*wsk_pobierz) (void) = &Tadmin::pobierz_dane;

void wybor_seansu(int*, fstream&, string**, int*, int);

int main()
{
	setlocale(LC_ALL, "");
	cout << "Wybierz sposób logowania" << endl
		<< "WprowadŸ '1' je¿eli jesteœ administratorem" << endl
		<< "WprowadŸ '2' je¿eli chcesz zarezerwowaæ bilet" << endl;
	int wybor;
	try
	{
		cin >> wybor;
		if (!cin)
		{
			throw (float)1.0;
		}
	}
	catch (float)
	{
		wybor = 69;
	}
	bool flaga_wyboru = true;
	system("cls");
	fstream repertuar;
	do
	{
		switch (wybor)
		{
		case 1:
		{
			flaga_wyboru = true;
			cout << "Jesteœ w panelu administratora" << endl;
			cout << "Podaj login i has³o, ka¿dorazowo zatwierdzaj¹c wybór enterem" << endl;
			Tadmin admin;
			string login, haslo;

			(admin.*wsk_pobierz)(); // wskaŸnik do funkcji sk³adowej

			fstream logowanie;

			logowanie.open("logowanie.txt", ios::in);

			if (logowanie.is_open())
			{
				getline(logowanie, login);
				getline(logowanie, haslo);

				if (admin.czy_poprawne(login, haslo))
				{
					cout << "Dostêp przyznany" << endl;
					logowanie.close();

					repertuar.open("repertuar.txt", ios::app);

					if (repertuar.is_open())
					{
						system("cls");
						cout << "Dodaj film do repertuaru" << endl
							<< "Podaj nazwê filmu, zatwierdŸ" << endl
							<< "WprowadŸ '*' i zatwierdŸ" << endl
							<< "Nastêpnie godziny seansu w formacie hh:mm, ka¿dorazowo zatwierdzaj¹c" << endl
							<< "Na koniec wprowadŸ '-' i zatwierdŸ" << endl;

						string temp; //zmienna do ktorej wprowadzamy dane z klawiatury
						do
						{
							getline(cin, temp);
							repertuar.write(&temp[0], temp.length());
							if (temp != "-")
							{
								repertuar << "\n";
							}
						} while (temp != "-");

						system("cls");
						cout << "Nie zapomnij dodaæ sal do seansów!!!" << endl;
						cout << "FORMAT: nazwa_filmuHH.MM.txt" << endl;
						cout << "Dane wprowadzone poprawnie, nastapi³o wylogowanie." << endl;

						repertuar.close();
					}
					else
					{
						cout << "Plik repertuar.txt nie zosta³ otwarty!" << endl;
					}
				}
				else
				{
					system("cls");
					cout << "B³êdny login lub has³o. Spróbuj ponownie." << endl;
					flaga_wyboru = false;
				}

			}
			else
			{
				cout << "Brak dostêpu do pliku logowanie.txt" << endl;
			}

			break;
		}
		case 2:
		{
			system("cls");
			flaga_wyboru = true;
			cout << "System Rezerwacji Biletów!" << endl << "Witaj!" << endl;
			Tklient klient;

			fstream klienci;
			klienci.open("klienci.txt", ios::in);

			if (klienci.is_open())
			{
				string login_klienta;
				bool login_flaga = true;
				while (login_flaga)
				{
					try
					{
						klient.pobierz_dane();
						string login_temp;
						login_klienta = klient.wyrzuc_login();
						klienci.seekg(0);
						while (!klienci.eof())
						{
							getline(klienci, login_temp);
							if (login_klienta == login_temp)
							{
								cout << "Taki login ju¿ istnieje, wybierz inny!" << endl;
								throw (float)1.0;
							}
							else
							{
								login_flaga = false;
							}
						}
					}
					catch (float)
					{
						login_flaga = true;
					}
				}
				klienci.close();
				klienci.open("klienci.txt", ios::app);

				if (klienci.is_open())
				{
					klienci << endl;
					klienci << login_klienta;
					klienci.close();
				}
				else
				{
					cout << "Coœ posz³o nie tak!" << endl;
					system("pause");
					system("exit");
				}

			}
			else
			{
				cout << "Coœ posz³o nie tak!" << endl;
				system("pause");
				system("exit");
			}


			repertuar.open("repertuar.txt", ios::in);



			if (repertuar.is_open())
			{
				int ile_film = ile_filmow(repertuar);

				string ** tab_rep = new string*[ile_film];
				int * temp_tab = new int[ile_film];

				repertuar.seekg(0); //ustawiamy znacznik na poczatek pliku, by umozliwic zczytanie danych

				int ile_godz = 0;

				for (int i = 0; i < ile_film; i++)
				{
					ile_godz = ile_godzin(repertuar);
					temp_tab[i] = ile_godz;
					tab_rep[i] = new string[ile_godz];
				}
				repertuar.seekg(0);

				uzu_rep(repertuar, tab_rep, temp_tab, ile_film);
				wys_rep(repertuar, tab_rep, temp_tab, ile_film);

				int wybor[2];

				wybor_seansu(wybor, repertuar, tab_rep, temp_tab, ile_film);

				string sala;
				sala = ((tab_rep[wybor[0]][0]) + (tab_rep[wybor[0]][wybor[1]]) + (".txt"));

				replace(sala.begin(), sala.end(), ' ', '_');
				replace(sala.begin(), sala.end(), ':', '.');

				fstream strumien;
				strumien.open(sala.c_str(), ios::in);
				if (strumien.is_open())
				{
					string bufor, nazwa;
					getline(strumien, bufor);
					nazwa = bufor;
					getline(strumien, bufor);
					int x = stoi(bufor);
					getline(strumien, bufor);
					int y = stoi(bufor);

					Tsala<string> sala_seansu(x, y, nazwa);

					sala_seansu.pusta();

					while (!strumien.eof())
					{
						strumien >> x;
						strumien >> y;
						sala_seansu.zajete(x - 1, y - 1);
					}

					cout << sala_seansu << endl;

					strumien.close();

					strumien.open(sala.c_str(), ios::app);

					if (strumien.is_open())
					{
						int ile_zajac;
						cout << "Ile miejsc chcesz zarezerwowaæ?" << endl;
						cin >> ile_zajac;
						while (!cin)
						{
							cout << "Wpisany znak nie by³ liczb¹, powtórz wpisywanie" << endl;
							cin.clear();
							cin.ignore(256, '\n');
							cin >> ile_zajac;
						}
						cin.clear();
						cin.ignore(256, '\n');

						int ** tab = new int*[ile_zajac];
						for (int i = 0; i < ile_zajac; i++)
						{
							tab[i] = new int[2];
						}
						for (int i = 0; i < ile_zajac; i++)
						{
							cout << "Wybierz miejsce metod¹: nr_rzedu enter nr_miejsca enter" << endl;
							cin >> x;
							cin >> y;
							if (sala_seansu.czy_wolne(x - 1, y - 1))
							{
								sala_seansu.zajete(x - 1, y - 1);
								tab[i][0] = x - 1;
								tab[i][1] = y - 1;
								cout << "Miejsce " << x << "," << y << " zarezerwowano dla Ciebie" << endl;
							}
							else
							{
								cout << "To miejsce jest ju¿ zajête lub nieprawid³owe, wybierz inne" << endl;
								i--;
							}
						}

						cout << "Wybra³eœ nastepuj¹ce miejsca:" << endl;
						for (int i = 0; i < ile_zajac; i++)
						{
							cout << tab[i][0] + 1 << " " << tab[i][1] + 1 << endl;
						}
						cout << "Czy potwierdzasz rezerwacjê? 1=TAK, 0=NIE" << endl;
						bool potwierdzenie;
						cin >> potwierdzenie;
						if (potwierdzenie == true)
						{
							system("cls");
							for (int i = 0; i < ile_zajac; i++)
							{
								x = tab[i][0] + 1;
								y = tab[i][1] + 1;
								strumien << " " << x;
								strumien.flush();
								strumien << " " << y;
								strumien.flush();
							}
							cout << "Gratulujê, zarezerwowa³eœ miejsca na nazwê u¿ytkownika: ";
							klient.wyswietl_dane();
							cout << "Na film: " << (tab_rep[wybor[0]][0]) << " o godz. " << (tab_rep[wybor[0]][wybor[1]]) << endl;
							cout << "Na dzieñ: ";
							data();
							cout << "Pamietaj by odebraæ bilety co najmniej 30min przed rozpoczêciem seansu!" << endl;
							cout << "Przy odbiorze podaj swoj¹ nazwê u¿ytkownika, tytu³ filmu i godzinê rozpoczêcia seansu" << endl;
							cout << "¯yczymy mi³ego seansu!" << endl;

							fstream klient_bilet;
							klient_bilet.open("klient_bilet.txt", ios::app);

							if (klient_bilet.is_open())
							{
								klient_bilet << endl;
								klient_bilet << klient.wyrzuc_login() << " ";
								klient_bilet << (tab_rep[wybor[0]][0]) << " " << (tab_rep[wybor[0]][wybor[1]]) << " ";
								for (int i = 0; i < ile_zajac; i++)
								{
									x = tab[i][0] + 1;
									y = tab[i][1] + 1;
									klient_bilet << " " << x;
									klient_bilet << " " << y;
								}
								klient_bilet.close();
							}
						}
						else
						{
							system("cls");
							cout << "Twoja rezerwacja zosta³a cofniêta" << endl;
							for (int i = 0; i < ile_zajac; i++)
							{
								sala_seansu.zwalniam(tab[i][0], tab[i][1]);
							}
						}
						strumien.close();
					}
					else
					{
						cout << "Nie uda³o siê otworzyæ..." << endl;
					}
				}
				else
				{
					cout << "Zamkniêty..." << endl;
				}

				repertuar.close();
			}
			else
			{
				cout << "Repertuar.txt nie zosta³ prawid³owo otwarty!" << endl;
			}

			break;
		}
		default:
		{
			system("cls");
			flaga_wyboru = false;
			cin.clear();
			cin.ignore(256, '\n');
			cout << "Wybór nieprawid³owy, spróbuj jeszcze raz." << endl
				<< "WprowadŸ '1' je¿eli jesteœ administratorem" << endl
				<< "WprowadŸ '2' je¿eli chcesz zarezerwowaæ bilet" << endl;
			try
			{
				cin >> wybor;
				if (!cin)
				{
					throw (float)1.0;
				}
			}
			catch (float)
			{
				wybor = 69;
			}
			break;
		}
		}
	} while (!flaga_wyboru);


	system("pause");
	return 0;
}

//***************************************************************************************

int ile_filmow(fstream& plik)
{
	string bufor;
	int licznik = 0;
	while (!plik.eof())
	{
		getline(plik, bufor);
		if (bufor == "-")
		{
			licznik++;
		}
	}
	return licznik;
}

int ile_godzin(fstream& plik)
{
	string bufor;
	int licznik = 0;
	for (int i = 1; i == 1;)
	{
		getline(plik, bufor);
		if (bufor == "*")
		{
			while (bufor != "-")
			{
				getline(plik, bufor);
				licznik++;
			}
			i = 0;
		}
	}
	return licznik;
}

void uzu_rep(fstream & plik, string ** rep, int * h, int f)
{
	string bufor;
	int ile_h;
	for (int i = 0; i < f; i++)
	{
		ile_h = h[i];
		for (int j = 0; j < ile_h;)
		{
			getline(plik, bufor);
			if ((bufor != "*") && (bufor != "-"))
			{
				rep[i][j] = bufor;
				j++;
			}
		}
	}
}

void wys_rep(fstream & plik, string ** rep, int * h, int f)
{
	system("cls");
	cout << "Dostêpne seanse: " << endl;
	int ile_h;
	for (int i = 0; i < f; i++)
	{
		ile_h = h[i];
		for (int j = 0; j < ile_h; j++)
		{
			cout << rep[i][j] << "  ";
		}
		cout << endl;
	}
}

void data()
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	int dzien = time.wDay;
	int miesiac = time.wMonth;
	int rok = time.wYear;
	cout << dzien + 1 << ":" << miesiac << ":" << rok << endl;
}

void wybor_seansu(int* wybor, fstream & plik, string ** rep, int * h, int f)
{
	bool flag = false;
	int test;
	while (flag == false)
	{
		cout << "Dokonujesz rezerwacji na dzien: ";
		data();
		cout << "Wybierz seans" << endl;
		cout << "Aby dokonaæ wyboru, wybierz na klawiaturze nr_filmu w kolejnoœci" << endl;
		cout << "1 - pierwszy film: " << rep[0][0] << " itd." << endl;
		cout << "ZatwierdŸ wybór enterem i w analogiczny sposób wybierz godzinê seansu" << endl;
		cout << "1 - pierwsza godzina seansu: " << rep[0][1] << " itd." << endl;

		int temp;
		cin.clear();
		cin.ignore(256, '\n');
		cin >> temp;
		while (!flag)
		{
			while (!cin)
			{
				cout << "Wpisany znak nie by³ liczb¹, powtórz wpisywanie" << endl;
				cin.clear();
				cin.ignore(256, '\n');
				cin >> temp;
			}
			plik.seekg(0);
			if (cin)
			{
				if ((temp < 1) || (temp > ile_filmow(plik)))
				{
					cout << "Film o takim numerze nie istnieje w bazie danych" << endl;
					cin.clear();
					cin.ignore(256, '\n');
					cin >> temp;
				}
				else
				{
					flag = true;
				}
			}
		}
		wybor[0] = temp - 1;
		flag = false;
		cin.clear();
		cin.ignore(256, '\n');
		cin >> temp;
		while (!flag)
		{
			while (!cin)
			{
				cout << "Wpisany znak nie by³ liczb¹, powtórz wpisywanie" << endl;
				cin.clear();
				cin.ignore(256, '\n');
				cin >> temp;
			}
			plik.seekg(0);
			if (cin)
			{
				if ((temp < 1) || (temp > ile_godzin(plik) - 1))
				{
					cout << "Godzina o takim numerze nie istnieje w bazie danych" << endl;
					cin.clear();
					cin.ignore(256, '\n');
					cin >> temp;
				}
				else
				{
					flag = true;
				}
			}
		}
		wybor[1] = temp;
		flag = false;
		cout << "Wybra³eœ film: " << rep[wybor[0]][0] << endl;
		cout << "O godzinie: " << rep[wybor[0]][wybor[1]] << endl;

		cin.clear();
		cin.ignore(256, '\n');

		cout << "Je¿eli to prawid³owy wybor wciœnij 1" << endl << "Je¿eli nieprawdi³owy wciœnij 0 i wybierz ponownie" << endl;
		try
		{
			cin >> test;
			if (test != 1 && test != 0)
			{
				throw (float)1.0;
			}
		}
		catch (float)
		{
			cout << "Coœ posz³o nie tak..." << endl;
			system("pause");
			exit(0);
		}
		if (test)
		{
			flag = true;
			system("cls");
		}
		else
		{
			flag = false;
		}

	}
}

template<class typ>
ostream & operator<<(ostream & screen, Tsala<typ>& object)
{
	HANDLE hOut;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	cout << "  ";
	for (int j = 0; j < 4 * object.miejsca + 1; j++)
	{
		if (j == 0 || j == 4 * object.miejsca)
		{
			cout << " ";
		}
		else
		{
			cout << "_";
		}
	}
	cout << endl;
	cout << "  ";
	cout << setw(4 * object.miejsca) << left << "|" << right << "|" << endl;
	cout << "  ";
	for (int j = 0; j < 4 * object.miejsca + 1; j++)
	{
		if (j == 0 || j == 4 * object.miejsca - 5)
		{
			cout << "|";
		}
		if (j == (4 * object.miejsca / 2 - 3))
		{
			cout << "EKRAN";
		}
		else
		{
			cout << " ";
		}
	}
	cout << endl;
	cout << "  ";
	for (int j = 0; j < 4 * object.miejsca + 1; j++)
	{
		if (j == 0 || j == 4 * object.miejsca)
		{
			cout << "|";
		}
		else
		{
			cout << "_";
		}
	}
	cout << endl << endl;
	for (int i = 0; i < object.miejsca + 1; i++)
	{
		if (i != 0)
		{
			screen << setw(3) << i << " ";
		}
		else
		{
			screen << setw(4) << "   ";
		}
	}
	screen << endl;
	for (int i = 0; i < object.rzedy; i++)
	{
		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		screen << setw(3) << i + 1 << " ";
		for (int j = 0; j < object.miejsca; j++)
		{
			if (object.czy_wolne(i, j))
			{
				SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				screen << setw(3) << object.tablica_sali[i][j] << " ";
			}
			else
			{
				SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
				screen << setw(3) << object.tablica_sali[i][j] << " ";
			}
		}
		screen << endl;
	}
	SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	screen << "Miejsca dostêpne - 'O'" << endl;
	SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
	screen << "Miejsca zajête - 'X'" << endl;
	SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	return screen;
}