#include <Ctrl 2kPCH.h>

/*

for tokens for lexing

*/

/*
Base Class for Token
*/
Token::Token(const std::string &value) : m_value(value) 
{
	//cstr	
}

std::string& Token::getValue()
{
	return this->m_value;
}

const std::string& Token::getValue() const
{
	return this->m_value;
}

TypeOfToken Token::getType() const
{
	return this->m_type;
}

void Token::addNegative()
{
	if (this->m_type == INT_CONSTANT || this->m_type == FLOATING_CONSTANT)
	{
		this->m_value = "-" + this->m_value;
	}
}

Token::~Token() 
{
	//dstr
}

/*
WhiteSpace
*/
WhiteSpaceToken::WhiteSpaceToken(std::string &value) : 
	Token(value)
{
	//cstr
	this->m_type = WHITESPACE;
}

WhiteSpaceToken::~WhiteSpaceToken() 
{
	//dstr	
}

/*
End Of Line
*/
EOLToken::EOLToken() : 
	Token(std::string(""))
{
	//cstr
	this->m_type = EOL;
}

EOLToken::~EOLToken() 
{
	
}

/*
String
*/
StringLiteralToken::StringLiteralToken(std::string &value) : 
	Token(value)
{
	//cstr
	this->m_type = STRING_LITERAL;
}

StringLiteralToken::~StringLiteralToken()
{
	//dstr
}

IntegerToken::IntegerToken(std::string &value) : 
	Token(value)
{
	//cstr
	this->m_type = INT_CONSTANT;
}

IntegerToken::~IntegerToken()
{
	//dstr
}

/*
Decimals
*/
FloatingToken::FloatingToken(std::string &value) : 
	Token(value)
{
	//cstr
	this->m_type = FLOATING_CONSTANT;
}

FloatingToken::~FloatingToken()
{
	//dstr
}

/*
Identifiers
*/
IdentifierToken::IdentifierToken(std::string &value) : 
	Token(value)
{
	//cstr
	this->m_type = IDENTIFIER;
}

IdentifierToken::~IdentifierToken()
{
	//dstr
}

/*
Keywords
*/
KeywordToken::KeywordToken(std::string &value) : 
	Token(value)
{
	//cstr
	this->m_type = KEYWORD;
}

KeywordToken::~KeywordToken()
{
	//dstr
}

/*
Punctuators
*/
PunctuatorToken::PunctuatorToken(std::string &value) : 
Token(value)
{
	//cstr
	this->m_type = PUNCTUATOR;
}

PunctuatorToken::~PunctuatorToken()
{
	//dstr
}

/*
Invalid Tokens
*/
InvalidToken::InvalidToken(std::string &value) : 
Token(value)
{
	//cstr
	this->m_type = INVALID;
}

InvalidToken::~InvalidToken()
{
	//dstr
}

/*

end Token

*/