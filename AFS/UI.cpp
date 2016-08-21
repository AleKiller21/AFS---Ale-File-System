#include "stdafx.h"
#include "UI.h"

using namespace std;

UI::UI()
{
}

void UI::createDisk(unsigned int size, string diskName)
{
	char* buffer;
	ofstream out(diskName.c_str(), ios::binary);

	buffer = setPartition(size);
	out.write(buffer, size);
	out.close();
	fileSystem.mountFileSystem(diskName, 'A', size);

	delete[] buffer;
	buffer = nullptr;
}

char* UI::setPartition(int size)
{
	char* buffer = new char[size];
	for (int i = 0; i < size; i++)
	{
		buffer[i] = '\0';
	}

	return buffer;
}

UI::~UI()
{
}
