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

CommandLineInterface::~CommandLineInterface()
{
}
