#pragma once
#include "Room.h"
#include <algorithm>

class Base
{
private:
	vector<Room> rooms;	//szobák tárolása
	vector<vector<int>> connections;	//szobák összeköttetésének tárolása
	vector<Person> people;	//emberek tárolása
	int resources[7];	//alapanyagok tárolása
	Room badRoom;	//"rossz" szoba deifiniálása
	Person badPerson; //"rossz" ember deiniálása

	bool matchOccWithType(string, string);	//ture ha a szoba típushoz releváns az ember munkája
	void writeCharNtimes(char, int);	// n-szer kiírja 'c'-t
	string resourceToStr(int);	//alapanyagból string készítése (pl ORE ami egy enum-olt int -> "ore" ami egy string)

	void removeConnection(int a, int b); //szoba összeköttetés elvétele a és b id-jû szobák között
public:
	Base(); //default konstruktor

	size_t getConnectionsSize() const { return connections.size(); }
	size_t getRoomsSize() const { return rooms.size(); }
	size_t getPeopleSize() const { return people.size(); }
	vector<Room>* getRooms() { return &rooms; }
	vector<Person>* getPeople() { return &people; }
	vector<vector<int>>* getConnections() { return &connections; }
	int* getResources() { return resources; }
	
	//NumOfOperatingMachines az a számi ami megmondja hogy a szobában hány géphez van olyan ember aki tudja operálni (nem beteg és releváns a munkája)
	int getNumOfOperatingMachines(Room&);

	void addResource(int type, int N) { resources[type] += N; }
	void addRoom(Room&);
	void addPerson(); //nincs Person referencia mert mindig random
	void removeRoom(Room&);
	void removeRoom(int); // int -> szoba id
	void removePerson(Person&);
	void removePerson(int); // int -> ember id
	void addConnection(Room& a, Room& b);//összeköttetés hozzáadása a és b szobák között
	bool addConnection(int a, int b);//int -> szoba id
	void removeAllConnectionOfRoom(Room&); //összes összeköttetés elvétele 1 bizonyos szobától
	void listRooms();
	void listPeople();
	void listResources();
	void listConnections();
	Room& getRoom(string&); // szoba keresése névvel
	Room& getRoom(int); // szoba keresése id vel
	Person& getPerson(string&); // ember keresése névvel
	Person& getPerson(int); // ember keresése id vel
};