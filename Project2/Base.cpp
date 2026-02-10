#pragma once
#include "Base.h"

//default konstruktor

Base::Base() : badRoom(-1), badPerson(-1)
{
	rooms.clear();
	people.clear();
	connections.clear();

	fill(resources, resources + 7, 40);
}

//alapanya átkonvertálása string-é

string Base::resourceToStr(int r)
{
	switch (r)
	{
	case ORE:
		return "ORE";
	case METAL:
		return "METAL";
	case ALLOY:
		return "ALLOY";
	case URANIUM:
		return "URANIUM";
	case WATER:
		return "WATER";
	case FOOD:
		return "FOOD";
	case ENERGY:
		return "ENERGY";
	default:
		return "ERROR";
	}
}

//összeköttetés hozzáadása ha még nem létezik

bool Base::addConnection(int a, int b)
{
	for (int i = 0; i < connections.size(); i++)
	{
		if ((connections.at(i).at(0) == b && connections.at(i).at(1) == a) || (connections.at(i).at(0) == a && connections.at(i).at(1) == b))
		{
			cout << "Theres already a connection!" << endl;
			return false;
		}
	}

	resources[ALLOY] -= 2;

	connections.push_back({ a,b });
	return true;
}

void Base::addConnection(Room& a, Room& b)
{
	addConnection(a.getId(), b.getId());
}

//összeköttetés elvétele

void Base::removeConnection(int a, int b)
{
	for (int i = 0; i < connections.size(); i++)
	{
		if ((connections.at(i).at(0) == b && connections.at(i).at(1) == a) || (connections.at(i).at(0) == a && connections.at(i).at(1) == b))
		{
			connections.erase(connections.begin() + i);
			return;
		}
	}
}

//szoba összes összeköttetésének elvétele

void Base::removeAllConnectionOfRoom(Room& r)
{
	int n = r.getId();

	for (int i = 0; i < connections.size(); i++)
	{
		if (connections.at(i)[0] == n)
		{
			removeConnection(connections.at(i)[1], n);
			i--;
		}
		else if (connections.at(i)[1] == n)
		{
			removeConnection(connections.at(i)[0], n);
			i--;
		}
	}
}

//szoba hozzáadáasa, szükséges alapanyagok elvétele

void Base::addRoom(Room& room)
{
	//ha nincs elég alapanyag)
	if (resources[room.getCostType1()] - room.getCost1() < 0 || resources[room.getCostType2()] - room.getCost2() < 0)
	{
		cout << "Not enough resources" << endl;
		return;
	}
	if (resources[ENERGY] < 5 && room.getType() != "powerplant")
	{
		cout << "Not enough energy" << endl;
		return;
	}

	rooms.push_back(room);

	resources[room.getCostType1()] -= room.getCost1();
	resources[room.getCostType2()] -= room.getCost2();
	resources[ENERGY] -= 5;

	room.lastidInc();
}

//szoba törlése

void Base::removeRoom(Room& room)
{
	for (int i = 0; i < rooms.size(); i++)
	{
		if (rooms.at(i).getId() == room.getId())
		{
			rooms.erase(rooms.begin() + i);
		}
	}
}

void Base::removeRoom(int id)
{
	removeRoom(getRoom(id));
}

//ember hozzáadása

void Base::addPerson()
{
	//ha nem kevesebb mint 20 emebr van akkor kilépés
	if (people.size() >= 20)
	{
		cout << "max num of ppl reached already" << endl;
		return;
	}

	string tmp = FileManager::randName(); // random név

	for (int i = 0; i < people.size(); i++)
	{
		//ha van már ilyen név akkor új név és repeat ameddig nem talál olyat ami nincs használva
		if (people.at(i).getName() == tmp)
		{
			tmp = FileManager::randName();
			i = 0;
		}
	}

	Person p(tmp);

	people.push_back(p);

	p.lastidInc();
}

//ember elvétele

void Base::removePerson(int id)
{
	for (int i = 0; i < people.size(); i++)
	{
		if (people.at(i).getId() == id)
		{
			people.erase(people.begin() + i);
		}
	}
}

void Base::removePerson(Person& p)
{
	removePerson(p.getId());
}

//szoba getter, ha nem találja a keresett szobát akkor a default "rossz" szobát adja vissza

Room& Base::getRoom(string& name)
{
	for (int i = 0; i < rooms.size(); i++)
	{
		if (rooms.at(i).getName() == name)
			return rooms.at(i);
	}
	return badRoom;
}

Room& Base::getRoom(int id)
{
	for (int i = 0; i < rooms.size(); i++)
	{
		if (rooms.at(i).getId() == id)
			return rooms.at(i);
	}
	return badRoom;
}

//ember getter, ha nem találja a keresett szobát akkor a default "rossz" embert adja vissza

Person& Base::getPerson(int id)
{
	for (int i = 0; i < people.size(); i++)
	{
		if (people.at(i).getId() == id)
			return people.at(i);
	}
	return badPerson;
}

Person& Base::getPerson(string& name)
{
	for (int i = 0; i < people.size(); i++)
	{
		if (people.at(i).getName() == name)
			return people.at(i);
	}
	return badPerson;
}

//n-szer kiírja a 'c' karaktert

void Base::writeCharNtimes(char c, int n)
{
	for (int i = 0; i < n; i++)
	{
		cout << c;
	}
}

void Base::listRooms()
{
	for (int i = 0; i < rooms.size(); i++)
	{
		cout << rooms.at(i).getName() << " type: " << rooms.at(i).getType();
		int s = rooms.at(i).getName().size() + 7 + rooms.at(i).getType().size();
		if (rooms.at(i).getNumOfMachines() > 0)
		{
			cout << " machines inside: " << rooms.at(i).getNumOfMachines();
			s += 20;
		}
		cout << endl;
		writeCharNtimes('-', s);
		cout << endl;
	}
}

void Base::listPeople()
{
	for (int i = 0; i < people.size(); i++)
	{
		Person* p = &people.at(i);

		cout << p->getName() << " Occupation: " << p->getOcc() << ". They are at: " << getRoom(p->getCurrentRoom()).getName() << " AGE: " << p->getAge();
		int s = p->getName().size() + 13 + p->getOcc().size() + 15 + getRoom(p->getCurrentRoom()).getName().size() + 6 + 2;

		if (p->getSick())
		{ 
			cout << " IS SICK";
			s += 8;
		}
		cout << endl;
		
		writeCharNtimes('-', s);
		cout << endl;
	}
}

void Base::listResources()
{
	for (int i = 0; i < 7; i++)
	{
		cout << resourceToStr(i) << ":";

		int n = 20 - resourceToStr(i).size() - 1 - 2;
		writeCharNtimes(' ', n);
		cout << resources[i] << endl;

		int s = resourceToStr(i).size() + 2 + 2;
		writeCharNtimes('-', 20);
		cout << endl;
	}
}

void Base::listConnections()
{
	for (int i = 0; i < connections.size(); i++)
	{
		Room a = this->getRoom(connections.at(i)[0]);
		Room b = this->getRoom(connections.at(i)[1]);

		cout << "Room " << a.getName() << " is connected to room " << b.getName() << endl;
		
		int s = 5 + a.getName().size() + 22 + b.getName().size();
		writeCharNtimes('-', s);
		cout << endl;
	}
}

bool Base::matchOccWithType(string occ, string type)
{
	if (type == "medbay" && occ == "Medic")
		return true;
	else if (type == "mine" && occ == "Miner")
		return true;
	else if (type == "biodome" && occ == "Botanist")
		return true;
	else if (type == "factory" && occ == "Engineer")
		return true;

	return false;
}

//NumOfOperatingMachines az a számi ami megmondja hogy a szobában hány géphez van olyan ember aki tudja operálni (nem beteg és releváns a munkája)

int Base::getNumOfOperatingMachines(Room& r)
{
	int tmp = 0;

	for (int i = 0; i < people.size(); i++)
	{
		if (matchOccWithType(people.at(i).getOcc(), r.getType()) && people.at(i).getCurrentRoom() == r.getId() && !people.at(i).getSick())
			tmp++;
	}
	return tmp;
}
