#include "stdafx.h"
#include "UI.h"

using namespace std;

UI::UI()
{
}

void UI::createDisk(streamsize size, string diskName)
{
	char* buffer;
	ofstream out(diskName.c_str(), ios::binary);

	buffer = setPartition(size);
	out.write(buffer, size);
	out.close();

	delete[] buffer;
	buffer = nullptr;

	fileSystem.mountNewFileSystem(diskName, 'A', size);
}

int UI::openDisk(std::string diskName)
{
	return fileSystem.openDisk(diskName);
}

int UI::importFile(std::string fileName, string name)
{
	return fileSystem.importFile(fileName, name); 
}

int UI::createEmptyFile(std::string fileName)
{
	return fileSystem.createEmptyFile(fileName);
}

char* UI::setPartition(streamsize size)
{
	char* buffer = new char[size];
	for (streamsize i = 0; i < size; i++)
	{
		buffer[i] = '\0';
	}

	return buffer;
}

UI::~UI()
{
}
