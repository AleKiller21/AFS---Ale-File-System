#pragma once
#include <list>

using namespace std;

class CommandValidations
{
public:
	CommandValidations();
	static bool validateCreateDiskCommand(list<string>* command);

	~CommandValidations();
};

