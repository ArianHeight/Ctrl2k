#pragma once

/*
Helper Class for Parser
*/
class Command
{
public:
	Command();
	~Command();

	virtual void act(int* ptr);
	virtual void act(std::string *ptr);
	virtual void act(float *ptr);
	virtual void act(double *ptr);
	virtual void act(bool *ptr);
	virtual void act(glm::ivec2 *ptr);
	virtual void act(glm::vec2 *ptr);

	virtual std::string getTarget();
	virtual std::string getCommandType();
};

class Assignment : public Command
{
public:
	Assignment(std::string target, Token value);
	Assignment(std::string target, std::vector<Token>& values);
	~Assignment();

	void act(int* ptr);
	void act(std::string *ptr);
	void act(float *ptr);
	void act(double *ptr);
	void act(bool *ptr);
	void act(glm::ivec2 *ptr);
	void act(glm::vec2 *ptr);

	std::string getTarget();
	std::string getCommandType();
private:
	std::string m_target;
	std::vector<Token> m_value;
};

class CallBack : public Command
{
public:
	CallBack(std::string target);
	~CallBack();

	std::string getTarget();
	std::string getCommandType();
private:
	std::string m_target;
};

/*

Main Parser for .game, .mp, and .ent files

*/
/*
class Lexer //used by parser only, so is pre-declared with parser
{
public:
	Lexer();
	~Lexer();

private:
	//no idea what to put here
	bool m_inLiteral; //flag for "" literal
	bool m_isWhiteSpace; //flag
	std::vector<int> m_tokenIndex; //indices for the tokens in the string *might not use
	std::string m_currentSemiToken; //a token only seperated by whitespace and \"

	//single check
	std::vector<char> whiteSpace = { ' ' };
	std::vector<char> stringLiteral = { '\"' };
	std::vector<char> intConstant = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	std::vector<char> floatConstant = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.' };

	//private method
	void addSemiToken(std::string &input, std::vector<Token> &output, int &tokenStart);

public:
	void lexString(std::string &input, std::vector<Token> &output); //takes string input, lexes it and adds to the output vector lexed tokens
};
*/
/*
class Parser
{
public:
	Parser(MapManager &mapOutput, TextureManager &textureOutput, FontManager &fontOutput, AnimationManager &animationOutput);
	~Parser();

private:
	Lexer m_lexer = Lexer();

	MapManager &m_mapRef;
	TextureManager &m_textureRef;
	FontManager &m_fontRef;
	AnimationManager &m_animationRef;

	/*
	for .mp parser
	*//*
	const std::vector<std::string> mpKeys = {
		"name", //map name 0
		"rootdir", //root directory 1
		"visible", //texture 2
		"collision", //collision map 3
		"visualDepth", //depth map 4
		"size", //map size 5
		"wp", //map origin world position 6
		"spawn", //player spawns 7
		"states" //map states 8
		};
	const std::vector<std::string> mpFlags = {
		"active" //active states 0
	};
	const std::string flagStopper = "close";

	int keywordCompare(const std::vector<std::string> &consts, std::string &word); //returns index of const, or -1 if not a keyword
	bool splitToken(std::string &string, std::vector<std::string> &out, char delim = ' '); //splits string into tokens, defaulting with delimiter ' '
	bool splitTokenIgnoreString(std::string &string, std::vector<std::string> &out, char delim = ' '); //splits string into tokens, defaulting with delimiter ' ', ignores " "
	void ignoreDelimiterComment(std::string &string, std::string &out, char delim = '#'); //splits using first instance of comment delimiter, defaulting to '#'
	void cleanPath(std::string &path, std::string &out); //cleans file path
	void checkMapName(std::string &proposedName); //checks map name against map manager
	void checkTextureName(std::string &proposedName); //checks texture name against texture manager
	void removeFileNameFromPath(std::string &string, std::string &out); //removes the file name from the end of a file path

public:
	int parseDotMP(std::string &filePath); 
	bool parseDotGame(std::string &filePath);
	int parseDotFNT(std::string &filePath);
	int parseDotAnim(std::string &filePath);
};
*/

class Assets; //for adding

class Parser
{
public:
	Parser(MapManager &mapOutput, TextureManager &textureOutput, FontManager &fontOutput, AnimationManager &animationOutput);
	Parser(Assets &gameAssets);
	~Parser();

private:
	Lexer m_lexer; //the lexer
	std::vector<Token> m_file; //the token inputs

	MapManager &m_mapRef;
	TextureManager &m_textureRef;
	FontManager &m_fontRef;
	AnimationManager &m_animationRef;

	std::vector<std::string> m_key; //identifiers to compare to
	std::vector<Command*> m_commandList;

	void parse(std::vector<Token> &in);
	void parseFile(std::string &filePath);

	bool splitToken(std::string &string, std::vector<std::string> &out, char delim = ' '); //splits string into tokens, defaulting with delimiter ' '
	void cleanPath(std::string &path, std::string &out); //cleans file path
	void checkMapName(std::string &proposedName); //checks map name against map manager
	void checkTextureName(std::string &proposedName); //checks texture name against texture manager
	void removeFileNameFromPath(std::string &string, std::string &out); //removes the file name from the end of a file path
public:
	bool checkMutableType(std::vector<Token> &in);

	int parseDotMP(std::string &filePath);
	//bool parseDotGame(std::string &filePath);
	int parseDotFNT(std::string &filePath);
	int parseDotAnim(std::string &filePath);
	int parseConfigs(std::string &filePath, std::string &name = std::string(""), std::vector<int> &data = std::vector<int>());
};

/*

end Main Parser

*/