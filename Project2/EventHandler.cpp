#include "EventHandler.h"

EventHandler::EventHandler(Base* b) : Invasion(b){} //gyakorlatilag default konstruktor mert semmi értelme az EventHandler-nek Base* nélkül

//Épület ami nem az 'Airlock' "megsemmisül" 90% eséllyel, aki benne van meghal

void EventHandler::AsteroidFall()
{
	//esély számítás

	if (randNum::randRange(1, 10) > 1 && base->getRoomsSize() > 1)

		//random szoba megsemmisül
	{
		Room& r = base->getRooms()->at(randNum::randRange(1, base->getRoomsSize()-1));

		cout << endl << r.getName() << " has been destroyed by an asteroid" << endl << endl;

		cout << "--------------------------------------------------" << endl;

		int rid = r.getId();	//room id elmentése az alien-hez

		base->removeAllConnectionOfRoom(r);
		base->removeRoom(r);

		//aki a szobában volt az most a "rossz" szobában van aminek az id je '-1', aki itt van meghal 

		for (int i = 0; i < base->getPeopleSize(); i++)
		{
			if (base->getRoom(base->getPeople()->at(i).getCurrentRoom()).getId() == -1)
			{
				cout << base->getPeople()->at(i).getName() << " has died as a consiquence." << endl;
				cout << "--------------------------------------------------" << endl;

				base->removePerson(base->getPeople()->at(i));
				i--;
			}
		}

		//Ha az alien a szobában volt õ is meghal 

		if (enemyroom == rid)
		{
			cout << "Alien has also been killed" << endl;
			enemyroom = -1;
			invasionState = false;
		}
	}

	else
		cout << endl << "Nothing was destroyed" << endl << endl;
}

//Épület ami nem az 'Airlock' "megsemmisül" 50% eséllyel, aki benne van meghal

void EventHandler::Marsquake()
{
	//esély számítás

	if (randNum::randRange(1, 10) > 5 && base->getRoomsSize() > 1)
	{
		//random szoba megsemmisül

		Room& r = base->getRooms()->at(randNum::randRange(1, base->getRoomsSize()-1));

		cout << endl << r.getName() << " has been destroyed in a marsquake" << endl << endl;

		cout << "--------------------------------------------------" << endl;

		int rid = r.getId(); //room id elmentése az alien-hez

		base->removeAllConnectionOfRoom(r);
		base->removeRoom(r);

		//aki a szobában volt az most a "rossz" szobában van aminek az id je '-1', aki itt van meghal 

		for (int i = 0; i < base->getPeopleSize(); i++)
		{
			if (base->getRoom(base->getPeople()->at(i).getCurrentRoom()).getId() == -1)
			{
				cout << base->getPeople()->at(i).getName() << " has died as a consiquence." << endl;
				cout << "--------------------------------------------------" << endl;

				base->removePerson(base->getPeople()->at(i));
				i--;
			}
		}

		//Ha az alien a szobában volt õ is meghal 

		if (enemyroom == rid)
		{
			cout << "Alien has also been killed" << endl;
			enemyroom = -1;
			invasionState = false;
		}
	}
	else
		cout << endl << "Nothing was destroyed" << endl << endl;
}

//Egy teljesen random ember teljesen random meghal


void EventHandler::HearthAttack()
{
	if (base->getPeopleSize() < 1)
		return;

	Person& p = base->getPeople()->at(randNum::randRange(0, base->getPeopleSize()-1));

	cout << endl << p.getName() << " has died in a heart attack." << endl << endl;

	base->removePerson(p);
}

//2 teljesen random ember megbetegszik

void EventHandler::Sickness()
{
	sickness = true;
	int f = 0;

	cout << endl;

	for (int i = 0; i < base->getPeopleSize(); i++)
	{
		if (!base->getPeople()->at(i).getSick())
			f++;
	}
	if (f > 2) { f = 2; }

	for (int i = 0; i < f; i++)
	{
		Person &p = base->getPeople()->at(randNum::randRange(0, base->getPeopleSize()-1));

		if (p.getSick())
		{
			i--;
		}

		else
		{
			p.setSick(true);
			cout << p.getName() << " has become very sick." << endl;
			cout << "-----------------------------" << endl;
		}
	}
	cout << endl << "They all have to go to a Medbay with functioning beds" << endl << endl;
}

//Ha a beteg ember eljutott egy medbay be ahol van ágy és medic akkor meggyógyul (ha több beteg ember van a medbay-ben mint ágy amihez orvos akkor minden beteg meghal)

void EventHandler::SicknessCheck()
{
	vector<Person*> peopleInMeds;
	vector<Room*> Meds;

	//medbay-ek eltárolása

	for (int i = 0; i < base->getRoomsSize(); i++)
	{
		if (base->getRooms()->at(i).getType() == "medbay")
			Meds.push_back(&base->getRooms()->at(i));
	}

	//olyan beteg emberek eltárolása akik medbay-ekben vannak

	for (int i = 0; i < base->getPeopleSize(); i++)
	{
		Person* p = &base->getPeople()->at(i);
		if (p->getSick() && base->getRoom(p->getCurrentRoom()).getType() == "medbay")
		{
			peopleInMeds.push_back(p);
		}
	}

	//emberek esetleges gyógyítása

	for (int i = 0; Meds.size() > i; i++)
	{
		int n = base->getNumOfOperatingMachines(*Meds.at(i)); //n = ágy amihez van medic ebben a medbayben
		int pplInthisMed = 0;

		//emberek száma ebben a medbay ben

		for (int j = 0; j < peopleInMeds.size(); j++)
		{
			if (peopleInMeds.at(j)->getCurrentRoom() == Meds.at(i)->getId())
				pplInthisMed++;
		}

		//ha több beteg ember van a medbay-ben mint ágy amihez orvos akkor minden beteg meghal

		if (pplInthisMed <= n)
		{
			for (int j = 0; j < peopleInMeds.size(); j++)
			{
				if (peopleInMeds.at(j)->getCurrentRoom() == Meds.at(i)->getId())
					peopleInMeds.at(j)->setSick(false);
			}
		}
	}
}

//aki a kör/év végén beteg maradt meghal

void EventHandler::SicknessKill()
{
	//Only use at end of turn/year
	int ogSize = base->getPeopleSize();

	for (int i = 0; i < base->getPeopleSize(); i++)
	{
		if (base->getPeople()->at(i).getSick())
		{
			cout << base->getPeople()->at(i).getName() << " has died of a disease" << endl;
			base->removePerson(base->getPeople()->at(i));
			i--;
		}
	}

	//ha kevesebb ember van mint a for ciklus elõtt volt akkor biztosan meghalt legalább 1 ember

	if (ogSize > base->getPeopleSize())
	{
		FileManager::printImage("SICKDEATH");
		system("pause");
	}
}

void EventHandler::DoRandomEvent()
{
	int n = randNum::randRange(1, 5);
	//int n = 1;
	system("CLS");

	switch (n)
	{
	case 1:
		//asteroids (literally a random building dissapears)
		FileManager::printImage("ASTEROIDS");
		system("pause");
		AsteroidFall();
		break;
	case 2:
		//alien invasion
		if (!invasionState)
		{
			invasionState = true;
			enemyroom = 0;
			cout << "INVASION HAS STARTED" << endl;
			FileManager::printImage("ALIEN");
		}
		break;
	case 3:
		//random ppl get sick (like 2) and they have to go to medbay before end of turn
		cout << "A VIRUS HAS BROKEN OUT" << endl << endl;
		FileManager::printImage("VIRUS");
		system("pause");
		Sickness();
		break;
	case 4:
		//asteroid but less likely to cause collapse
		FileManager::printImage("EARTHQUAKE");
		system("pause");
		Marsquake();
		break;
	case 5:
		//literally a random death
		FileManager::printImage("HEARTATTACK");
		system("pause");
		HearthAttack();
		break;

	default:
		break;
	}
}