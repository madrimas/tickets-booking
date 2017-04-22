#include "Tadmin.h"

Tadmin::Tadmin()
{
}


Tadmin::~Tadmin()
{
}

void Tadmin::pobierz_dane()
{
	cin >> login;
	cin >> haslo;
}

bool Tadmin::czy_poprawne(string wzor_login, string wzor_haslo)
{
	if ((login == wzor_login) && (haslo == wzor_haslo))
	{
		return true;
	}
	else
	{
		return false;
	}
}