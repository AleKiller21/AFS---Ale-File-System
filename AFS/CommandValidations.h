#pragma once
#include <list>

using namespace std;

class CommandValidations
{
public:
	CommandValidations();
	static int validateCreateDiskCommand(list<string>* arguments);
	static int validateOpenDiskCommand(list<string>* arguments);
	static int validateTouchCommand(list<string>* arguments);
	static int validateCommandsWithoutArguments(list<string>* arguments);
		
	~CommandValidations();
};

