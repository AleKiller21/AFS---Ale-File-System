#pragma once
#include "AFS.h"

class UI
{
private:
	AFS fileSystem;

	char* setPartition(unsigned int size) const;
	bool checkDiskExists(std::string diskName);

public:

	UI();
	int createDisk(unsigned int size, std::string diskName);
	int formatDisk(std::string diskName);
	int mountFileSystem();
	int unmountFileSystem();
	int openDisk(std::string diskName);
	int closeDisk();
	int importFile(std::string fileName, std::string name);
	int createEmptyFile(std::string fileName);
	std::list<FileInfo>* listFiles() const;
	std::list<unsigned int>* getFileSystemInfo() const;

	~UI();
};

