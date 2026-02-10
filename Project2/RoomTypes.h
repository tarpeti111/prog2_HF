#pragma once
#include "Room.h"

class Medbay : public Room
{
private:
	int numOfBeds = 0;

	int resource1 = ALLOY;
	int cost1 = 10;

	int resource2 = WATER;
	int cost2 = 30;
public:

	int getResourceType1()
	{
		return resource1;
	}

	int getResourceType2()
	{
		return resource2;
	}
};

class Biodome : public Room
{
	int numOfVegetablePlants = 0;
	int numOfOxygenPlants = 0;

	int resource1 = ALLOY;
	int cost1 = 10;

	int resource2 = WATER;
	int cost2 = 30;

public:
	int getResourceType1()
	{
		return resource1;
	}

	int getResourceType2()
	{
		return resource2;
	}
};