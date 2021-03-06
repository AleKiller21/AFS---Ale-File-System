#pragma once
#include "UI.h"
#include "CommandValidations.h"
#include <string>


using namespace std;

class CommandLineInterface
{
	UI ui;
	unsigned int size;
	list<string> commands;

	int createDisk(list<string>* arguments);
	int mountFileSystem();
	int unmountFileSystem();
	int openDisk(string diskName);
	int createEmptyFile(list<string>* sentence);
	int listFiles() const;
	int showFileSystemInfo() const;
	int renameFile(list<string>* path);
	int importFile(list<string>* path);
	int exportFile(list<string>* path);
	int deleteFile(list<string>* path);
	int deleteDisk(list<string>* arguments);
	int evaluateCommands(list<string>* sentence);
	int help();
	int close();

public:


	CommandLineInterface();
	void loopMenu();

	~CommandLineInterface();
};

