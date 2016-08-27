#pragma once
#include "UI.h"
#include "Parser.h"
#include "CommandValidations.h"
#include <string>

using namespace std;

class CommandLineInterface
{
	UI ui;
	unsigned int size;

	int evaluateCommands(list<string>* sentence);

public:


	CommandLineInterface();
	void createDisk(list<string>* arguments);
	void loopMenu();
	int mountFileSystem(list<string>* arguments);
	int openDisk(string diskName);
	int createEmptyFile(string fileName);

	~CommandLineInterface();
};

