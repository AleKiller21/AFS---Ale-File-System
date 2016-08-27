#include "stdafx.h"
#include "CommandValidations.h"
#include <string>

#define SUCCESS 0
#define WRONG_NUMBER_ARGUMENTS 100
#define DISK_NAME_MISSING 101
#define INVALID_SIZE_ARGUMENT 102
#define INVALID_SIZE_UNIT 103


CommandValidations::CommandValidations()
{
}

int CommandValidations::validateCreateDiskCommand(list<string>* arguments)
{
	list<string>::iterator it = arguments->begin();
	unsigned int size = 0;
	
	if (arguments->size() != 3) return WRONG_NUMBER_ARGUMENTS;
	if (!(*it).compare("") || !(*it).compare(" ")) return DISK_NAME_MISSING;

	try
	{
		size = stoul(*(++it), nullptr);
	}
	catch (...)
	{
		return INVALID_SIZE_ARGUMENT;
	}

	++it;
	if ((*it).compare("MB") && (*it).compare("GB")) return INVALID_SIZE_UNIT;

	return SUCCESS;
}

int CommandValidations::validateMountCommand(list<string>* arguments)
{
	list<string>::iterator it = arguments->begin();

	return SUCCESS;
}

int CommandValidations::validateOpenDiskCommand(list<string>* arguments)
{
	list<string>::iterator it = arguments->begin();

	return SUCCESS;
}

int CommandValidations::validateTouchCommand(list<string>* arguments)
{
	list<string>::iterator it = arguments->begin();

	return SUCCESS;
}

int CommandValidations::validateListCommand(list<string>* arguments)
{
	list<string>::iterator it = arguments->begin();

	return SUCCESS;
}

CommandValidations::~CommandValidations()
{
}
