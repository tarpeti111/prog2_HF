#include "Handler.h"

Handler::Handler(Base* other) //gyakorlatilag default konstruktor mert semmi értelme a handlernek Base* nélkül
{
	base = other;
}

//jelenlegi idõ kiírása a körök száma alapján

void Handler::writeTimeAndAddYears(int numYears) {

	time_t currentTime;
	time(&currentTime);

	tm localTime;
	localtime_s(&localTime, &currentTime);

	localTime.tm_year += numYears;

	mktime(&localTime);

	char updatedTimeString[26];
	asctime_s(updatedTimeString, sizeof(updatedTimeString), &localTime);
	std::cout << "Date: " << updatedTimeString;
}

//kezdeti paraméterek beállítása (1 Airlock és 3 random ember)

void Handler::Initialize()
{
	Room r("Airlock", "Airlock");

	base->addRoom(r);

	for (int i = 0; i < 3; i++)
	{
		base->addPerson();
	}
}

//Random ember érkezik az Airlock-ba

void Handler::arrival()
{

	base->addPerson();

	Person p = base->getPeople()->back();

	string tmp = p.getOcc();

	tmp.at(0) = toupper(tmp.at(0));

	system("CLS");
	cout << endl << "A new colonist has arrived at Airlock: " << p.getName() << ", occupation: " << tmp << ", age: " << p.getAge() << "." << endl << endl;

	FileManager::printImage("ARRIVAL");

	system("pause");
}

//Menü ami visszaad egy számot annak függvényében, hogy mit választott a felhasználõ

int Handler::Menu()
{
	string str;

	cout << "What would you like to do?" << endl;
	cout << endl;
	cout << "Make a new room (1) (1 ACTION)" << endl;
	cout << "Connect 2 rooms together (2) (1 ACTION)" << endl;
	cout << "Move someone (3) (1 ACTION)" << endl;
	cout << "Build a new machine for a room (4) (1 ACTION)" << endl;
	cout << "List all rooms (5)" << endl;
	cout << "List all people (6)" << endl;
	cout << "List resources (7)" << endl;
	cout << "List connections of rooms (8)" << endl;
	cout << "No action (9)" << endl;
	cout << "Exit game (0)" << endl;

	//user input
	cout << endl;
	cin >> str;

	//mivel 0-9 opció van és 0-9 pontosan 10 szám van ezért ha a beküldött string 1 hosszú és az elsõ eleme szám csak akkor valid az input

	if (str.length() == (size_t)1 && isdigit(str[0]))
		return stoi(str);

	return -1;
}

//létezõ szobatípus kiválasztása

string Handler::getValidRoomType()
{
	string str;

	while (1)
	{
		cout << "What type of room will it be? (MEDBAY, BIODOME, POWERPLANT, WATERPURIFIER, MINE, FACTORY)" << endl;
		cin >> str;

		for (char& c : str) {
			c = tolower(c);
		}

		if (str == "medbay" || str == "biodome" || str == "powerplant" || str == "waterpurifier" || str == "mine" || str == "factory")
			return str;
		else
		{
			system("CLS");
			cout << "INVALID TYPE" << endl << endl;
			system("pause");
			system("CLS");
		}
	}
	return "\0";
}

//szoba hozzáadása egy új név és egy típus alapján

bool Handler::AddRoom()
{
	string name;
	string type;

	cout << "What will be the name of the room?" << endl;
	cin >> name;

	//Ha már van ilyen nevû szoba akkor vissza a menübe

	if (base->getRoom(name).getId() >= 0)
	{
		cout << "Room with that name already exists" << endl;
		return false;
	}

	type = getValidRoomType();

	Room r(name, type);

	int s = base->getRoomsSize();

	base->addRoom(r);

	//Ha nem sikerült hozzáadni a szobát akkor invalid action

	if (s >= base->getRoomsSize())
	{
		cout << "Failed to create room" << endl << endl;
		return false;

	}

	//ha a szoba erõmû volt akkor +20 energy (+20 mert az 5-öt még mindig leveszi minden szobához)

	if (type == "powerplant")
	{
		base->addResource(ENERGY, 25);
	}

	//output

	type.at(0) = toupper(type.at(0));
	cout << endl << "--------------------------------------------" << endl << endl;
	cout << "Created room: " << name << ", with type: " << type << endl;
	cout << endl << "--------------------------------------------" << endl << endl;

	for (char& c : type) {
		c = toupper(c);
	}

	//kép megjelenítése a szoba nevével
	FileManager::printImage(type);
	return true;
}

//2 szoba osszekötése

bool Handler::Connect()
{
	string a;
	string b;

	//user input

	cout << "Which 2 rooms would you like to connect?" << endl;
	cin >> a;
	cout << "and" << endl;
	cin >> b;
	cout << endl;

	//ha nincs elég alapanyag akkor return

	if (base->getResources()[ALLOY] < 2)
	{
		cout << "Not enough resources" << endl;
		return false;
	}

	int C1 = base->getRoom(a).getId();
	int C2 = base->getRoom(b).getId();

	//ha 2 rossz szoba
	if (C1 < 0 && C2 < 0)
	{
		cout << "Neither of the rooms exist" << endl;
		return false;
	}

	//ha 1 jó szoba kétszer
	if (C1 == C2)
	{
		cout << "Can't connect room to itself" << endl;
		return false;
	}

	//ha 2 jó szoba
	if (C1 >= 0 && C2 >= 0)
	{
		if (base->addConnection(C1, C2))
		{
			cout << "Connection has been made" << endl;
			FileManager::printImage("CORRIDOR");
			return true;
		}
		else
			return false;
	}

	//ha csak az egyik szoba rossz
	cout << "Connection has not been made, one of the rooms was invalid" << endl;

	return false;
}

//kör végi alapanyagok számítása és az abból adódó esetleges halálok számítása
//Az alapanyagok a szobák száma a bennelévõ gépek és emberek függvénye adja 

void Handler::EndTurnAddResources()
{
	for (int i = 0; i < base->getRoomsSize(); i++)
	{
		string tmp = base->getRooms()->at(i).getType();
		int n = base->getNumOfOperatingMachines(base->getRooms()->at(i));	//n = (ember aki nem beteg és olyan munkája van ami a szobához releváns)

		//20% urán vagy 30% fém vagy 50% érc bányászható
		if (tmp == "mine")
		{
			int r = randNum::randRange(1, 10);

			if(r > 5)
				base->addResource(ORE, 5 + (n * 5));
			if(r >= 3 && r <= 5)
				base->addResource(METAL, 5 + (n * 5));
			if (r < 3)
				base->addResource(URANIUM, 5 + (n * 5));
		}

		//a factory 2 ércbõl és 1 fémbõl csinál 1 ötvözetet
		else if (tmp == "factory")
		{
			for(int i = 0; i < (3 + (n * 3)) && base->getResources()[ORE] >= 2 && base->getResources()[METAL] >= 1; i++)
			{
				base->addResource(ORE, -2);
				base->addResource(METAL, -1);
				base->addResource(ALLOY, 2);
			}
		}

		else if (tmp == "biodome")
		{
			base->addResource(FOOD, 5 + (n * 5));
		}

		else if (tmp == "waterpurifier")
		{
			base->addResource(WATER, 10);
		}

		else if (tmp == "powerplant")
		{
			base->addResource(URANIUM, -1);
		}
	}

	//emberek álltal elhasznált víz/étel
	base->addResource(WATER, base->getPeopleSize() * -2);
	base->addResource(FOOD, base->getPeopleSize() * -2);

	//éhen-/szomjanhalás eldöntése

	int ogPplSize = base->getPeopleSize();

	//ha elhasználás után kevesebb étel van mint 0 vagy kevesebb ví van mint 0 minen embernek 70% esélye van meghalni
	if (base->getResources()[FOOD] < 0 || base->getResources()[WATER] < 0)
	{
		for (int i = 0; i < base->getPeopleSize(); i++)
		{
			int n = randNum::randRange(1, 10);
			if (n > 3)
			{
				cout << base->getPeople()->at(i).getName() << " has died of starvation or dehydration." << endl;
				base->removePerson(base->getPeople()->at(i));
			}
		}
	}

	//ha az étel kisebb mint 0 akkor a az érték -1 szeresének hozzáadása (a + (0-a) = 0)  
	if (base->getResources()[FOOD] < 0)
		base->addResource(FOOD, 0 - base->getResources()[FOOD]);

	//egyan az csak vízzel
	if (base->getResources()[WATER] < 0)
		base->addResource(FOOD, 0 - base->getResources()[WATER]);

	//ha kevesebb ember van mint volt az éhen-/szomjanhalás eldöntés elõtt akkor biztosan meghalt valaki
	if (ogPplSize < base->getPeopleSize())
	{
		system("pause");
	}
}

//életkorok állítása, esetleges halálozások eldöntées

void Handler::AgeHandler()
{
	for (int i = 0; i < base->getPeopleSize(); i++)
	{
		//életkor incrementálása
		base->getPeople()->at(i).incAge();

		//aki 70 év fölött van 50% eséllyel meghalhat végelgyengülésben
		if (base->getPeople()->at(i).getAge() >= 70)
		{
			int n = randNum::randRange(0, 1);
			if (n == 0)
			{
				cout << base->getPeople()->at(i).getName() << " has died of old age, at age: " << base->getPeople()->at(i).getAge() << endl;
				base->removePerson(base->getPeople()->at(i));
				system("pause");
			}
		}
	}
}

//ember mozgatása egyik szobából a másikba (közvetlen összeköttetés szükségeltetett)

bool Handler::MovePerson()
{
	string name;
	string room;

	// user input
	cout << "who" << endl;
	cin >> name;

	//formázás
	for (char& c : name) {
		c = tolower(c);
	}
	name.at(0) = toupper(name.at(0));

	//user input
	cout << "where" << endl;	
	cin >> room;

	//ha invalid ember akkor return flase
	Person p = base->getPerson(name);
	if (p.getId() < 0) { cout << "Invalid Person" << endl; return false; }

	//ha invalid szoba akkor return false
	Room r = base->getRoom(room);
	if (r.getId() < 0) { cout << "Invalid Room" << endl; return false; }

	//összeköttetés keresése
	// 
	//key beállítása
	int key[2] = { r.getId(), p.getCurrentRoom() };
	vector<vector<int>> connections = *base->getConnections();

	//szûrés a key-re
	for (int i = 0; i < connections.size(); i++)
	{
		if ((connections.at(i).at(0) == key[0] && connections.at(i).at(1) == key[1]) || (connections.at(i).at(1) == key[0] && connections.at(i).at(0) == key[1]))
		{
			//ha van egyezés akkor return ture
			base->getPerson(name).move(r.getId());
			return true;
		}
	}

	//ha nem volt egyezés a key-el akkor return false
	cout << "Rooms not connected directly" << endl;
	return false;
}

//gép hozzáadása szobához

bool Handler::AddMachine()
{
	string rname;
	cout << "In which room would you like to make a new machine (if its a medbay you are makeing a new bed)" << endl;
	cin >> rname;

	//ha invalid a szoba return false
	if (base->getRoom(rname).getId() < 0)
	{
		cout << "INVALID ROOM" << endl;
		return false;
	}

	//ha a szoba Airlock vagy water purifier vagy powerplant return false, mert semmi értelme gépet rakni bele (nincs hozzájuk munka)
	else if (base->getRoom(rname).getId() == 0 || base->getRoom(rname).getType() == "waterpurifier" || base->getRoom(rname).getType() == "powerplant")
	{
		cout << "There is no reason to add a machine to a" << base->getRoom(rname).getType() << endl;
		return false;
	}

	else
	{
		// ha nincs 2 alloy akkor return false
		if (base->getResources()[ALLOY] < 2)
		{
			cout << "Not enough resources" << endl;
			return false;
		}

		//2 alloy elhasználása, gép hozzáadása
		base->addResource(ALLOY, -2);
		base->getRoom(rname).IncRoomMachines();
		cout << "new machine added to room " << base->getRoom(rname).getName() << endl;
		return true;
	}
}

//döntés kiválasztása és végrehajtása

bool Handler::Action(int actId)
{
	bool valid = false;

	switch (actId)
	{
	case 1:

		valid = AddRoom();
		break;

	case 2:

		valid = Connect();
		break;

	case 3:
		valid = MovePerson();
		break;
	case 4:
		valid = AddMachine();
		break;
	case 5:
		base->listRooms();
		break;
	case 6:
		base->listPeople();
		break;
	case 7:
		base->listResources();
		break;

	case 8:
		base->listConnections();
		break;
	case 9:
		valid = true;
		break;

	default:

		cout << "INVALID ACTION" << endl;
		break;
	}
	return valid;
}