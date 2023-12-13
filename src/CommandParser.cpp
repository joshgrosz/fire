#include "CommandParser.h"
#include <vector>

TextAdventureCommand ParseAdventureCommand()
{
	TextAdventureCommand retCommand = {};
	retCommand.Verb = "";

	const unsigned int INPUT_CHAR_LIMIT = 256;
	char inputStr[INPUT_CHAR_LIMIT];

	char* pRet = fgets(inputStr, INPUT_CHAR_LIMIT, stdin);

	std::vector<std::string> tokens;
	char* next_token = nullptr;
	char *token = strtok_s(inputStr, " \t\n", &next_token);
	while (token)
	{
		tokens.push_back(token);
		token = strtok_s(nullptr, " \t\n", &next_token);
	}

	if (tokens.size() >= 1)
	{
		std::string verbString = tokens[0];

		for (unsigned int i = 0; i < verbString.size(); ++i)
		{
			verbString[i] = tolower(verbString[i]);
		}

		retCommand.Verb = verbString;
	}

	return retCommand;
}