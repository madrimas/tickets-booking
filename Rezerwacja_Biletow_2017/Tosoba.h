#pragma once

#include <iostream>
#include <string>
using namespace std;

class Tosoba
{
protected:
	string login;
public:
	virtual void pobierz_dane() = 0;
};