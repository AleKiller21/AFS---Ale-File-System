#pragma once
#include <fstream>

class UI
{
private:
	char* setPartition(int size);


public:

	UI();
	void createDisk(int size, std::string name);

	~UI();
};

