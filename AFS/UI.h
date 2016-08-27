#pragma once
#include "AFS.h"

class UI
{
private:
	AFS fileSystem;

	char* setPartition(unsigned int size);

public:

	UI();
	void createDisk(unsigned int size, std::string diskName);
	int mountFileSystem(std::string diskName);
	int openDisk(std::string diskName);
	int importFile(std::string fileName, std::string name);
	int createEmptyFile(std::string fileName);

	~UI();
};

