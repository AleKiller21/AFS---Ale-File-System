#include "stdafx.h"
#include "UI.h"

using namespace std;

UI::UI()
{
}

int UI::createDisk(unsigned int size, string diskName)
{
	if (checkDiskExists(diskName)) return 10;

	char* buffer;
	ofstream out(diskName.c_str(), ios::binary);

	buffer = setPartition(size);
	out.write(buffer, size);
	out.close();

	delete[] buffer;
	buffer = nullptr;

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

char* UI::setPartition(unsigned int size) const
{
	char* buffer = new char[size];
	for (unsigned int i = 0; i < size; i++)
	{
		buffer[i] = '\0';
	}

	return buffer;
}

UI::~UI()
{
}
