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
		TokenPair(ControlsTokenType, std::string);
		const ControlsTokenType getTokenType();
		std::string getTokenValue();

	protected:
		ControlsTokenType tokenType;
		std::string tokenValue;
	};

	class Lexer {
	public:
		Lexer();
		void loadFile(std::string);
		//void appendToken(TokenPair);
		//const std::string summary(); // brief information about the lexer instance
		//std::vector<std::string> raw; // unprocessed strings from a file
		std::vector<TokenPair> tokenList;

	protected:
		TokenPair tokenize(std::string);
	};

}