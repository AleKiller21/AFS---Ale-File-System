#include "stdafx.h"
#include "CommandValidations.h"
#include <string>

#define SUCCESS 0
#define WRONG_NUMBER_ARGUMENTS 100
#define DISK_NAME_MISSING 101
#define INVALID_SIZE_ARGUMENT 102
#define INVALID_SIZE_UNIT 103
#define INVALID_FILE_NAME 104


CommandValidations::CommandValidations()
{
}

int CommandValidations::validateCreateDiskCommand(list<string>* arguments)
{
	list<string>::iterator it = arguments->begin();
	unsigned int size = 0;
	int sizeWithSigned = 0;
	
	if (arguments->size() != 3) return WRONG_NUMBER_ARGUMENTS;
	if (!(*it).compare("") || !(*it).compare(" ")) return DISK_NAME_MISSING;

	try
	{
		size = stoul(*(++it), nullptr);
		sizeWithSigned = stoul(*it, nullptr);
	}
	catch (...)
	{
		return INVALID_SIZE_ARGUMENT;
	}

	if (size == 0 || sizeWithSigned < 0 || size > 4294967295) return INVALID_SIZE_ARGUMENT;

	++it;
	if ((*it).compare("MB") && (*it).compare("GB")) return INVALID_SIZE_UNIT;

	return SUCCESS;
}

int CommandValidations::validateOpenDiskCommand(list<string>* arguments)
{
	list<string>::iterator it = arguments->begin();

	if (arguments->size() != 1) return WRONG_NUMBER_ARGUMENTS;
	if (!(*it).compare("") || !(*it).compare(" ")) return DISK_NAME_MISSING;

	return SUCCESS;
}

int CommandValidations::validateTouchCommand(list<string>* arguments)
{
	list<string>::iterator it = arguments->begin();

	if (arguments->size() != 1) return WRONG_NUMBER_ARGUMENTS;
	if (!(*it).compare("") || !(*it).compare(" ")) return INVALID_FILE_NAME;

	return SUCCESS;
}

int CommandValidations::validateCommandsWithoutArguments(list<string>* arguments)
{
	list<string>::iterator it = arguments->begin();

	if (arguments->size() != 0) return WRONG_NUMBER_ARGUMENTS;

	return SUCCESS;
}

CommandValidations::~CommandValidations()
{
}
