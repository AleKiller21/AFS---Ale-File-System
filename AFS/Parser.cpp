#include "stdafx.h"
#include "Parser.h"


Parser::Parser()
{
}

list<string>* Parser::parseCommand(string command)
{
	int base = 0;
	list<string>* words = new list<string>();
	command.append(" ");

	for (int i = 0; i < command.size(); i++)
	{
		if (command[i] != ' ') continue;

		words->push_back(command.substr(base, i - base));
		base = i + 1;
	}

	return words;
}

string Parser::extractNameFromPath(string filePath)
{
	int lastBackSlashPosition = -1;
	int fileNameStart = 0;
	string name;

	for (int i = 0; i < filePath.size(); i++)
	{
		if (filePath[i] == '\\') lastBackSlashPosition = i;
	}

	fileNameStart = ++lastBackSlashPosition;
	name = filePath.substr(fileNameStart);

	return name;
}

string Parser::constructPath(list<string>* path)
{
	string constructedPath = "";

	for (list<string>::iterator it = path->begin(); it != path->end(); ++it)
	{
		constructedPath += *it + " ";
	}

	return constructedPath;
}

Parser::~Parser()
{
}
