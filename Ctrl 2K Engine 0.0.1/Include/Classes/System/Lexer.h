#pragma once

/*

The Lexer Class
used to split strings into tokens

*/

class Lexer //used by parser only, so is pre-declared with parser
{
public:
	Lexer();
	~Lexer();

private:
	//no idea what to put here
	TypeOfToken m_currentToken;
	std::vector<int> m_tokenIndex; //indices for the tokens in the string *might not use
	std::string m_currentSemiToken; //a token only seperated by whitespace and \"
	bool isString;
	bool isComment;
	bool mulitLineComment;

	//single check
	std::vector<char> whiteSpace = { ' ' };
	std::vector<char> stringLiteral = { '\"' };
	std::vector<char> intConstant = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	std::vector<char> floatConstant = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.' };

	//private method
	void approximateTokenType(char c);

	bool isPunctuator(char c);
	bool isAlphabet(char c);
	bool isNumber(char c);
	bool isPunctuator(char c, char cPeek); //runs through 2-character operators
	bool isWhiteSpace(const std::string &str); //check if whole string is whitespace
	bool isFloat(const std::string &str);
	bool isInt(const std::string &str);
	bool isSymbol(const std::string &str);

	void lexIntSubstr(std::vector<Token> &output); //reads through a semi token and judges if its an int or floating point
	void lexIdSubstr(std::vector<Token> &output); //reads through a semi token and judges if its an valid identifier or not
	void tokenize(std::vector<Token> &output); //tokenizes the string

public:
	void test(std::string str);
	void lexFile(const std::string &filePath, std::vector<Token> &output, bool ignoreEOL = true, bool ignoreWhiteSpace = true); //takes string input, lexes it and adds to the output vector lexed tokens
};

/*

end Lexer

*/