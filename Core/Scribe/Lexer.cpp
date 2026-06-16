#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include "Lexer.h"

using namespace scb;

// --------- TokenPair ------------

TokenPair::TokenPair(ControlsTokenType tt, std::string tv)
	: tokenType(tt), tokenValue(tv) {}

const ControlsTokenType TokenPair::getTokenType() {
	return tokenType;
}

std::string TokenPair::getTokenValue() {
	return tokenValue;
}

// --------- Lexer ------------

Lexer::Lexer() {}

//void Lexer::appendToken(TokenPair tp) {
//	return;
//}

//const std::string Lexer::summary() {
//	return std::to_string(tokenList.size());
//}

TokenPair Lexer::tokenize(std::string line) {

	// A list of regular expressions to compare
	std::vector<std::regex> r = { std::regex("(move_forward = )(\")(.*)(\";)"),
								std::regex("(move_backward = )(\")(.*)(\";)"),
								std::regex("(move_left = )(\")(.*)(\";)"),
								std::regex("(move_right = )(\")(.*)(\";)")};

	// Store match results to work out subexpressions
	std::smatch some_match;

	// Locations of token and value from the match result array
	unsigned short token_location = 1;
	unsigned short value_location = 3;

	for (const auto& i : r) {
		if (regex_match(line, some_match, i)) {
			if (some_match[token_location].str() == "move_forward = ") {
				return TokenPair(ControlsTokenType::MOVE_FORWARD, some_match[value_location].str());
			}
			else if (some_match[token_location].str() == "move_backward = ") {
				return TokenPair(ControlsTokenType::MOVE_BACKWARD, some_match[value_location].str());
			}
			else if (some_match[token_location].str() == "move_left = ") {
				return TokenPair(ControlsTokenType::MOVE_LEFT, some_match[value_location].str());
			}
			else if (some_match[token_location].str() == "move_right = ") {
				return TokenPair(ControlsTokenType::MOVE_RIGHT, some_match[value_location].str());
			}
			else {
				return TokenPair(ControlsTokenType::TEST, "Unknown token value");
			}
		}
	}
}

void Lexer::loadFile(std::string path) {
	std::fstream myFile;
	std::string line;

	myFile.open(path);
	if (myFile.is_open()) {
		while (getline(myFile, line)) {
			tokenList.push_back(tokenize(line));
		}
		myFile.close();
	}

}