#pragma once
#include "UI.h"
#include "Parser.h"
#include "CommandValidations.h"
#include "ErrorHandler.h"
#include <string>
#include <stdlib.h>

using namespace std;

class CommandLineInterface
{
	UI ui;
	unsigned int size;
	list<string> commands;

	int help();
	int close();
	int evaluateCommands(list<string>* sentence);

public:


	CommandLineInterface();
	int createDisk(list<string>* arguments);
	void loopMenu();
	int mountFileSystem();
	int unmountFileSystem();
	int openDisk(string diskName);
	int createEmptyFile(string fileName);
	int listFiles();
	int showFileSystemInfo();

	~CommandLineInterface();
};

