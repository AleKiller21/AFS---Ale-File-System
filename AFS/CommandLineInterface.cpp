#include "stdafx.h"
#include "CommandLineInterface.h"


CommandLineInterface::CommandLineInterface()
{
}

void CommandLineInterface::createDisk(streamsize size, string diskName)
{
	ui.createDisk(size, diskName);
	this->size = size;
}

void CommandLineInterface::loopMenu()
{
	string command = "";
	list<string>* words;

	while (command.compare("exit"))
	{
		cout << ">\n";
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
		
	}
}

CommandLineInterface::~CommandLineInterface()
{
}
