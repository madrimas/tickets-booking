#pragma once
template <class typ>
class Tsala
{
private:
	int rzedy, miejsca;
	string nazwa;
	typ** tablica_sali;
public:
	Tsala(int = 1, int = 1, string = "pusta");
	~Tsala();
	void pusta();
	void zajete(int, int);
	void zwalniam(int, int);
	bool czy_wolne(int, int);
	Tsala(Tsala&);
	Tsala& operator=(Tsala&);

	template <class typ>
	friend ostream& operator<<(ostream& screen, Tsala<typ>&object);
};

template<class typ>
inline Tsala<typ>::Tsala(int X, int Y, string name)
{
	rzedy = X;
	miejsca = Y;
	nazwa = name;
	tablica_sali = new typ*[rzedy];
	for (int i = 0; i < rzedy; i++)
	{
		tablica_sali[i] = new typ[miejsca];
	}
}

template<class typ>
Tsala<typ>::~Tsala()
{
	for (int i = 0; i < rzedy; i++)
	{
		delete[] tablica_sali[i];
	}
	delete[] tablica_sali;
}

template<class typ>
inline void Tsala<typ>::pusta()
{
	for (int i = 0; i < rzedy; i++)
	{
		for (int j = 0; j < miejsca; j++)
		{
			tablica_sali[i][j] = "O";
		}
	}
}

template<class typ>
inline void Tsala<typ>::zajete(int x, int y)
{
	tablica_sali[x][y] = "X";
}

template<class typ>
inline void Tsala<typ>::zwalniam(int x, int y)
{
	tablica_sali[x][y] = "O";
}

template<class typ>
inline bool Tsala<typ>::czy_wolne(int x, int y)
{
	try
	{
		if (x<0 || y<0 || x>rzedy || y>miejsca)
		{
			throw (int)1;
		}
		if (tablica_sali[x][y] == "O")
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	catch (int)
	{
		cout << "Nie ma takiego miejsca/rzêdu..." << endl;
		system("pause");
		system("exit");
	}
}

template<class typ>
Tsala<typ>::Tsala(Tsala<typ> & wzor)
{
	this->rzedy = wzor.rzedy;
	this->miejsca = wzor.miejsca;
	this->tablica_sali = new typ*[wzor.rzedy];
	for (int i = 0; i < wzor.rzedy; i++)
	{
		tablica_sali[i] = new typ[wzor.miejsca];
	}
	for (int i = 0; i < rzedy; i++)
	{
		for (int j = 0; j < miejsca; j++)
		{
			tablica_sali[i][j] = wzor.tablica_sali[i][j];
		}
	}
}

template<class typ>
Tsala<typ>& Tsala<typ>::operator=(Tsala<typ> & wzor)
{
	if (&wzor == this) return *this;

	for (int i = 0; i < rzedy; i++)
	{
		delete tablica_sali[i];
	}
	delete[] tablica_sali;

	this->rzedy = wzor.rzedy;
	this->miejsca = wzor.miejsca;
	this->tablica_sali = new typ*[wzor.rzedy];
	for (int i = 0; i < wzor.rzedy; i++)
	{
		tablica_sali[i] = new typ[wzor.miejsca];
	}
	for (int i = 0; i < rzedy; i++)
	{
		for (int j = 0; j < miejsca; j++)
		{
			tablica_sali[i][j] = wzor.tablica_sali[i][j];
		}
	}
}