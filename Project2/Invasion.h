#pragma once
#include "Base.h"

class Invasion{
private:

	vector<vector<int>> ReachableRooms(int current);	//stobák id-je amiket el tud érni az alien
	bool HasSecurityMovedToEnemy();	//van e "security" munkájú ember az ûrlénynél jelenleg
	bool InvasionKill();	//az alien megöl mindenkit a szobában és visszaadja, hogy volt e kill
	void InvasionMove();	//az alien mozog egy szobába ahol látja hogy vannak, vagy egy random szobába ha nem lát senkit

protected:

	bool invasionState = false;
	int enemyroom = -1;	//alapból az alien nincs sehol
	Base* base;

public:

	Invasion(Base*);	//gyakorlatilag default konstruktor mert semmi értelme az Invasion-nek Base* nélkül

	void DoInvasion();	//Alien "AI"

	bool getInvasionState() const { return invasionState; }
	int getEnemyRoom() const { return enemyroom; }
};