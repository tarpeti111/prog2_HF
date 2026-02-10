#pragma once
#include "Base.h"
#include "randnum.h"

extern void printImage(string);

class Handler {
private:
	Base* base;
	bool AddRoom();
	bool Connect(); //szobák összekötése
	bool MovePerson();
	string getValidRoomType();	//a standard inputról
	bool AddMachine();
public:
	Handler(Base*);
	void writeTimeAndAddYears(int);
	void Initialize();	//kezdeti feltételek
	void arrival();	//új ember érkezik az airlock-hoz
	int Menu();	//menü rendszer visszatér a választott action-nel (0-9 1db szám) 
	void EndTurnAddResources();	//kör végi alapanyag kalkuláció / esetleges halálok (nincs elég étel vagy víz)
	void AgeHandler();	//életkorok növelése / esetleges halálok (ha valaki 70+ minden évben 50% esélye van meghalni)
	bool Action(int);	//int -> értékû action kiválasztása és végrehajtása
};
