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

	int createDisk(list<string>* arguments);
	int formatDisk(string diskName);
	int mountFileSystem();
	int unmountFileSystem();
	int openDisk(string diskName);
	int createEmptyFile(string fileName);
	int listFiles() const;
	int showFileSystemInfo() const;
	int renameFile(string currentFileName, string newFileName);
	int evaluateCommands(list<string>* sentence);
	int help();
	int close();

public:


	CommandLineInterface();
	void loopMenu();

	~CommandLineInterface();
};

