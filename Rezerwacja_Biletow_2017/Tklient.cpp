#include "Tklient.h"

Tklient::Tklient()
{
}


Tklient::~Tklient()
{
}

void Tklient::pobierz_dane()
{
	cout << "Podaj nazw� u�ytkownika, b�dzie ona potrzebna do odbioru bilet�w!" << endl;
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
