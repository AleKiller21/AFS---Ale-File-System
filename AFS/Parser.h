#pragma once
#include <list>

using namespace std;

class Parser
{
public:
	Parser();
	static list<string>* parseCommand(string command);
	~Parser();
};

