#pragma once
#include "Tosoba.h"

class Tadmin : public Tosoba
{
protected:
	string haslo;
public:
	Tadmin();
	~Tadmin();
	void pobierz_dane();
	bool czy_poprawne(string, string);
};