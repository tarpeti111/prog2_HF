#include "Invasion.h"

Invasion::Invasion(Base* b)	//gyakorlatilag default konstruktor mert semmi értelme az Invasion-nek Base* nélkül
{
	base = b;
}

//szobák amiket el tud érni az alien (úgy kell elképzelni mint egy BFS algoritmussal alkotott gráf aminek a gyökér pontja az ûrlény jelenlegi helye a mélysége pedig 1)

vector<vector<int>> Invasion::ReachableRooms(int current)
{
	vector<vector<int>> tmp;
	vector<vector<int>> connections = *base->getConnections();

	for (int i = 0; i < connections.size(); i++)
	{
		if (connections.at(i)[0] == current || connections.at(i)[1] == current)
			tmp.push_back(connections.at(i));
	}

	return tmp;
}

//van e "security" munkájú ember az ûrlénynél jelenleg

bool Invasion::HasSecurityMovedToEnemy()
{
	vector<Person> people = *base->getPeople();

	for (int i = 0; i < people.size(); i++)
	{
		if (people.at(i).getOcc() == "Security" && people.at(i).getCurrentRoom() == enemyroom)
			return true;
	}
	return false;
}

//Az alien megöl mindenkit a jelenlegi szobában

bool Invasion::InvasionKill()
{
	vector<Person>* people = base->getPeople();
	int ogSize = base->getPeopleSize();

	for (int i = 0; i < people->size(); i++)
	{
		if (people->at(i).getCurrentRoom() == enemyroom)
		{
			cout << "Alien has killed " << people->at(i).getName() << endl;
			cout << "----------------------------------------------" << endl;
			base->removePerson(people->at(i));
			i--;
		}
	}

	if (ogSize > people->size())
	{
		FileManager::printImage("KILL");
		system("pause");
		return true;
	}
	return false;
}

//Az alien mozog

void Invasion::InvasionMove()
{
	vector<Person>* people = base->getPeople();

	vector<vector<int>> seeing = ReachableRooms(enemyroom); //szobák amiket "lát" az alien
	int ogRoom = enemyroom;

	//ha van ember a szobákban amiket lát az alien akkor elmozdul afelé az ember felé akit elõször meglát
	for (int i = 0; i < people->size(); i++)
	{
		int pid = people->at(i).getCurrentRoom();

		for (int j = 0; j < seeing.size(); j++)
		{
			if (pid == seeing.at(j)[0])
				enemyroom = pid;

			else if (pid == seeing.at(j)[1])
				enemyroom = pid;
		}

		//ha elmozdult az alien akkor break
		if (ogRoom != enemyroom)
			break;
	}

	//ha nem mozdult el semerre az enemy (nem látott senkit) akkor egy random szobába megy
	if (ogRoom == enemyroom)
	{
		int n = randNum::randRange(0, seeing.size() - 1);

		if (seeing.at(n)[0] != enemyroom)
			enemyroom = seeing.at(n)[0];
		else if (seeing.at(n)[1] != enemyroom)
			enemyroom = seeing.at(n)[1];
	}
}

//az alien "AI"-ja

void Invasion::DoInvasion()
{
	//ha nincs jelenleg security az alien en
	if (!HasSecurityMovedToEnemy())
	{
		//ha tud "ölni" az alien akkor "öl" és kilép
		if (InvasionKill())
			return;

		//ha nem tud akkor "mozog "lép" egyet
		InvasionMove();

		//ha az alien oda "lépett" ahol van "security" akkor 80% esélye van megülni a security-t
		if (HasSecurityMovedToEnemy())
		{
			if (randNum::randRange(1, 10) > 8)
			{
				invasionState = false;
				FileManager::printImage("ALIENDEAD");
				system("pause");
			}
			else
				InvasionKill();
		}

		else
			InvasionKill();
	}

	//ha van security az alien en a kör a lépés kezdetén akkor 80% esélye van a "security" nak
	else
	{
		if (randNum::randRange(1, 10) > 2)
		{
			invasionState = false;
			FileManager::printImage("ALIENDEAD");
			system("pause");
		}
		else
			InvasionKill();
	}
}