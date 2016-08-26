#pragma once
#include "AFS.h"

class UI
{
private:
	AFS fileSystem;

	char* setPartition(std::streamsize size);

public:

	UI();
	void createDisk(std::streamsize size, std::string diskName);
	int openDisk(std::string diskName);
	int importFile(std::string fileName, std::string name);
	int createEmptyFile(std::string fileName);

	~UI();
};

