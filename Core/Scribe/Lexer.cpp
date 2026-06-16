#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include "Lexer.h"

using namespace scb;

// --------- Lexer ------------

Lexer::Lexer() {}

//void Lexer::appendToken(TokenPair tp) {
//	return;
//}

//const std::string Lexer::summary() {
//	return std::to_string(tokenList.size());
//}

void Lexer::tokenize(std::string line)
{

	std::regex r = std::regex("(.*)( = )(.*)");

	// Store match results to work out subexpressions
	std::smatch some_match;

	// Locations of token and value from the match result array
	unsigned short token_location = 1;
	unsigned short value_location = 3;

	Token tempToken;

	if (regex_match(line, some_match, r))
	{
		tempToken.value = some_match[1];
		tempToken.type = ConfigTokenType::KEY;
		tokenList.push_back(tempToken);

		tempToken.value = some_match[3];
		tempToken.type = ConfigTokenType::VALUE;
		tokenList.push_back(tempToken);
	}

}

void Lexer::loadFile(std::string path)
{
	std::fstream myFile;
	std::string line;

	myFile.open(path);
	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			tokenize(line);
		}
		myFile.close();
	}

}