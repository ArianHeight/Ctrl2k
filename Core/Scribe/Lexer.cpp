#include <iostream>
#include <vector>
#include "Lexer.h"

using namespace scb;

// --------- TokenPair ------------

TokenPair::TokenPair(ControlsTokenType tt, const char* tv)
	: tokenType(tt), tokenValue(tv) {}

const ControlsTokenType TokenPair::getTokenType() {
	return tokenType;
}

const const char* TokenPair::getTokenValue() {
	return tokenValue;
}

// --------- Lexer ------------

Lexer::Lexer() {}

void Lexer::appendToken(TokenPair tp) {
	return;
}

const char* Lexer::summary() {
	return "useless lexer";
}