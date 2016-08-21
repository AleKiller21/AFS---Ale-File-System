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

	~UI();
};

