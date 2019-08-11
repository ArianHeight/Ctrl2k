#include <Ctrl 2kPCH.h>

/*

code for the Lexer

*/

Lexer::Lexer()
{
	//cstr
}

void Lexer::approximateTokenType(char c)
{
	if (this->isNumber(c))
	{
		this->m_currentToken = INT_CONSTANT; //floats and ints start off as int
	}
	else if (this->isAlphabet(c))
	{
		this->m_currentToken = IDENTIFIER; //most others are considered identifier
	}
	else if (c == '\"')
	{
		this->m_currentToken = STRING_LITERAL; //string literal
	}
	else if (c == '\n') //newline is EOL
	{
		this->m_currentToken = EOL;
	}
	else if (this->isPunctuator(c))
	{
		this->m_currentToken = PUNCTUATOR; //operators and punctuators start off as punctuator(also EOL from "////")
	}
	else if (c == ' ')
	{
		this->m_currentToken = WHITESPACE;
	}
	else
	{
		this->m_currentToken = INVALID;
	}
}

bool Lexer::isPunctuator(char c)
{
	switch (c) //runs through all punctuators - () [] and {} must always appear in pairs
	{
	case '!':case '%':case '^':case '&':case '*':case '(':case ')':case '-':case '+':case '=':case '{':case '}':case '|':case '~':case '[':case ']':case '\\':case ';':case '\'':
	case ':':case '\"':case '<':case '>':case '?':case ',':case '.':case '/':case '#':
		return true;
	}

	return false;
}

bool Lexer::isAlphabet(char c)
{
	switch (c) //runs through a-z, A-Z
	{
	case 'a':case 'b':case 'c':case 'd':case 'e':case 'f': case'g':case 'h':case 'i':case 'j':case 'k':case 'l':case 'm':case 'n':case 'o':case 'p':case 'q':case 'r':case 's':case 't':case 'u':case 'v':case 'w':case 'x':case 'y':case 'z':
	case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I':case 'J':case 'K':case 'L':case 'M':case 'N':case 'O':case 'P':case 'Q':case 'R':case 'S':case 'T':case 'U':case 'V':case 'W':case 'X':case 'Y':case 'Z':
		return true;
	}

	return false;
}

bool Lexer::isNumber(char c)
{
	switch (c) //runs through 0-9
	{
	case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
		return true;
	}

	return false;
}

bool Lexer::isPunctuator(char c, char cPeek)
{
	std::string str; //puts 2 together into string
	str += c;
	str += cPeek;

	if (str == "+=" || str == "-=" || str == "*=" || str == "/=") //binary to self
	{
		return true;
	}
	else if (str == "++" || str == "--") //unary to self
	{
		return true;
	}
	else if (str == "==" || str == "!=" || str == "<=" || str == ">=" || str == "||" || str == "&&") //comparators
	{
		return true;
	}
	else if (str == "<<" || str == ">>") //left and right shift
	{
		return true;
	}
	else if (str == "::") //scope
	{
		return true;
	}

	return false; //if none of above
}

bool Lexer::isWhiteSpace(std::string &str)
{
	for (char c : str)
	{
		if (c != ' ')
		{
			return false;
		}
	}

	return true;
}

bool Lexer::isFloat(std::string &str)
{
	char c; //for iteration
	short periods = 0; //amount of times period appears

	for (int i = 0; i < str.size(); i++) //iterate string
	{
		c = str[i];

		if (!this->isNumber(c)) //not number means check for period and check for f at end
		{
			if (c == '.')
			{
				periods++;
			}
			else if (c != 'f' || i != str.size() - 1) //if char is f and it is at end *10.01f, this will NOT RUN!!
			{
				return false;
			}
		}
	}

	if (periods == 1) //check for stuff like 10.0.1??
	{
		return true;
	}
	return false;
}

bool Lexer::isInt(std::string &str)
{
	char c; //for iteration

	for (int i = 0; i < str.size(); i++) //iterate string
	{
		c = str[i];

		if (!this->isNumber(c)) //not number means check for period and check for L at end
		{
			if (c == '.') //this means it is probably a float
			{
				return false;
			}
			else if (c != 'L' || i != str.size() - 1) //if char is L and it is at end *999999999999999999999L, this will NOT RUN!!
			{
				return false;
			}
		}
	}

	return true;
}

bool Lexer::isSymbol(std::string &str)
{
	char c; //for iteration

	if (str.size() >= 1) //check if first char is num, invalid symbol name
	{
		if (this->isNumber(str[0]))
		{
			return false;
		}
	}
	for (int i = 0; i < str.size(); i++) //iterate string
	{
		c = str[i];

		if (!this->isAlphabet(c) && !this->isNumber(c)) //not alphabet and number means maybe a _
		{
			if (c != '_')
			{
				return false;
			}
		}
	}

	return true;
}

void Lexer::lexIntSubstr(std::vector<Token> &output)
{

}

void Lexer::lexIdSubstr(std::vector<Token> &output)
{

}

void Lexer::tokenize(std::vector<Token> &output) //tokenizes the string
{
	if (this->isFloat(this->m_currentSemiToken)) //decimal
	{
		output.push_back(FloatingToken(this->m_currentSemiToken));
	}
	else if (this->isInt(this->m_currentSemiToken)) //integer
	{
		output.push_back(IntegerToken(this->m_currentSemiToken));
	}
	else if (this->isSymbol(this->m_currentSemiToken)) //id or keyword
	{
		output.push_back(IdentifierToken(this->m_currentSemiToken));
	}
	else //invalid
	{
		output.push_back(InvalidToken(this->m_currentSemiToken));
	}

	this->m_currentSemiToken.clear();
}

void Lexer::test(std::string str)
{
	std::cout << "Integer: " << this->isInt(str) << '\n';
	std::cout << "Decimal: " << this->isFloat(str) << '\n';
	std::cout << "Symbol: " << this->isSymbol(str) << '\n';
}

void Lexer::lexFile(std::string &filePath, std::vector<Token> &output, bool ignoreEOL, bool ignoreWhiteSpace) //takes string input, lexes it and adds to the output vector lexed tokens
{
	std::ifstream file(filePath.c_str());
	char c = ' '; //for input
	char cPeek = ' ';
	this->isString = false;
	this->isComment = false;
	this->mulitLineComment = false;

	if (!file.is_open())
	{
		return; //early return wrong filepath
	}
	else
	{
		while (!file.eof()) //goes through file
		{
			c = char(file.get());
			cPeek = char(file.peek());

			if (!this->isString && !this->isComment) //if not in ""
			{
				if (c == ' ') //whitespace??
				{
					if (!ignoreWhiteSpace) //checks for whitespace and pushes back
					{
						this->m_currentSemiToken += c;

						if (cPeek != ' ' && this->isWhiteSpace(this->m_currentSemiToken))
						{
							output.push_back(WhiteSpaceToken(this->m_currentSemiToken));
							this->m_currentSemiToken.clear();
						}
					}
					if (!this->m_currentSemiToken.empty() && !this->isWhiteSpace(this->m_currentSemiToken)) //tokenize if not empty
					{
						this->tokenize(output); //tokenizes output
						this->m_currentSemiToken.clear(); //clears
					}
				}
				else if (c == '\n') //end of line
				{
					if (!ignoreEOL)
					{
						output.push_back(EOLToken());
					}
					if (!this->m_currentSemiToken.empty()) //tokenize if not empty
					{
						this->tokenize(output); //tokenizes output
						this->m_currentSemiToken.clear(); //clears
					}
				}
				else if (c == ';') //end of line
				{
					output.push_back(EOLToken());
					if (!this->m_currentSemiToken.empty()) //tokenize if not empty
					{
						this->tokenize(output); //tokenizes output
						this->m_currentSemiToken.clear(); //clears
					}
				}
				else if (c == '/' && cPeek == '/') // sinlge line comment
				{
					c = file.get(); //forwards .get() to the second / in //
					cPeek = file.peek();

					this->mulitLineComment = false;
					this->isComment = true;
				}
				else if (c == '/' && cPeek == '*') //mulitline comment
				{
					c = file.get(); //forwards .get() to the * in /*
					cPeek = file.peek();

					this->mulitLineComment = true;
					this->isComment = true;
				}
				else if (c != '\t' && !this->isPunctuator(c) && c != '\"') //adds to token if not seperating element
				{
					this->m_currentSemiToken += c;
					if (cPeek == ' ' || (this->isPunctuator(cPeek) && !this->isFloat(std::string(this->m_currentSemiToken + cPeek))) || c == '\"' || cPeek == '\t')
					{
						this->tokenize(output); //tokenizes the currentToken
					}
				}
				else if (c != '\t' && c != '\"') //add punctuator to token
				{
					if (this->isPunctuator(c, cPeek)) //is 2 character operator
					{
						this->m_currentSemiToken += c;
						this->m_currentSemiToken += cPeek; //adds the 2 character op to semitoken
						c = file.get(); //forwards to the 2nd character of the op
						cPeek = file.peek();

						output.push_back(PunctuatorToken(this->m_currentSemiToken)); //adds op to output
						this->m_currentSemiToken.clear(); //clears what not needed
					}
					else if (!this->isFloat(std::string(this->m_currentSemiToken + c + cPeek))) //single character punc guard against floating point
					{
						this->m_currentSemiToken += c;
						output.push_back(PunctuatorToken(this->m_currentSemiToken)); //adds op to output
						this->m_currentSemiToken.clear();
					}
					else //floating point
					{
						this->m_currentSemiToken += c;
					}
				}
				else if (c != '\t') // ""
				{
					isString = true;
				}
			}
			else if (this->isComment)
			{
				if (!this->mulitLineComment) //single line, ends at //
				{
					if (c == '\n') //ends comment
					{
						if (!ignoreEOL) //pushes an EOL if needed
						{
							output.push_back(EOLToken());
						}
						if (!this->m_currentSemiToken.empty()) //tokenize if not empty
						{
							this->tokenize(output); //tokenizes output
							this->m_currentSemiToken.clear(); //clears
						}

						this->isComment = false;
					}
				}
				else //mulitline comment, ends at */
				{
					if (c == '*' && cPeek == '/') //ends comment
					{
						c = file.get(); //forwards to the / character in the */
						cPeek = file.peek();

						this->isComment = false;
					}
					if (!this->m_currentSemiToken.empty()) //tokenize if not empty
					{
						this->tokenize(output); //tokenizes output
						this->m_currentSemiToken.clear(); //clears
					}
				}
			}
			else //in string
			{
				if (c == '\\') //exception time, just like french... XD
				{
					this->m_currentSemiToken += c; //adds the \\ first
					if (cPeek == '\"' || cPeek == '\n' || cPeek == '\t')
					{
						c = file.get(); //forwards to 2nd character of ///n
						cPeek = file.peek();
						this->m_currentSemiToken += c;
					}
				}
				else if (c == '\"') //end string
				{
					isString = false;
					output.push_back(StringLiteralToken(this->m_currentSemiToken)); //straight up uses the token
					this->m_currentSemiToken.clear(); //clears the semitoken
				}
				else if (c != '\n' && c != '\t') //ignore newlines
				{
					this->m_currentSemiToken += c;
				}
				else if (cPeek != ' ' && cPeek != '\t' && cPeek != '\n')
				{
					this->m_currentSemiToken += ' '; //add whitespace
				}
			}
		}
	}
}

Lexer::~Lexer()
{
	//DSTR
}

/*

end Lexer

*/