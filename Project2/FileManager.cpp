#include "FileManager.h"

using namespace std;

//random név kiválasztása a NAME.txt bõl

string FileManager::randName()
{
	string name;

	ifstream ifile;
	ifile.open("NAMES.txt", ios::in);

	srand(time(nullptr));
	int n = randNum::randRange(1, 20);

	if (ifile.is_open())
	{
		for (int i = 0; i < n; i++)
		{
			getline(ifile, name);
		}
	}

	ifile.close();

	return name;
}

//"kép" kiírása a filename alapján (ASCII art-ok)

void FileManager::printImage(string s)
{
	s += ".txt";
	ifstream inputFile(s);

	if (inputFile.is_open()) {
		string line;
		while (getline(inputFile, line)) {
			cout << line << endl;
		}
		inputFile.close();
	}
	else {
		cout << "Failed to open the file." << endl;
	}
}