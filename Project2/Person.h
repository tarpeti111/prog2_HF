#pragma once
#include <string>
#include "FileManager.h"
#include <iostream>

using namespace std;

class Person
{
private:
	string name;
	string occupation;
	int id;
	static int lastid;
	unsigned age;
	bool sick = false;
	int currentRoom = 0;	//szoba id ahol az ember van

	string randOcc(); //random munka

public:
	Person(string&);	//ember konstruktor névvel
	Person(int);	//ember konstruktor id-vel

	string getOcc() const { return occupation; }
	string getName() const { return name; }
	int getId() const { return id; }
	int getAge() const { return age; }
	int getCurrentRoom() const{ return currentRoom; }
	bool getSick() const { return sick; }

	void move(int n) { currentRoom = n; }
	void setSick(bool b) { sick = b; }

	void lastidInc() { lastid++; }
	void incAge() { age++; }
};