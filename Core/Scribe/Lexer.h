#pragma once
#include <vector>

namespace scb
{
	enum class ControlsTokenType {
		MOVE_FORWARD,
		MOVE_BACKWARD,
		MOVE_LEFT,
		MOVE_RIGHT
	};

	class TokenPair {
	public:
		TokenPair(ControlsTokenType, const char*);
		const ControlsTokenType getTokenType();
		const const char* getTokenValue(); // is "const const char*" funny?

	protected:
		ControlsTokenType tokenType;
		const char* tokenValue;
	};

	class Lexer {
	public:
		Lexer();
		void appendToken(TokenPair);
		const char* summary();

	protected:
		std::vector<int> tokenList;
	};

}