#pragma once
#include <string>
#include <vector>

namespace scb
{
	enum class ControlsTokenType {
		TEST,
		MOVE_FORWARD,
		MOVE_BACKWARD,
		MOVE_LEFT,
		MOVE_RIGHT
	};

	class TokenPair {
	public:
		TokenPair(ControlsTokenType, const char*);
		const ControlsTokenType getTokenType();
		const char* getTokenValue();

	protected:
		ControlsTokenType tokenType;
		const char* tokenValue;
	};

	class Lexer {
	public:
		Lexer();
		void loadFile(const char*);
		void appendToken(TokenPair);
		const std::string summary(); // brief information about the lexer instance
		std::vector<std::string> raw; // unprocessed strings from a file

	protected:
		std::vector<int> tokenList;
	};

}