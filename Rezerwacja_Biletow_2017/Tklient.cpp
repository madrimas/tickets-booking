#include "Tklient.h"

Tklient::Tklient()
{
}


Tklient::~Tklient()
{
}

void Tklient::pobierz_dane()
{
	cout << "Podaj nazwê u¿ytkownika, bêdzie ona potrzebna do odbioru biletów!" << endl;
	cin >> login;
}

void Tklient::wyswietl_dane()
{
	cout << this->login << endl;
}

string Tklient::wyrzuc_login()
{
	string temp = login;
	return temp;
}
