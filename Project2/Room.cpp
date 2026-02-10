#pragma once
#include "Room.h"

int Room::lastid = 0;

//szoba konstruktor id-vel
Room::Room(int id) : name("f"), type("f"), id(id), cost1(0), costType1(0), cost2(0), costType2(0) {}

//szoba konstruktor névvel és típussal, a típus függvényében az építéshez szükséges alapanyagok beállítása
Room::Room(string name, string type) : name(name), type(type), id(lastid)
{
	if (type == "medbay")
	{
		cost1 = 10;
		costType1 = WATER;
		cost2 = 10;
		costType2 = ALLOY;
	}
	else if (type == "biodome")
	{
		cost1 = 10;
		costType1 = WATER;
		cost2 = 10;
		costType2 = ALLOY;
	}
	else if (type == "powerplant")
	{
		cost1 = 50;
		costType1 = URANIUM;
		cost2 = 10;
		costType2 = ALLOY;
	}
	else if (type == "waterpurifier")
	{
		cost1 = 10;
		costType1 = METAL;
		cost2 = 10;
		costType2 = ALLOY;
	}
	else if (type == "mine")
	{
		cost1 = 10;
		costType1 = ORE;
		cost2 = 10;
		costType2 = ALLOY;
	}
	else if (type == "factory")
	{
		cost1 = 10;
		costType1 = METAL;
		cost2 = 10;
		costType2 = ALLOY;
	}
}