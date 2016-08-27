#include "stdafx.h"
#include "ErrorHandler.h"

using namespace std;

#define SUCCESS 0
#define WRONG_NUMBER_ARGUMENTS 100
#define DISK_NAME_MISSING 101
#define INVALID_SIZE_ARGUMENT 102
#define INVALID_SIZE_UNIT 103
#define WRONG_COMMAND 200

ErrorHandler::ErrorHandler()
{
}

string ErrorHandler::handleError(int errorCode)
{
	switch (errorCode)
	{
	case SUCCESS:
		return "OK!";

	case WRONG_NUMBER_ARGUMENTS:
		return "The number of arguments in the command is wrong!";

	case DISK_NAME_MISSING:
		return "Name for the new disk is not in the command!";

	case INVALID_SIZE_ARGUMENT:
		return "The size of the disk is invalid!";

	case INVALID_SIZE_UNIT:
		return "The unit used for the size of the new disk is wrong!";

	case WRONG_COMMAND:
		return "Wrong command!";
	}
}

ErrorHandler::~ErrorHandler()
{
}
