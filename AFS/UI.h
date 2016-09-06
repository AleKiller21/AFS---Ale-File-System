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
	int mountFileSystem();
	int unmountFileSystem();
	int openDisk(std::string diskName);
	int closeDisk();
	int importFile(std::list<std::string>* path);
	int exportFile(std::list<std::string>* path);
	int deleteFile(std::list<std::string>* path);
	int createEmptyFile(std::list<std::string>* sentence);
	int renameFile(std::string fileCurrentName, std::string fileNewName);
	std::list<FileInfo>* listFiles() const;
	std::list<unsigned int>* getFileSystemInfo() const;

	~UI();
};

