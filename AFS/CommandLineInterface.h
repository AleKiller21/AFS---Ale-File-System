#pragma once
#include "UI.h"
#include "Parser.h"
#include <string>

using namespace std;

class CommandLineInterface
{
	UI ui;
	streamsize size;

	void evaluateCommands(list<string>* sentence);

public:


	CommandLineInterface();
	void createDisk(streamsize size, string diskName);
	void loopMenu();
	int mountFileSystem(string diskName, char partition);
	int openDisk(string diskName);
	int createEmptyFile(string fileName);

	~CommandLineInterface();
};

