#pragma once


using namespace std;

class Parser
{
public:
	Parser();
	static list<string>* parseCommand(string command);
	static string extractNameFromPath(string filePath);
	static string constructPath(list<string>* path);
	~Parser();
};

