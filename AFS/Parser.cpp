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

Parser::~Parser()
{
}
