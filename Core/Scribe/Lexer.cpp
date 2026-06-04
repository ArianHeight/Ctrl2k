#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Lexer.h"

using namespace scb;
using namespace std;

// --------- TokenPair ------------

TokenPair::TokenPair(ControlsTokenType tt, const char* tv)
	: tokenType(tt), tokenValue(tv) {}

const ControlsTokenType TokenPair::getTokenType() {
	return tokenType;
}

const char* TokenPair::getTokenValue() {
	return tokenValue;
}

// --------- Lexer ------------

Lexer::Lexer() {}

void Lexer::appendToken(TokenPair tp) {
	return;
}

const string Lexer::summary() {
	return to_string(raw.size());
}

void Lexer::loadFile(const char* path) {
	fstream myFile;

	myFile.open(path);
	string line;

	// If file didn't load then raw vector doesn't get updated
	if (myFile.is_open()) {
		while (getline(myFile, line)) {
			raw.push_back(line);
		}
		myFile.close();
	}

}