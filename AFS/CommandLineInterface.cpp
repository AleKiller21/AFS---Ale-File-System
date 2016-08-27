#include "stdafx.h"
#include "CommandLineInterface.h"


CommandLineInterface::CommandLineInterface()
{
}

void CommandLineInterface::createDisk(list<string>* arguments)
{
	list<string>::iterator it = arguments->begin();
	string diskName = *it;
	unsigned int size = stoul(*(++it), nullptr);
	string unit = *(++it);

	if (!unit.compare("MB")) size *= 1024 * 1024;
	else if (!unit.compare("GB")) size *= 1024 * 1024 * 1024;

	ui.createDisk(size, diskName);
}

void CommandLineInterface::loopMenu()
{
	string command = "";
	list<string>* words;

	while (command.compare("exit"))
	{
		cout << ">";
		getline(cin, command);
		words = Parser::parseCommand(command);
		evaluateCommands(words);
	}
}

int CommandLineInterface::mountFileSystem(string diskName, char partition)
{
	return ui.mountFileSystem(diskName, partition, size);
}

int CommandLineInterface::openDisk(string diskName)
{
	return ui.openDisk(diskName);
}

int CommandLineInterface::createEmptyFile(string fileName)
{
	return ui.createEmptyFile(fileName);
}

void CommandLineInterface::evaluateCommands(list<string>* sentence)
{
	string command = sentence->front();

	if (!command.compare("crtdsk"))
	{
		//CommandValidations::validateCreateDiskCommand();
		sentence->erase(sentence->begin());
		createDisk(sentence);
	}
}

CommandLineInterface::~CommandLineInterface()
{
}
