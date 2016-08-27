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
		words = nullptr;
		cout << "\n>";
		getline(cin, command);
		words = Parser::parseCommand(command);
		evaluateCommands(words);
		words->clear();
		delete words;
	}
}

int CommandLineInterface::mountFileSystem(list<string>* arguments)
{
	list<string>::iterator it = arguments->begin();
	string diskName = *it;
	string fileSystem = *(++it);
	if (fileSystem.compare("afs")) return -1;

	return ui.mountFileSystem(diskName);
}

int CommandLineInterface::openDisk(string diskName)
{
	return ui.openDisk(diskName);
}

int CommandLineInterface::createEmptyFile(string fileName)
{
	return ui.createEmptyFile(fileName);
}

int CommandLineInterface::listFiles()
{
	list<FileInfo>* files =  ui.listFiles();
	cout << "\n";

	for (list<FileInfo>::iterator it = files->begin(); it != files->end(); ++it)
	{
		cout << "Name: " << (*it).name << endl;
		cout << "Inodo: " << (*it).inode << endl;
		cout << "Size on Disk: " << (*it).sizeInDisk << " bytes" << endl;
		cout << "Size on Bytes: " << (*it).sizeInBytes << " bytes" << endl;
		cout << "\n";
	}

	cout << "\n";
	files->clear();
	delete files;
	return 0;
}

int CommandLineInterface::evaluateCommands(list<string>* sentence)
{
	string command = sentence->front();

	if (!command.compare("crtdsk"))
	{
		//CommandValidations::validateCreateDiskCommand();
		sentence->erase(sentence->begin());
		createDisk(sentence);
		return 0;
	}

	if (!command.compare("mount"))
	{
		//CommandValidations::
		sentence->erase(sentence->begin());
		return mountFileSystem(sentence);
	}

	if (!command.compare("open"))
	{
		//CommandValidations::
		sentence->erase(sentence->begin());
		return openDisk(sentence->front());
	}

	if (!command.compare("touch"))
	{
		//CommandValidations::
		sentence->erase(sentence->begin());
		return createEmptyFile(sentence->front());
	}

	if (!command.compare("ls"))
	{
		//CommandValidations::
		return listFiles();
	}

	return -1;
}

CommandLineInterface::~CommandLineInterface()
{
}
