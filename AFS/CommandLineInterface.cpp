#include "stdafx.h"
#include "Parser.h"
#include "ErrorHandler.h"
#include <stdlib.h>
#include "CommandLineInterface.h"


CommandLineInterface::CommandLineInterface()
{
	commands.push_back("crtdsk <DISK_NAME> <SIZE> <MB/GB> -- Creates a new disk with a primary partition that will have afs mounted.");
	commands.push_back("rename <FILE_NAME> <NEW_NAME> -- Renames the specified file with the new name in the second argument.");
	commands.push_back("open <DISK_NAME> -- open the disk with the specified name.");
	commands.push_back("touch <FILE_NAME> -- Creates an empty file with the specified name.");
	commands.push_back("import <FILE_PATH> -- Will import the file, specified in the path, into AFS.");
	commands.push_back("export <FILE_NAME> -- Exports the specified file.");
	commands.push_back("mount -- Loads afs structures to memory.");
	commands.push_back("unmount -- Removes afs structures from memory. You won't be able to do anything until you mount them back.");
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

int CommandLineInterface::listFiles() const
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
		printf_s("Date Created: %s", (*it).DateCreated); cout << "\n";
		cout << "\n";
	}

	files->clear();
	delete files;
	return 0;
}

int CommandLineInterface::showFileSystemInfo() const
{
	list<unsigned int>* info = ui.getFileSystemInfo();
	if (!info) return 7;

	cout << "\n";

	list<unsigned int>::iterator it = info->begin();
	cout << "Partition Size : " << *it << " bytes" << endl;
	cout << "Total blocks : " << *(++it) << endl;
	cout << "Free blocks : " << *(++it) << endl;
	cout << "Used blocks : " << *(++it) << endl;
	cout << "Block size : " << *(++it) << " bytes" << endl;
	cout << "Data bytes available per block : " << *(++it) << " bytes" << endl;
	cout << "Bitmap size : " << *(++it) << " bytes" << endl;
	cout << "Bitmap block : " << *(++it) << endl;
	cout << "Words in bitmap : " << *(++it) << endl;
	cout << "Directory size : " << *(++it) << " bytes" << endl;
	cout << "Directory block : " << *(++it) << endl;
	cout << "Total inodes : " << *(++it) << endl;
	cout << "Free inodes : " << *(++it) << endl;
	cout << "InodeTable size : " << *(++it) << " bytes" << endl;
	cout << "InodeTable block : " << *(++it) << endl;
	cout << "First data block : " << *(++it) << endl;
	cout << "sizeof Superblock : " << *(++it) << " bytes" << endl;
	cout << "sizeof DirectoryEntry : " << *(++it) << " bytes" << endl;
	cout << "sizeof Inode : " << *(++it) << " bytes" << endl;

	info->clear();
	delete info;
	return 0;
}

int CommandLineInterface::renameFile(string currentFileName, string newFileName)
{
	return ui.renameFile(currentFileName, newFileName);
}

int CommandLineInterface::importFile(list<string>* path)
{
	return ui.importFile(path);
}

int CommandLineInterface::exportFile(list<string>* path)
{
	return ui.exportFile(path);
}

int CommandLineInterface::deleteFile(list<string>* path)
{
	cout << "Are you sure you want to delete this file? (Y)/N ";
	//char response;
	//cin >> response;
	string response;
	getline(cin, response);
	if (response == "Y" || response == "y")
		return ui.deleteFile(path);

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

	if (!command.compare("rename"))
	{
		sentence->erase(sentence->begin());
		list<string>::iterator it = sentence->begin();
		error = CommandValidations::validateRenameCommand(sentence);
		if (error != 0) return error;
		string currentName = *it;
		return renameFile(currentName, *(++it));
	}

	if (!command.compare("import"))
	{
		sentence->erase(sentence->begin());
		return importFile(sentence);
	}

	if (!command.compare("export"))
	{
		sentence->erase(sentence->begin());
		return exportFile(sentence);
	}

	if (!command.compare("delete"))
	{
		sentence->erase(sentence->begin());
		return deleteFile(sentence);
	}

	return 200;
}

CommandLineInterface::~CommandLineInterface()
{
}
