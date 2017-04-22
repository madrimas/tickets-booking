#pragma once
#include "Tosoba.h"

class Tklient : public Tosoba
{
public:
	Tklient();
	~Tklient();
	void pobierz_dane();
	void wyswietl_dane();
	string wyrzuc_login();
};