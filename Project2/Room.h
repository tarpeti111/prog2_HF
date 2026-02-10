#pragma once
#include "Person.h"
#include <vector>

//alapanyagok
enum { ORE, METAL, ALLOY, URANIUM, WATER, FOOD, ENERGY };

class Room
{
private:
	string name;
	string type;
	int id;
	static int lastid;
	int NumofMachines = 0;

	//építéshez szükséges alapanyagok
	int cost1 = 0;
	int costType1 = 0;
	int cost2 = 0;
	int costType2 = 0;

public:

	Room(string, string);
	Room(int);

	int getId() const { return id; }
	string getName() const { return name; }
	string getType() const { return type; }
	int getNumOfMachines() const { return NumofMachines; }

	int getCost1() const { return cost1; }
	int getCost2() const { return cost2; }
	int getCostType1() const { return costType1; }
	int getCostType2() const { return costType2; }

	void addMachine(int n = 1) { NumofMachines += n; }

	void lastidInc(){ lastid++; }
	void IncRoomMachines() { NumofMachines++; }
};