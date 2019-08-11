#pragma once

/*

Tokens for the Lexer and Parser

*/

class Token
{
public:
	Token(std::string &value);
	~Token();

	std::string &getValue();
	TypeOfToken getType();
	void addNegative();

protected:
	std::string m_value;
	TypeOfToken m_type{ WHITESPACE };
};

class WhiteSpaceToken : public Token
{
public:
	WhiteSpaceToken(std::string &value);
	~WhiteSpaceToken();
};

class EOLToken : public Token
{
public:
	EOLToken();
	~EOLToken();
};

class StringLiteralToken : public Token
{
public:
	StringLiteralToken(std::string &value);
	~StringLiteralToken();
};

class IntegerToken : public Token
{
public:
	IntegerToken(std::string &value);
	~IntegerToken();
};

class FloatingToken : public Token
{
public:
	FloatingToken(std::string &value);
	~FloatingToken();
};

class IdentifierToken : public Token
{
public:
	IdentifierToken(std::string &value);
	~IdentifierToken();
};

class KeywordToken : public Token
{
public:
	KeywordToken(std::string &value);
	~KeywordToken();
};

class PunctuatorToken : public Token
{
public:
	PunctuatorToken(std::string &value);
	~PunctuatorToken();
};

class InvalidToken : public Token
{
public:
	InvalidToken(std::string &value);
	~InvalidToken();
};

/*

end Token Defs

*/