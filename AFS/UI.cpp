#include "stdafx.h"
#include "UI.h"

#define PAGE_SIZE 536870912

using namespace std;

UI::UI()
{
}

int UI::createDisk(unsigned int size, string diskName)
{
	if (checkDiskExists(diskName)) return 10;
	ofstream out(diskName.c_str(), ios::binary);

	int iterations = size / PAGE_SIZE;

	for (int i = 0; i < iterations; i++)
	{
		char* buffer = new char[PAGE_SIZE];
		for (unsigned int x = 0; x < PAGE_SIZE; x++)
		{
			buffer[x] = '\0';
		}

		out.write(buffer, PAGE_SIZE);
		size -= PAGE_SIZE;
		delete[] buffer;
		buffer = nullptr;
	}

	if (size > 0)
	{
		char* buffer = new char[size];
		for (unsigned int i = 0; i < size; i++)
		{
			buffer[i] = '\0';
		}

		out.write(buffer, size);
		delete[] buffer;
		buffer = nullptr;
	}

	out.close();

	return fileSystem.writeFileSystemStructuresToDisk(diskName);
}

int UI::mountFileSystem()
{
	return fileSystem.mountFileSystem();
}

int UI::unmountFileSystem()
{
	return fileSystem.unmountFileSystem();
}

int UI::openDisk(std::string diskName)
{
	if (!checkDiskExists(diskName)) return 9;

	return fileSystem.openDisk(diskName);
}

int UI::closeDisk()
{
	return fileSystem.closeDisk();
}

int UI::importFile(list<string>* path)
{
	return fileSystem.importFile(path); 
}

int UI::exportFile(list<string>* path)
{
	return fileSystem.exportFile(path);
}

int UI::deleteFile(list<string>* path)
{
	return fileSystem.deleteFile(path);
}

int UI::createEmptyFile(list<string>* sentence)
{
	return fileSystem.createEmptyFile(sentence);
}

int UI::renameFile(list<string>* path)
{
	return fileSystem.renameFile(path);
}

int UI::deleteDisk(std::string diskName)
{
	return fileSystem.deleteDisk(diskName);
}

bool UI::checkDiskExists(std::string diskName)
{
	ifstream disk(diskName.c_str());
	if (disk)
	{
		disk.close();
		return true;
	}

	disk.close();
	return false;
}

std::list<FileInfo>* UI::listFiles() const
{
	return fileSystem.listFiles();
}

std::list<unsigned>* UI::getFileSystemInfo() const
{
	return fileSystem.getFileSystemInfo();
}

UI::~UI()
{
}
