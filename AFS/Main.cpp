// Main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "UI.h"


int _tmain(int argc, _TCHAR* argv[])
{
	UI ui;
	ui.createDisk(1073741824, "Disk.af");

	//char* buffer = new char[20];

	//std::cout << sizeof(buffer) << std::endl;
	//delete[] buffer;
}

