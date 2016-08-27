#include "stdafx.h"
#include "CommandLineInterface.h"


CommandLineInterface::CommandLineInterface()
{
	commands.push_back("crtdsk <DISK_NAME> <SIZE> <MB/GB> -- Creates a new disk with a primary partition that will have afs mounted.");
	commands.push_back("open <DISK_NAME> -- open the disk with the specified name.");
	commands.push_back("mount -- Loads afs structures to memory.");
	commands.push_back("unmount -- Removes afs structures from memory. You won't be able to do anything until you mount them back.");
	commands.push_back("touch <FILE_NAME> -- Creates an empty file with the specified name.");
	commands.push_back("ls -- Lists all the existing files and their corresponding info.");
	commands.push_back("help -- Shows all the commands available, the arguments they require, and a brief description of each of them.");
}

int CommandLineInterface::createDisk(list<string>* arguments)
{
	list<string>::iterator it = arguments->begin();
	string diskName = *it;
	unsigned int size = stoul(*(++it), nullptr);
	string unit = *(++it);

	if (!unit.compare("MB")) size *= 1024 * 1024;
	else if (!unit.compare("GB")) size *= 1024 * 1024 * 1024;

	return ui.createDisk(size, diskName);
}

void CommandLineInterface::loopMenu()
{
	string command = "";
	list<string>* words;
	int error;

	while (command.compare("exit"))
	{
		words = nullptr;
		cout << "\nAleShell>";
		getline(cin, command);
		words = Parser::parseCommand(command);
		error = evaluateCommands(words);
		cout << ErrorHandler::handleError(error) << endl;
		words->clear();
		delete words;
	}
}

int CommandLineInterface::mountFileSystem()
{
	return ui.mountFileSystem();
}

int CommandLineInterface::unmountFileSystem()
{
	return ui.unmountFileSystem();
}

int CommandLineInterface::openDisk(string diskName)
{
	return ui.openDisk(diskName);
}

int CommandLineInterface::createEmptyFile(string fileName)
{
	return ui.createEmptyFile(fileName);
}

int CommandLineInterface::listFiles()
{
	list<FileInfo>* files =  ui.listFiles();
	if (!files) return 7;

	cout << "\n";

	for (list<FileInfo>::iterator it = files->begin(); it != files->end(); ++it)
	{
		cout << "Name: " << (*it).name << endl;
		cout << "Inodo: " << (*it).inode << endl;
		cout << "Size on Disk: " << (*it).sizeInDisk << " bytes" << endl;
		cout << "Size on Bytes: " << (*it).sizeInBytes << " bytes" << endl;
		cout << "\n";
	}

	files->clear();
	delete files;
	return 0;
}

int CommandLineInterface::showFileSystemInfo()
{
	list<unsigned int>* info = ui.getFileSystemInfo();
	if (!info) return 7;

	cout << "\n";

	list<unsigned int>::iterator it = info->begin();
	cout << "Partition Size : " << *(++it) << endl;
	cout << "Total blocks : " << *it << endl;
	cout << "Free blocks : " << *(++it) << endl;
	cout << "Used blocks : " << *(++it) << endl;
	cout << "Block size : " << *(++it) << endl;
	cout << "Bitmap size : " << *(++it) << endl;
	cout << "Bitmap block : " << *(++it) << endl;
	cout << "Words in bitmap : " << *(++it) << endl;
	cout << "Directory size : " << *(++it) << endl;
	cout << "Directory block : " << *(++it) << endl;
	cout << "Total inodes : " << *(++it) << endl;
	cout << "Free inodes : " << *(++it) << endl;
	cout << "InodeTable size : " << *(++it) << endl;
	cout << "InodeTable block : " << *(++it) << endl;
	cout << "First data block : " << *(++it) << endl;

	info->clear();
	delete info;
	return 0;
}

int CommandLineInterface::help()
{
	for (list<string>::iterator it = commands.begin(); it != commands.end(); ++it)
	{
		cout << "\n" << *it << endl;
	}

	return 0;
}

int CommandLineInterface::close()
{
	return ui.closeDisk();
}

int CommandLineInterface::evaluateCommands(list<string>* sentence)
{
	string command = sentence->front();
	int error;

	if (!command.compare("exit"))
		return 0;

	if (!command.compare("crtdsk"))
	{
		sentence->erase(sentence->begin());
		error = CommandValidations::validateCreateDiskCommand(sentence);
		if (error != 0) return error;
		return createDisk(sentence);
	}

	if (!command.compare("open"))
	{
		sentence->erase(sentence->begin());
		error = CommandValidations::validateOpenDiskCommand(sentence);
		if (error != 0) return error;
		return openDisk(sentence->front());
	}

	if (!command.compare("mount"))
	{
		sentence->erase(sentence->begin());
		error = CommandValidations::validateCommandsWithoutArguments(sentence);
		if (error != 0) return error;
		return mountFileSystem();
	}

	if (!command.compare("touch"))
	{
		sentence->erase(sentence->begin());
		error = CommandValidations::validateTouchCommand(sentence);
		if (error != 0) return error;
		return createEmptyFile(sentence->front());
	}

	if (!command.compare("ls"))
	{
		sentence->erase(sentence->begin());
		error = CommandValidations::validateCommandsWithoutArguments(sentence);
		if (error != 0) return error;
		return listFiles();
	}

	if (!command.compare("unmount"))
	{
		sentence->erase(sentence->begin());
		error = CommandValidations::validateCommandsWithoutArguments(sentence);
		if (error != 0) return error;
		return unmountFileSystem();
	}

	if (!command.compare("help"))
	{
		sentence->erase(sentence->begin());
		error = CommandValidations::validateCommandsWithoutArguments(sentence);
		if (error != 0) return error;
		return help();
	}

	if (!command.compare("cls"))
	{
		sentence->erase(sentence->begin());
		error = CommandValidations::validateCommandsWithoutArguments(sentence);
		if (error != 0) return error;
		system("cls");
		return 0;
	}

	if (!command.compare("close"))
	{
		sentence->erase(sentence->begin());
		error = CommandValidations::validateCommandsWithoutArguments(sentence);
		if (error != 0) return error;
		return close();
	}

	if (!command.compare("afs"))
	{
		sentence->erase(sentence->begin());
		error = CommandValidations::validateCommandsWithoutArguments(sentence);
		if (error != 0) return error;
		return showFileSystemInfo();
	}

	return 200;
}

CommandLineInterface::~CommandLineInterface()
{
}
