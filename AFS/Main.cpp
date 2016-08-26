// Main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include "UI.h"
#include "CommandLineInterface.h"


int _tmain(int argc, _TCHAR* argv[])
{
	//UI ui;
	//ui.createDisk(136314880, "Disk.af");
	////ui.openDisk("Disk.af");
	//////ui.importFile("C:\\Users\\alefe\\Unitec\\Trimestres_Por_Cursar.txt", "Cursar.txt");
	////ui.createEmptyFile("Destino.txt");

	CommandLineInterface cli;
	cli.loopMenu();
}

