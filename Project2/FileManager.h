#pragma once
#include <sstream>
#include <fstream>
#include <string>
#include "randnum.h"

class FileManager {
public:
	static std::string randName();	//random név a NAMES.txt bõl
	static void printImage(std::string s);	//"kép" kiírása (s.txt)
};