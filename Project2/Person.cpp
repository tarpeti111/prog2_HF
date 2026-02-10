#pragma once
#include "Person.h"

int Person::lastid = 0;

//random occupation (munka)
string Person::randOcc()
{
	int n = randNum::randRange(0, 4);

	switch (n)
	{
	case 0:
		return "Medic";
	case 1:
		return "Botainst";
	case 2:
		return "Engineer";
	case 3:
		return "Miner";
	case 4:
		return "Security";
	default:
		return "Jobless";
	}
}

//ember konstruktor névvel
Person::Person(string& name) : name(name), occupation(randOcc()), age(randNum::randRange(20, 70)), id(lastid) {}

//ember konstruktor id-vel
Person::Person(int n) : name("f"), occupation("f"), age(0), id(n) {}
