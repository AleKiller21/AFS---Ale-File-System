#include "stdafx.h"
#include "ErrorHandler.h"

using namespace std;

#define SUCCESS 0
#define DISK_NOT_OPEN  1
#define DISK_ALREADY_OPEN 2
#define NOT_ENOUGH_BLOCKS 3
#define FILE_ALREADY_EXISTS 4
#define NO_FREE_INODE 5
#define NO_FREE_DIRECTORY_ENTRY 6
#define FILE_SYSTEM_NOT_MOUNTED 7
#define FILE_SYSTEM_ALREADY_MOUNTED 8
#define DISK_NOT_EXIST 9
#define DISK_ALREADY_EXISTS 10
#define DISK_ALREADY_FORMATTED 12
#define FILE_NOT_FOUND 13
#define WRONG_NUMBER_ARGUMENTS 100
#define DISK_NAME_MISSING 101
#define INVALID_SIZE_ARGUMENT 102
#define INVALID_SIZE_UNIT 103
#define INVALID_FILE_NAME 104
#define FILE_NEW_NAME_INVALID 105
#define INVALID_FILE_PATH 106
#define WRONG_COMMAND 200
#define INVALID_COMMAND_STRUCTURE 201

ErrorHandler::ErrorHandler()
{
}

string ErrorHandler::handleError(int errorCode)
{
	switch (errorCode)
	{
	case SUCCESS:
		return "OK!";

	case DISK_NOT_OPEN:
		return "The disk must be open to continue!";

	case DISK_ALREADY_OPEN:
		return "A disk is already open. Close it first!";

	case NOT_ENOUGH_BLOCKS:
		return "There are not enough free blocks to save the file!";

	case FILE_ALREADY_EXISTS:
		return "This file already exists!";

	case NO_FREE_INODE:
		return "There is no inode available to save the file!";

	case NO_FREE_DIRECTORY_ENTRY:
		return "There is no entry available in the root directory to save the file!";

	case FILE_SYSTEM_NOT_MOUNTED:
		return "You must first mount the file system to continue!";

	case FILE_SYSTEM_ALREADY_MOUNTED:
		return "The file system is already mounted!";

	case DISK_NOT_EXIST:
		return "The specified disk does not exist!";

	case DISK_ALREADY_EXISTS:
		return "The specified disk already exists!";

	case DISK_ALREADY_FORMATTED:
		return "The disk already has a file system written on it!";

	case FILE_NOT_FOUND:
		return "No file with such name exists!";

	case WRONG_NUMBER_ARGUMENTS:
		return "The number of arguments in the command is wrong!";

	case DISK_NAME_MISSING:
		return "Name for the new disk is not in the command!";

	case INVALID_SIZE_ARGUMENT:
		return "The size of the disk is invalid!";

	case INVALID_SIZE_UNIT:
		return "The unit used for the size of the new disk is wrong!";

	case INVALID_FILE_NAME:
		return "The name of the file is invalid!";

	case FILE_NEW_NAME_INVALID:
		return "The new name for the file is invalid!";

	case INVALID_FILE_PATH:
		return "The path is invalid!";

	case WRONG_COMMAND:
		return "Wrong command!";

	default:
		return "An error has occurred!";
	}
}

ErrorHandler::~ErrorHandler()
{
}
