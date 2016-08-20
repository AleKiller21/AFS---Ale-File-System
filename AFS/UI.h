#pragma once
#include "AFS.h"

class UI
{
private:
	AFS fileSystem;

	char* setPartition(int size);

public:

	UI();
	void createDisk(unsigned int size, std::string diskName);

	~UI();
};

