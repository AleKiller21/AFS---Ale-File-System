#include "stdafx.h"
#include "CommandValidations.h"


CommandValidations::CommandValidations()
{
}

bool CommandValidations::validateCreateDiskCommand(list<string>* command)
{
	list<string>::iterator it = ++command->begin();
	return true;
}

CommandValidations::~CommandValidations()
{
}
