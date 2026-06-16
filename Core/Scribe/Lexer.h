#pragma once
#include <string>
#include <vector>

namespace scb
{
	enum class ConfigTokenType
	{
		KEY,
		VALUE
	};

	struct Token
	{
		std::string value;
		ConfigTokenType type;
	};

	class Lexer
	{
	public:
		Lexer();
		void loadFile(std::string);
		std::vector<Token> tokenList;

	protected:
		void tokenize(std::string);
	};

}