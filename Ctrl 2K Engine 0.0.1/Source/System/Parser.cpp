#include <Ctrl 2kPCH.h>

//helper functions pre-declaration
//template <typename T>
//bool inVector(T in, std::vector<T>& theList);

/*
Lexer::Lexer()
{
	//cstr
}

void Lexer::addSemiToken(std::string &input, std::vector<Token> &output, int &tokenStart)
{

}

void Lexer::lexString(std::string &input, std::vector<Token> &output)
{
	int stringLength = input.size();
	std::string currentChar;
	int tokenStart = 0;
	this->m_currentSemiToken.clear();

	for (int cursor = 0; cursor < stringLength; cursor++) //pre-iter
	{
		currentChar = input[cursor];

		this->m_currentSemiToken.append(currentChar);

	}
}

Lexer::~Lexer()
{
	//dstr
}

/*

end Lexer

*/

/*

Commands

*/

Command::Command()
{
	//cstr
}

void Command::act(int* ptr)
{

}

void Command::act(std::string *ptr)
{

}

void Command::act(float *ptr)
{

}

void Command::act(double *ptr)
{

}

void Command::act(bool *ptr)
{
	
}

void Command::act(glm::ivec2 *ptr)
{

}

void Command::act(glm::vec2 *ptr)
{

}

std::string Command::getTarget()
{
	return "";
}

std::string Command::getCommandType()
{
	return "Base";
}

Command::~Command()
{
	//dstr
}

Assignment::Assignment(std::string target, Token value) : 
	m_target(target)
{
	//cstr
	this->m_value.push_back(value);
}

Assignment::Assignment(std::string target, std::vector<Token>& values) : 
	m_target(target)
{
	//multi-value assignment
	this->m_value = values;
}

void Assignment::act(int* ptr)
{
	if (this->m_value[0].getType() == INT_CONSTANT || this->m_value[0].getType() == FLOATING_CONSTANT)
	{
		*ptr = std::stoi(this->m_value[0].getValue());
	}
}

void Assignment::act(std::string *ptr)
{
	if (this->m_value[0].getType() == STRING_LITERAL)
	{
		*ptr = this->m_value[0].getValue();
	}
}

void Assignment::act(float *ptr)
{
	if (this->m_value[0].getType() == INT_CONSTANT || this->m_value[0].getType() == FLOATING_CONSTANT)
	{
		*ptr = std::stof(this->m_value[0].getValue());
	}
}

void Assignment::act(double *ptr)
{
	if (this->m_value[0].getType() == INT_CONSTANT || this->m_value[0].getType() == FLOATING_CONSTANT)
	{
		*ptr = std::stod(this->m_value[0].getValue());
	}
}

void Assignment::act(bool *ptr)
{
	if (this->m_value[0].getType() == INT_CONSTANT || this->m_value[0].getValue() == "true" || this->m_value[0].getValue() == "false")
	{
		if (this->m_value[0].getValue() == "true")
		{
			*ptr = true;
		}
		else
		{
			*ptr = false;
		}
	}
}

void Assignment::act(glm::ivec2 *ptr)
{
	if (this->m_value[0].getType() == INT_CONSTANT && this->m_value.size() == 3)
	{
		if (this->m_value[2].getType() == INT_CONSTANT)
		{
			ptr->x = std::stoi(this->m_value[0].getValue());
			ptr->y = std::stoi(this->m_value[2].getValue());
		}
	}
}

void Assignment::act(glm::vec2 *ptr)
{
	if ((this->m_value[0].getType() == INT_CONSTANT || this->m_value[0].getType() == FLOATING_CONSTANT) &&  this->m_value.size() == 3)
	{
		if (this->m_value[2].getType() == INT_CONSTANT || this->m_value[2].getType() == FLOATING_CONSTANT)
		{
			ptr->x = std::stof(this->m_value[0].getValue());
			ptr->y = std::stof(this->m_value[2].getValue());
		}
	}
}

std::string Assignment::getTarget()
{
	return this->m_target;
}

std::string Assignment::getCommandType()
{
	return "Assignment";
}

Assignment::~Assignment()
{
	//dstr
}

CallBack::CallBack(std::string target) :
	m_target(target)
{
	//cstr
}

std::string CallBack::getTarget()
{
	return this->m_target;
}

std::string CallBack::getCommandType()
{
	return "CallBack";
}

CallBack::~CallBack()
{
	//dstr
}

/*

end Commands

*/

/*

Main Parser for .game, .mp, and .ent files

*/

bool tokenInside(std::string obj, std::vector<Token> &in)
{
	for (int i = 0; i < in.size(); i++)
	{
		if (in[i].getValue() == obj)
		{
			return true;
		}
	}
	return false;
}

Parser::Parser(MapManager &mapOutput, TextureManager &textureOutput, FontManager &fontOutput, AnimationManager &animationOutput) :
	m_lexer(Lexer()), m_mapRef(mapOutput), m_textureRef(textureOutput), m_fontRef(fontOutput), m_animationRef(animationOutput)
{
	//cstr
}

Parser::Parser(Assets &gameAssets) : 
	m_lexer(Lexer()), m_mapRef(gameAssets.getMM()), m_textureRef(gameAssets.getTM()), m_fontRef(gameAssets.getFM()), m_animationRef(gameAssets.getAM())
{
	//cstr
}

bool Parser::checkMutableType(std::vector<Token> &in)
{
	/*
	numbers - INT_CONSTANT, FLOATING_CONSTANT
	strings - STRING_LITERAL
	*/
	if (in[0].getType() == INT_CONSTANT || in[0].getType() == FLOATING_CONSTANT)
	{
		for (Token t : in)
		{
			if (t.getType() != INT_CONSTANT && t.getType() != FLOATING_CONSTANT)
			{
				return false;
			}
		}
		return true;
	}
	else if (in[0].getType() == STRING_LITERAL)
	{
		for (Token t : in)
		{
			if (t.getType() != STRING_LITERAL)
			{
				return false;
			}
		}
		return true;
	}
	else if (in[0].getValue() == "true" || in[0].getValue() == "false")
	{
		for (Token t : in)
		{
			if (in[0].getValue() != "true" || in[0].getValue() != "false")
			{
				return false;
			}
		}
		return true;
	}

	return false;
}

void Parser::parse(std::vector<Token> &in)
{
	std::vector<Token> buffer;
	std::string target;

	for (int i = 0; i < in.size(); i++) //parse through tokens
	{
		if (in[i].getValue() == "-" && (in[i + 1].getType() == INT_CONSTANT || in[i + 1].getType() == FLOATING_CONSTANT))
		{
			in[i + 1].addNegative();
			in.erase(in.begin() + i);
		}
	}

	for (int i = 0; i < in.size(); i++) //parse through tokens
	{
		if (in[i].getType() != EOL) //only push to buffer if not end of line
		{
			buffer.push_back(in[i]);
		}
		else //check with forms at end of line
		{
			if (buffer[0].getType() == IDENTIFIER) //Id is first token
			{
				int nameEnd = 0;
				target = "";
				for (int j = 0; j < buffer.size(); j++) //used for targets with variable name of Id.Id.Id to find index of last ID
				{
					if (buffer[j].getType() != IDENTIFIER && buffer[j].getValue() != ".")
					{
						nameEnd = j;
						break;
					}
					else
					{
						target += buffer[j].getValue(); //appends to name
					}
				}
				buffer.erase(buffer.begin(), buffer.begin() + nameEnd);

				if (buffer[0].getValue() == "(" && tokenInside(")", buffer)) //callback
				{
					this->m_commandList.push_back(new CallBack(target));
				}
				else if (buffer[0].getValue() == "{")
				{
					//block
				}
				else if (buffer[0].getValue() == "=") //assignment
				{
					buffer.erase(buffer.begin());
					//if (this->checkMutableType(buffer))
					if (true)
					{
						this->m_commandList.push_back(new Assignment(target, buffer)); //adds the command
					}
				}
				buffer.clear();
			}
		}
	}
	//std::cout << "done parsing" << std::endl;
}

void printToken(Token t)
{
	std::string str = "";
	std::string space = "";

	switch (t.getType())
	{
	case WHITESPACE: //0
		str = "whitespace";
		break;
	case EOL: //1 End of Line
		str = "End of Line";
		break;
	case STRING_LITERAL: //2 string literal
		str = "string literal";
		break;
	case INT_CONSTANT: //3 integer
		str = "integer";
		break;
	case FLOATING_CONSTANT: //4 float
		str = "floating point";
		break;
	case IDENTIFIER: //5 identifier
		str = "identifier";
		break;
	case KEYWORD: //6 keywords
		str = "keyword";
		break;
	case PUNCTUATOR: //7 (), {} and other stuff like that as well as all operators
		str = "puncuator";
		break;
	case INVALID: //8 invalid tokens
		str = "ERROR INVALID TOKEN";
		break;
	}

	std::cout << '[' << str.c_str() << ']';
	for (int i = 0; i < 20 - str.size(); i++)
	{
		space += ' ';
	}
	std::cout << space.c_str() << std::string(" : [").c_str() << t.getValue().c_str() << ']' << '\n';
}

void Parser::parseFile(std::string &filePath)
{
	this->m_file.clear();
	this->m_commandList.clear();
	this->m_lexer.lexFile(filePath, this->m_file); //splits file into tokens
	this->parse(this->m_file); //parses the file into m_commandList
}

bool Parser::splitToken(std::string &string, std::vector<std::string> &out, char delim)
{
	string.push_back(delim); //to get last part of string to output to vector

	int tokenLength = 0;
	int tokenBegin = 0;
	int size = string.size();
	char c = 0;
	bool delimAppeared = false;
	bool stringFlag = false;

	for (int count = 0; count < size; count++)
	{
		c = string[count];

		if (c == delim && !stringFlag) //delim is hit while not in between ""
		{
			if (tokenLength != 0) //check so it doesn't output a delim in case of double delim
			{
				if (tokenLength < size)
				{
					delimAppeared = true;
				}
				out.push_back(string.substr(tokenBegin, tokenLength));
				tokenLength = 0;
			}
		}
		else if (c == '\"') //hits string marker
		{
			if (tokenLength != 0)
			{
				out.push_back(string.substr(tokenBegin, tokenLength));
				tokenLength = 0;
			}

			if (stringFlag) //flag update
			{
				stringFlag = false;
			}
			else
			{
				stringFlag = true;
			}
		}
		else
		{
			if (tokenLength == 0)
			{
				tokenBegin = count;
			}
			tokenLength += 1;
		}
	}

	string.pop_back(); //pop added delim off back

	return delimAppeared;
}

void Parser::cleanPath(std::string &path, std::string &out)
{
	std::vector<std::string> bufferOne;
	std::vector<std::string> bufferTwo;
	int size;

	this->splitToken(path, bufferOne, '/'); //first split
	size = bufferOne.size();
	for (int count = 0; count < size; count++)
	{
		this->splitToken(bufferOne[count], bufferTwo, '\\');
	} //at this point, all tokens of path should be split into bufferTwo

	size = bufferTwo.size();
	for (int count = 0; count < size; count++)
	{
		if (bufferTwo[count] == ".." && count - 1 >= 0)
		{
			if (bufferTwo[count - 1] != "..")
			{
				bufferTwo.erase(bufferTwo.begin() + count); //current
				bufferTwo.erase(bufferTwo.begin() + count - 1); //previous

																//update size and count
				size -= 2;
				count -= 2;
			}
		}
	}

	size = bufferTwo.size();
	for (int count = 0; count < size; count++)
	{
		out.append(bufferTwo[count] + "/");
	}
	out.pop_back(); //takes out final extra /
}

void Parser::checkMapName(std::string &proposedName)
{
	if (this->m_mapRef.search(proposedName) != -1)
	{
		int count = 1;
		while (this->m_mapRef.search(proposedName + std::to_string(count)) != -1)
		{
			count++;
			if (count > 100)
			{
				std::cerr << "Too many maps with the name: " << proposedName << std::endl;
				return;
			}
		}
		proposedName.append(std::to_string(count));
	}
}

void Parser::checkTextureName(std::string &proposedName)
{
	if (this->m_textureRef.search(proposedName) != -1)
	{
		int count = 1;
		while (this->m_textureRef.search(proposedName + std::to_string(count)) != -1)
		{
			count++;
			if (count > 100)
			{
				std::cerr << "Too many textures with the name: " << proposedName << std::endl;
				return;
			}
		}
		proposedName.append(std::to_string(count));
	}
}

void Parser::removeFileNameFromPath(std::string &string, std::string &out)
{
	std::vector<std::string> bufferOne;
	std::vector<std::string> bufferTwo;
	int size;

	this->splitToken(string, bufferOne, '/'); //first split
	size = bufferOne.size();
	for (int count = 0; count < size; count++)
	{
		this->splitToken(bufferOne[count], bufferTwo, '\\');
	} //at this point, all tokens of path should be split into bufferTwo

	bufferTwo.pop_back(); //remove file name

	size = bufferTwo.size();
	for (int count = 0; count < size; count++)
	{
		out.append(bufferTwo[count] + "/");
	}
	out.pop_back(); //takes out final extra /
}

int Parser::parseDotMP(std::string &filePath)
{
	this->parseFile(filePath); //outputs to m_commandList
	std::string commandTarget = "";

	std::vector<std::string> pathTokens;
	std::vector<std::string> mapName;

	std::string fooString;
	std::string mRootDir = "";
	std::string mName;
	std::string mVisName;
	std::string mVisPath;
	std::string mCollName;
	std::string mCollPath;
	std::string mDepthName;
	std::string mDepthPath;
	float mSize = 0.0f;
	glm::vec2 mWP;
	int numStates = 0.0f;
	std::vector<glm::vec2> mSpawns;

	for (Command* c : this->m_commandList) //iterates
	{
		if (c->getCommandType() == "Assignment")
		{
			commandTarget = c->getTarget();
			if (commandTarget == "name")
			{
				c->act(&mName);
				this->checkMapName(mName);
			}
			else if (commandTarget == "rootdir")
			{
				c->act(&mRootDir);
				std::string base;
				this->removeFileNameFromPath(filePath, base);
				mRootDir = base + "/" + mRootDir;
				this->cleanPath(mRootDir, fooString);
				mRootDir = fooString;
				fooString.clear();
			}
			else if (commandTarget == "visible")
			{
				std::string base;
				c->act(&base);
				this->cleanPath(mRootDir + "/" + base, mVisPath);
				this->splitToken(mVisPath, pathTokens, '/');
				this->splitToken(pathTokens[pathTokens.size() - 1], mapName, '.');
				mVisName = mapName[0];
				mapName.clear();
				//this->checkTextureName(mVisName);
			}
			else if (commandTarget == "collision")
			{
				std::string base;
				c->act(&base);
				this->cleanPath(mRootDir + "/" + base, mCollPath);
				this->splitToken(mCollPath, pathTokens, '/');
				this->splitToken(pathTokens[pathTokens.size() - 1], mapName, '.');
				mCollName = mapName[0];
				mapName.clear();
				//this->checkTextureName(mCollName);
			}
			else if (commandTarget == "visualDepth")
			{
				std::string base;
				c->act(&base);
				this->cleanPath(mRootDir + "/" + base, mDepthPath);
				this->splitToken(mDepthPath, pathTokens, '/');
				this->splitToken(pathTokens[pathTokens.size() - 1], mapName, '.');
				mDepthName = mapName[0];
				mapName.clear();
				//this->checkTextureName(mDepthName);
			}
			else if (commandTarget == "size")
			{
				c->act(&mSize);
			}
			else if (commandTarget == "wp")
			{
				c->act(&mWP);
			}
			else if (commandTarget == "spawn")
			{
				glm::vec2 sp;
				c->act(&sp);
				mSpawns.push_back(sp);
			}
		}
	}

	this->m_textureRef.create(mVisName, mVisPath);
	this->m_textureRef.create(mCollName, mCollPath);
	this->m_textureRef.create(mDepthName, mDepthPath);

	return this->m_mapRef.create(mName, mVisName, mCollName, mDepthName, mSize, mWP);
}

int Parser::parseDotFNT(std::string &filePath)
{
	this->m_file.clear();
	this->m_lexer.lexFile(filePath, this->m_file); //splits file into tokens

	std::string fFace;
	int fSize;
	float fLineHeight;
	float fBase;
	float fWidth = 1;
	float fHeight = 1;
	std::string fAtlasPath;
	this->removeFileNameFromPath(filePath, fAtlasPath);
	int fCharCount;

	char cID = -1;
	int x;
	int y;
	int width;
	int height;
	int xoffset;
	int yoffset;
	int advance;

	Font *newFont = new Font();

	for (int i = 0; i < this->m_file.size(); i++)
	{
		if (this->m_file[i].getValue() == "char")
		{
			if (cID != -1) //new char created as long as it is not first instance of char
			{
				newFont->m_characters.insert(std::pair<char, FontChar>(cID, FontChar((float)x, (float)y, (float)width, (float)height, (float)xoffset, (float)yoffset, (float)advance, fWidth, fHeight, (float)fSize)));
			}
			else
			{
				cID = 0; //frees id mod so that it updates on first char
			}
		}
		else if (this->m_file[i].getValue() == "id" && this->m_file[i - 1].getValue() == "char")
		{
			if (this->m_file[i + 2].getType() == INT_CONSTANT)
			{
				cID = std::stoi(this->m_file[i + 2].getValue());
			}
		}
		else if (this->m_file[i].getValue() == "x")
		{
			if (this->m_file[i + 2].getType() == INT_CONSTANT)
			{
				x = std::stoi(this->m_file[i + 2].getValue());
			}
		}
		else if (this->m_file[i].getValue() == "y")
		{
			if (this->m_file[i + 2].getType() == INT_CONSTANT)
			{
				y = std::stoi(this->m_file[i + 2].getValue());
			}
		}
		else if (this->m_file[i].getValue() == "width")
		{
			if (this->m_file[i + 2].getType() == INT_CONSTANT)
			{
				width = std::stoi(this->m_file[i + 2].getValue());
			}
		}
		else if (this->m_file[i].getValue() == "height")
		{
			if (this->m_file[i + 2].getType() == INT_CONSTANT)
			{
				height = std::stoi(this->m_file[i + 2].getValue());
			}
		}
		else if (this->m_file[i].getValue() == "xoffset")
		{
			if (this->m_file[i + 2].getType() == INT_CONSTANT)
			{
				xoffset = std::stoi(this->m_file[i + 2].getValue());
			}
		}
		else if (this->m_file[i].getValue() == "yoffset")
		{
			if (this->m_file[i + 2].getType() == INT_CONSTANT)
			{
				yoffset = std::stoi(this->m_file[i + 2].getValue());
			}
		}
		else if (this->m_file[i].getValue() == "xadvance")
		{
			if (this->m_file[i + 2].getType() == INT_CONSTANT)
			{
				advance = std::stoi(this->m_file[i + 2].getValue());
			}
		}
		else if (this->m_file[i].getValue() == "count")
		{
			if (this->m_file[i + 2].getType() == INT_CONSTANT)
			{
				fCharCount = std::stoi(this->m_file[i + 2].getValue());
			}
		}
		else if (this->m_file[i].getValue() == "size")
		{
			if (this->m_file[i + 2].getType() == INT_CONSTANT)
			{
				fSize = std::stoi(this->m_file[i + 2].getValue());
			}
		}
		else if (this->m_file[i].getValue() == "face")
		{
			if (this->m_file[i + 2].getType() == STRING_LITERAL)
			{
				fFace = this->m_file[i + 2].getValue();
			}
		}
		else if (this->m_file[i].getValue() == "file")
		{
			if (this->m_file[i + 2].getType() == STRING_LITERAL)
			{
				std::string fp = fAtlasPath + "/" +  this->m_file[i + 2].getValue();
				fAtlasPath.clear();
				this->cleanPath(fp, fAtlasPath);
			}
		}
		else if (this->m_file[i].getValue() == "lineHeight")
		{
			if (this->m_file[i + 2].getType() == INT_CONSTANT)
			{
				fLineHeight = std::stoi(this->m_file[i + 2].getValue());
			}
		}
		else if (this->m_file[i].getValue() == "base")
		{
			if (this->m_file[i + 2].getType() == INT_CONSTANT)
			{
				fBase = std::stoi(this->m_file[i + 2].getValue());
			}
		}
		else if (this->m_file[i].getValue() == "scaleW")
		{
			if (this->m_file[i + 2].getType() == INT_CONSTANT)
			{
				fWidth = std::stoi(this->m_file[i + 2].getValue());
			}
		}
		else if (this->m_file[i].getValue() == "scaleH")
		{
			if (this->m_file[i + 2].getType() == INT_CONSTANT)
			{
				fHeight = std::stoi(this->m_file[i + 2].getValue());
			}
		}
	}
	//for the last char
	newFont->m_characters.insert(std::pair<char, FontChar>(cID, FontChar((float)x, (float)y, (float)width, (float)height, (float)xoffset, (float)yoffset, (float)advance, fWidth, fHeight, (float)fSize)));
	
	newFont->setAtlas(SOIL_load_OGL_texture(fAtlasPath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y)); //loads texture into OGL(openGL));
	newFont->bindAtlas();
	//sets texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	newFont->setAtlasDim(fWidth, fHeight);
	newFont->setBase((float)fBase);
	newFont->setFace(fFace);
	newFont->setLineHeight(fLineHeight);
	newFont->setSize(fSize);
	newFont->normalizeValues();

	return this->m_fontRef.addFont(newFont);
}

int Parser::parseDotAnim(std::string &filePath)
{
	this->parseFile(filePath); //outputs to m_commandList
	std::string commandTarget = "";

	Animation *currentAnimation = new Animation();

	std::string processed = "";
	std::vector<std::string> pathTokens;
	std::vector<std::string> lineTokens;

	std::string name;
	std::string textureDir;
	std::string audioDir;
	std::string atlasPath;
	std::string atlasName;
	std::string audioPath;
	std::string audioName;
	double totalTime = 0.0; //total animation time in seconds
	double defaultTimePerFrame = 0.0;
	int rows = 0;
	int rowHeight;
	int pixelToWorld; //how many pixels are in 1.0f in world space
	int defaultFrameCount = 1;
	bool repeatableAnimation = true;
	glm::ivec2 defaultFrameDimensions;
	glm::ivec2 atlasDimensions;

	int prFrameCount = -1; //pr stands for per-row
	std::vector<double> pfTime; //non-default time perframe
	std::vector<glm::ivec2> pfFrameDimensions;
	std::vector<bool> checklist; //true if row has been added, false if not
	bool flag = false; //true if a certain row is flagged
	int flaggedRow = 0; //the row that is currently flagged

	for (Command* c : this->m_commandList) //iterates
	{
		if (c->getCommandType() == "Assignment")
		{
			commandTarget = c->getTarget();
			if (commandTarget == "name")
			{
				c->act(&name);
				int aniIndex = this->m_animationRef.search(name);
				if (aniIndex != -1) //already exists or name overlap
				{
					delete currentAnimation;
					currentAnimation = nullptr;
					return aniIndex;
				}
			}
			else if (commandTarget == "rootDir.texture")
			{
				c->act(&textureDir);
				std::string base;
				this->removeFileNameFromPath(filePath, base);
				textureDir = base + "/" + textureDir;
				this->cleanPath(textureDir, processed);
				textureDir = processed;
				processed.clear();
			}
			else if (commandTarget == "rootDir.audio")
			{
				c->act(&audioDir);
				std::string base;
				this->removeFileNameFromPath(filePath, base);
				audioDir = base + "/" + audioDir;
				this->cleanPath(audioDir, processed);
				audioDir = processed;
				processed.clear();
			}
			else if (commandTarget == "spriteSheet")
			{
				c->act(&atlasName);
				this->cleanPath(textureDir + "/" + atlasName, atlasPath);
				this->splitToken(atlasPath, pathTokens, '/');
				this->splitToken(pathTokens[pathTokens.size() - 1], lineTokens, '.');
				atlasName = lineTokens[0];
				//this->checkTextureName(atlasName);
			}
			else if (commandTarget == "atlasDim")
			{
				c->act(&atlasDimensions);
			}
			else if (commandTarget == "rows")
			{
				c->act(&rows);
				checklist.resize(rows);
				pfFrameDimensions.resize(rows);
				for (int i = 0; i < rows; i++)
				{
					checklist[i] = false;
					pfFrameDimensions[i] = glm::ivec2(-1);
				}
				currentAnimation->setRows(rows);
			}
			else if (commandTarget == "ptw")
			{
				c->act(&pixelToWorld);
			}
			else if (commandTarget == "rowHeight")
			{
				c->act(&rowHeight);
			}
			else if (commandTarget == "sameFrameHeight")
			{
				c->act(&defaultFrameDimensions.y);
			}
			else if (commandTarget == "sameFrameWidth")
			{
				c->act(&defaultFrameDimensions.x);
			}
			else if (commandTarget == "totalTime")
			{
				c->act(&totalTime);
				currentAnimation->setTotalTime(totalTime);
			}
			else if (commandTarget == "sameFrameCount")
			{
				c->act(&defaultFrameCount);
			}
			else if (commandTarget == "repeat")
			{
				c->act(&repeatableAnimation);
			}
			else if (commandTarget == "flagRow")
			{
				c->act(&flaggedRow);
				flag = true;
			}
			else if (commandTarget == "frameWidth")
			{
				c->act(&pfFrameDimensions[flaggedRow].x);
			}
			else if (commandTarget == "frameHeight")
			{
				c->act(&pfFrameDimensions[flaggedRow].y);
			}
			else if (commandTarget == "close")
			{
				if (flag == true)
				{
					flag = false;
					checklist[flaggedRow] = true;
					flaggedRow = 0;
				}
			}
		}
		else if (c->getCommandType() == "CallBack")
		{

		}
	}

	defaultTimePerFrame = totalTime / (double)defaultFrameCount;
	glm::ivec2 cursor;
	cursor.y = atlasDimensions.y;

	//compiles prlist
	for (int count = 0; count < rows; count++) //adds each frame to the animation
	{
		if (!checklist[count]) //if not been added yet
		{
			pfFrameDimensions[count] = defaultFrameDimensions;
		}
	}
	for (int count = 0; count < rows; count++) //adds each frame to the animation
	{
		double timeFrame = 0.0;
		cursor.x = 0;
		if (count > 0) //at 0, cursor.y = atlasDim.
		{
			cursor.y -= pfFrameDimensions[count - 1].y;
		}
		for (int i = 0; i < defaultFrameCount; i++)
		{
			currentAnimation->addFrame(count, cursor, pfFrameDimensions[count], atlasDimensions, pixelToWorld, timeFrame, defaultTimePerFrame);
			cursor.x += pfFrameDimensions[count].x;
			timeFrame += defaultTimePerFrame;
		}
	}

	currentAnimation->setName(name);
	currentAnimation->setAtlas(this->m_textureRef.create(atlasName, atlasPath));
	currentAnimation->setRepeatable(repeatableAnimation);

	return this->m_animationRef.addAnimation(currentAnimation);
}

int Parser::parseConfigs(std::string &filePath, std::string &name, std::vector<int> &data)
{
	this->parseFile(filePath); //outputs to m_commandList
	std::string commandTarget = "";

	std::string targetSector;
	bool fullscreen;
	bool vsync;
	int windowWidth;
	int windowHeight;
	glm::ivec2 resolution;

	for (Command* c : this->m_commandList) //iterates
	{
		if (c->getCommandType() == "Assignment")
		{
			commandTarget = c->getTarget();
			if (commandTarget == "name")
			{
				c->act(&name);
			}
			else if (commandTarget == "fullscreen")
			{
				c->act(&fullscreen);
			}
			else if (commandTarget == "vsync")
			{
				c->act(&vsync);
			}
			else if (commandTarget == "windowWidth")
			{
				c->act(&windowWidth);
			}
			else if (commandTarget == "windowHeight")
			{
				c->act(&windowHeight);
			}
			else if (commandTarget == "resolution")
			{
				c->act(&resolution);
			}
		}
	}

	//outputs to vector
	data.push_back(fullscreen);
	data.push_back(vsync);
	data.push_back(windowWidth);
	data.push_back(windowHeight);
	data.push_back(resolution.x);
	data.push_back(resolution.y);

	return 0;
}

Parser::~Parser()
{
	//dstr
}

/*
int Parser::keywordCompare(const std::vector<std::string> &consts, std::string &word)
{
	int size = consts.size();
	for (int i = 0; i < size; i++)
	{
		if (consts[i] == word)
		{
			return i;
		}
	}
	return -1;
}

void Parser::ignoreDelimiterComment(std::string &string, std::string &out, char delim)
{
	int size = string.size();
	char c = 0;
	bool stringFlag = false;

	for (int count = 0; count < size; count++)
	{
		c = string[count];

		if (c == delim && !stringFlag)
		{
			size = count;
		}
		else if (c == '\"')
		{
			if (stringFlag) //flag update
			{
				stringFlag = false;
			}
			else
			{
				stringFlag = true;
			}
		}
	}

	out = string.substr(0, size);
}

bool Parser::splitTokenIgnoreString(std::string &string, std::vector<std::string> &out, char delim )
{
	string.push_back(delim); //to get last part of string to output to vector

	int tokenLength = 0;
	int tokenBegin = 0;
	int size = string.size();
	char c = 0;
	bool delimAppeared = false;

	for (int count = 0; count < size; count++)
	{
		c = string[count];

		if (c == delim) //delim is hit while not in between ""
		{
			if (tokenLength != 0) //check so it doesn't output a delim in case of double delim
			{
				if (tokenLength < size)
				{
					delimAppeared = true;
				}
				out.push_back(string.substr(tokenBegin, tokenLength));
				tokenLength = 0;
			}
		}
		else
		{
			if (tokenLength == 0)
			{
				tokenBegin = count;
			}
			tokenLength += 1;
		}
	}

	string.pop_back(); //pop added delim off back

	return delimAppeared;
}

int Parser::parseDotMP(std::string &filePath)
{
	std::ifstream file(filePath.c_str());

	if (!file.is_open()) //file not open properly
	{
		std::cerr << "file " << filePath << " not opened properly" << std::endl;
		return -1;
	}

	std::string currentLine = "";
	std::string processed = "";
	std::vector<std::string> tokens;
	std::vector<std::string> pathTokens;
	std::vector<std::string> mapName;
	tokens.reserve(4);
	pathTokens.reserve(6);
	mapName.reserve(2);

	std::string fooString = "";
	int tokenSize = 0;
	std::string mRootDir = "";
	std::string mName;
	std::string mVisName;
	std::string mVisPath;
	std::string mCollName;
	std::string mCollPath;
	std::string mDepthName;
	std::string mDepthPath;
	float mSize;
	glm::vec2 mWP;
	int numStates;
	std::vector<glm::vec2> mSpawns;

	int keywordIndex = -1;
	bool flag = false;
	int flagIndex = -1;

	while (std::getline(file, currentLine))
	{
		tokens.clear();
		pathTokens.clear();
		mapName.clear();

		this->ignoreDelimiterComment(currentLine, processed);
		currentLine = processed;

		this->splitToken(currentLine, tokens); //splits into tokens

		tokenSize = tokens.size();
		if (tokenSize > 1 && !flag) //not flagged
		{
			keywordIndex = this->keywordCompare(this->mpKeys, tokens[0]); //check against keywords
			if (keywordIndex != -1) //keyword
			{
				switch (keywordIndex)
				{
				case 0: //map name
					if (tokenSize == 2)
					{
						mName = tokens[1];
						this->checkMapName(mName);
					}
					break;
				case 1: //root dir
					if (tokenSize == 2)
					{
						this->removeFileNameFromPath(filePath, mRootDir);
						mRootDir.append("/" + tokens[1]);
						this->cleanPath(mRootDir, fooString);
						mRootDir = fooString;
						fooString.clear();
					}
					break;
				case 2: //visible
					if (tokenSize == 2) //derived name
					{
						this->cleanPath(mRootDir + "/" + tokens[1], mVisPath);
						this->splitToken(mVisPath, pathTokens, '/');
						this->splitToken(pathTokens[pathTokens.size() - 1], mapName, '.');
						mVisName = mapName[0];
						//this->checkTextureName(mVisName);
					}
					else if (tokenSize == 3)
					{
						this->cleanPath(mRootDir + "/" + tokens[2], mVisPath);
						mVisName = tokens[1];
						//this->checkTextureName(mVisName);
					}
					break;
				case 3: //collision
					if (tokenSize == 2) //derived name
					{
						this->cleanPath(mRootDir + "/" + tokens[1], mCollPath);
						this->splitToken(mCollPath, pathTokens, '/');
						this->splitToken(pathTokens[pathTokens.size() - 1], mapName, '.');
						mCollName = mapName[0];
						//this->checkTextureName(mCollName);
					}
					else if (tokenSize == 3)
					{
						this->cleanPath(mRootDir + "/" + tokens[2], mCollPath);
						mCollName = tokens[1];
						//this->checkTextureName(mCollName);
					}
					break;
				case 4: //depth
					if (tokenSize == 2) //derived name
					{
						this->cleanPath(mRootDir + "/" + tokens[1], mDepthPath);
						this->splitToken(mDepthPath, pathTokens, '/');
						this->splitToken(pathTokens[pathTokens.size() - 1], mapName, '.');
						mDepthName = mapName[0];
						//this->checkTextureName(mDepthName);
					}
					else if (tokenSize == 3)
					{
						this->cleanPath(mRootDir + "/" + tokens[2], mDepthPath);
						mDepthName = tokens[1];
						//this->checkTextureName(mDepthName);
					}
					break;
				case 5: //map size
					if (tokenSize == 2)
					{
						try
						{
							mSize = std::stof(tokens[1]);
						}
						catch (...)
						{
							mSize = 1;
						}
					}
					break;
				case 6: //map origin world pos
					if (tokenSize == 3)
					{
						try
						{
							mWP.x = std::stof(tokens[1]);
							mWP.y = std::stof(tokens[2]);
						}
						catch (...)
						{
							mWP.x = 0.0f;
							mWP.y = 0.0f;
						}
					}
					break;
				case 7: //player spawns
					if (tokenSize == 3)
					{
						glm::vec2 temp;
						try
						{
							temp.x = std::stof(tokens[1]);
							temp.y = std::stof(tokens[2]);
						}
						catch (...)
						{
							temp.x = 0.0f;
							temp.y = 0.0f;
						}
						mSpawns.push_back(temp);
					}
					break;
				case 8: //map states
					if (tokenSize == 2)
					{
						try
						{
							numStates = std::stoi(tokens[1]);
						}
						catch (...)
						{
							numStates = 1;
						}
					}
					break;
				}
			}
			else //flag
			{
				keywordIndex = this->keywordCompare(this->mpFlags, tokens[0]);
				if (keywordIndex != -1) //actual flag
				{
					flag = true;

					switch (keywordIndex)
					{
					case 0: //active
						flagIndex = 0;
						break;
					}
				}
			}
		}
		else if (tokenSize > 1) //flagged
		{
			if (tokens[0] == this->flagStopper)
			{
				flag = false;
				flagIndex = -1;
			}
			else
			{
				//check flag state and do things in accordance to flag rules
				switch (flagIndex)
				{
				case 0: //active
					break;
				}
			}
		}
	}
	
	file.close();

	this->m_textureRef.create(mVisName, mVisPath);
	this->m_textureRef.create(mCollName, mCollPath);
	this->m_textureRef.create(mDepthName, mDepthPath);

	return this->m_mapRef.create(mName, mVisName, mCollName, mDepthName, mSize, mWP);
}

bool Parser::parseDotGame(std::string &filePath)
{
	return true;
}

int Parser::parseDotFNT(std::string &filePath)
{
	std::ifstream file(filePath.c_str());

	if (!file.is_open()) //file not open properly
	{
		std::cerr << "file " << filePath << " not opened properly" << std::endl;
		return -1;
	}

	std::string currentLine = "";
	std::string fooString = "";
	std::string segment;
	int tokenSize = 0;
	std::vector<std::string> tokens;
	std::vector<std::string> pathTokens;
	std::vector<std::string> lineTokens;
	tokens.reserve(12);
	pathTokens.reserve(12);
	lineTokens.reserve(12);

	std::string fFace;
	int fSize;
	float fLineHeight;
	float fBase;
	float fWidth = 1;
	float fHeight = 1;
	std::string fAtlasPath;
	this->removeFileNameFromPath(filePath, fAtlasPath);
	int fCharCount;

	char cID;
	int x;
	int y;
	int width;
	int height;
	int xoffset;
	int yoffset;
	int advance;

	Font *newFont = new Font();

	while (std::getline(file, currentLine))
	{
		tokens.clear();
		pathTokens.clear();
		lineTokens.clear();

		this->splitToken(currentLine, tokens);
		tokenSize = tokens.size();

		if (tokens.size() > 1)
		{
			fooString = tokens[0];

			for (int i = 0; i < tokenSize; i++)
			{
				lineTokens.clear();
				segment = tokens[i];

				this->splitToken(segment, lineTokens, '=');

				if (lineTokens.size() == 2)
				{
					if (fooString == "info") //info
					{
						if (lineTokens[0] == "size")
						{
							try
							{
								fSize = std::stoi(lineTokens[1]);
							}
							catch (...)
							{
								fSize = 1;
							}
						}
					}
					else if (fooString == "common") //common info
					{
						if (lineTokens[0] == "lineHeight")
						{
							try
							{
								fLineHeight = std::stof(lineTokens[1]);
							}
							catch (...)
							{
								fLineHeight = 1;
							}
						}
						else if (lineTokens[0] == "base")
						{
							try
							{
								fBase = std::stof(lineTokens[1]);
							}
							catch (...)
							{
								fBase = 1;
							}
						}
						else if (lineTokens[0] == "scaleW")
						{
							try
							{
								fWidth = std::stoi(lineTokens[1]);
							}
							catch (...)
							{
								fWidth = 1;
							}
						}
						else if (lineTokens[0] == "scaleH")
						{
							try
							{
								fHeight = std::stoi(lineTokens[1]);
							}
							catch (...)
							{
								fHeight = 1;
							}
						}
					}
					else if (fooString == "chars") //character count
					{
						if (lineTokens[0] == "count")
						{
							try
							{
								fCharCount = std::stoi(lineTokens[1]);
							}
							catch (...)
							{
								fCharCount = 0;
							}
						}
					}
					else if (fooString == "char") //character
					{
						if (lineTokens[0] == "id")
						{
							try
							{
								cID = std::stoi(lineTokens[1]);
							}
							catch (...)
							{
								cID = 0;
							}
						}
						else if (lineTokens[0] == "x")
						{
							try
							{
								x = std::stoi(lineTokens[1]);
							}
							catch (...)
							{
								x = 1;
							}
						}
						else if (lineTokens[0] == "y")
						{
							try
							{
								y = std::stoi(lineTokens[1]);
							}
							catch (...)
							{
								y = 1;
							}
						}
						else if (lineTokens[0] == "width")
						{
							try
							{
								width = std::stoi(lineTokens[1]);
							}
							catch (...)
							{
								width = 1;
							}
						}
						else if (lineTokens[0] == "height")
						{
							try
							{
								height = std::stoi(lineTokens[1]);
							}
							catch (...)
							{
								height = 1;
							}
						}
						else if (lineTokens[0] == "xoffset")
						{
							try
							{
								xoffset = std::stoi(lineTokens[1]);
							}
							catch (...)
							{
								xoffset = 1;
							}
						}
						else if (lineTokens[0] == "yoffset")
						{
							try
							{
								yoffset = std::stoi(lineTokens[1]);
							}
							catch (...)
							{
								yoffset = 1;
							}
						}
						else if (lineTokens[0] == "xadvance")
						{
							try
							{
								advance = std::stoi(lineTokens[1]);
							}
							catch (...)
							{
								advance = 1;
							}
						}
					}
				}
				else if (lineTokens.size() == 1)
				{
					if (fooString == "info") //info
					{
						if (lineTokens[0] == "face" && i + 1 < tokenSize)
						{
							fFace = tokens[i + 1];
						}
					}
					else if (fooString == "page") //page
					{
						if (lineTokens[0] == "file" && i + 1 < tokenSize)
						{
							fAtlasPath.append("/" + tokens[i + 1]);
						}
					}
				}
			} //segment iteration

			if (fooString == "char")
			{
				//add FontChar
				if (cID >= 0 && cID < 128)
				{
					newFont->m_characters.insert(std::pair<char, FontChar>(cID, FontChar((float)x, (float)y, (float)width, (float)height, (float)xoffset, (float)yoffset, (float)advance, fWidth, fHeight, (float)fSize)));
				}
			}
		} //tokens > 1
	} //line iteration

	file.close();

	newFont->setAtlas(SOIL_load_OGL_texture(fAtlasPath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y)); //loads texture into OGL(openGL));
	newFont->bindAtlas();
	//sets texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	newFont->setAtlasDim(fWidth, fHeight);
	newFont->setBase((float)fBase);
	newFont->setFace(fFace);
	newFont->setLineHeight(fLineHeight);
	newFont->setSize(fSize);
	newFont->normalizeValues();

	return this->m_fontRef.addFont(newFont);
}

int Parser::parseDotAnim(std::string &filePath)
{
	std::ifstream file(filePath.c_str());

	if (!file.is_open()) //file not open properly
	{
		std::cerr << "file " << filePath << " not opened properly" << std::endl;
		return -1;
	}

	std::string currentLine = "";
	std::string processed = "";
	int tokenSize = 0;
	bool flag = false;
	int flaggedRow = 0;
	std::vector<std::string> tokens;
	std::vector<std::string> pathTokens;
	std::vector<std::string> lineTokens;
	tokens.reserve(3);
	pathTokens.reserve(3);
	lineTokens.reserve(3);

	Animation *currentAnimation = new Animation();

	std::string name;
	std::string textureDir;
	std::string audioDir;
	std::string atlasPath;
	std::string atlasName;
	std::string audioPath;
	std::string audioName;
	double totalTime = 0.0; //total animation time in seconds
	double defaultTimePerFrame = 0.0;
	int rows;
	int rowHeight;
	int pixelToWorld; //how many pixels are in 1.0f in world space
	int defaultFrameCount = 1;
	glm::ivec2 defaultFrameDimensions;
	glm::ivec2 atlasDimensions;

	int prFrameCount = -1; //pr stands for per-row
	std::vector<double> pfTime; //non-default time perframe
	std::vector<glm::ivec2> pfFrameDimensions;
	std::vector<bool> checklist; //true if row has been added, false if not

	while (std::getline(file, currentLine))
	{
		tokens.clear();
		pathTokens.clear();
		lineTokens.clear();

		this->ignoreDelimiterComment(currentLine, processed);
		currentLine = processed;

		this->splitToken(currentLine, tokens); //splits into tokens
		processed.clear();

		tokenSize = tokens.size();
		if (tokenSize > 1)
		{
			if (!flag) //not flagged
			{
				if (tokens[0] == "name")
				{
					if (tokenSize == 2)
					{
						name = tokens[1];
						int aniIndex = this->m_animationRef.search(name);
						if (aniIndex != -1) //already exists or name overlap
						{
							delete currentAnimation;
							currentAnimation = nullptr;
							file.close();
							return aniIndex;
						}
					}
				}
				else if (tokens[0] == "rootDir")
				{
					if (tokenSize == 2) //rootDir ~dir
					{
						this->removeFileNameFromPath(filePath, textureDir);
						textureDir.append("/" + tokens[1]);
						this->cleanPath(textureDir, processed);
						textureDir = processed;
						audioDir = processed;
						processed.clear();
					}
					else if (tokenSize == 3) //rootDir texture/audio ~dir
					{
						if (tokens[1] == "texture")
						{
							this->removeFileNameFromPath(filePath, textureDir);
							textureDir.append("/" + tokens[2]);
							this->cleanPath(textureDir, processed);
							textureDir = processed;
							processed.clear();
						}
						else if (tokens[1] == "audio")
						{
							this->removeFileNameFromPath(filePath, audioDir);
							audioDir.append("/" + tokens[2]);
							this->cleanPath(audioDir, processed);
							audioDir = processed;
							processed.clear();
						}
					}
				}
				else if (tokens[0] == "spriteSheet")
				{
					if (tokenSize == 2) //derived name
					{
						this->cleanPath(textureDir + "/" + tokens[1], atlasPath);
						this->splitToken(atlasPath, pathTokens, '/');
						this->splitToken(pathTokens[pathTokens.size() - 1], lineTokens, '.');
						atlasName = lineTokens[0];
						//this->checkTextureName(atlasName);
					}
					else if (tokenSize == 3)
					{
						this->cleanPath(textureDir + "/" + tokens[2], atlasPath);
						atlasName = tokens[1];
						//this->checkTextureName(atlasName);
					}
				}
				else if (tokens[0] == "afile")
				{
					if (tokenSize == 2)
					{
						this->cleanPath(audioDir + "/" + tokens[1], audioPath);
					}
				}
				else if (tokens[0] == "rows")
				{
					if (tokenSize == 2)
					{
						try
						{
							rows = std::stoi(tokens[1]);
						}
						catch (...)
						{
							rows = 0;
						}
						checklist.resize(rows);
						for (int i = 0; i < rows; i++)
						{
							checklist[i] = false;
						}
						currentAnimation->setRows(rows);
					}
				}
				else if (tokens[0] == "ptw")
				{
					if (tokenSize == 2)
					{
						try
						{
							pixelToWorld = std::stoi(tokens[1]);
						}
						catch (...)
						{
							pixelToWorld = 1;
						}
					}
				}
				else if (tokens[0] == "rowHeight")
				{
					if (tokenSize == 2)
					{
						try
						{
							rowHeight = std::stoi(tokens[1]);
						}
						catch (...)
						{
							rowHeight = 1;
						}
					}
				}
				else if (tokens[0] == "atlasDim")
				{
					if (tokenSize == 2)
					{
						try
						{
							atlasDimensions.x = std::stoi(tokens[1]);
							atlasDimensions.y = std::stoi(tokens[1]);
						}
						catch (...)
						{
							atlasDimensions.x = 1;
							atlasDimensions.y = 1;
						}
					}
					else if (tokenSize == 3)
					{
						try
						{
							atlasDimensions.x = std::stoi(tokens[1]);
							atlasDimensions.y = std::stoi(tokens[2]);
						}
						catch (...)
						{
							atlasDimensions.x = 1;
							atlasDimensions.y = 1;
						}
					}
				}
				else if (tokens[0] == "sameFrameHeight")
				{
					if (tokenSize == 2)
					{
						try
						{
							defaultFrameDimensions.y = std::stoi(tokens[1]);
						}
						catch (...)
						{
							defaultFrameDimensions.y = 1;
						}
					}
				}
				else if (tokens[0] == "sameFrameWidth")
				{
					if (tokenSize == 2)
					{
						try
						{
							defaultFrameDimensions.x = std::stoi(tokens[1]);
						}
						catch (...)
						{
							defaultFrameDimensions.x = 1;
						}
					}
				}
				else if (tokens[0] == "totalTime")
				{
					if (tokenSize == 2)
					{
						try
						{
							totalTime = std::stod(tokens[1]);
						}
						catch (...)
						{
							totalTime = 0.0;
						}
						defaultTimePerFrame = totalTime / (double)defaultFrameCount;
						currentAnimation->setTotalTime(totalTime);
					}
				}
				else if (tokens[0] == "sameFrameCount")
				{
					if (tokenSize == 2)
					{
						try
						{
							defaultFrameCount = std::stoi(tokens[1]);
						}
						catch (...)
						{
							defaultFrameCount = 1;
						}
						defaultTimePerFrame = totalTime / (double)defaultFrameCount;
					}
				}
				else if (tokens[0] == "row")
				{
					if (tokenSize == 2)
					{
						flag = true;
						try
						{
							flaggedRow = std::stoi(tokens[1]);
						}
						catch (...)
						{
							flaggedRow = 0;
						}
						checklist[flaggedRow] = true;
						prFrameCount = -1;
						pfFrameDimensions.clear();
						pfTime.clear();
					}
				}
			}
			else //flagged
			{
				if (tokens[0] == "frameCount")
				{
					if (tokenSize == 2)
					{
						try
						{
							prFrameCount = std::stoi(tokens[1]);
						}
						catch (...)
						{
							prFrameCount = 1;
						}
						pfFrameDimensions.resize(prFrameCount);
						pfTime.resize(prFrameCount);

						for (int i = 0; i < prFrameCount; i++)
						{
							pfFrameDimensions[i] = defaultFrameDimensions;
							pfTime[i] = defaultTimePerFrame;
						}
					}
				}
				else if (tokens[0] == "frameWidth")
				{
					if (tokenSize == 2) //all
					{
						int width;
						try
						{
							width = std::stoi(tokens[1]);
						}
						catch (...)
						{
							width = 1;
						}
						for (int i = 0; i < pfFrameDimensions.size(); i++)
						{
							pfFrameDimensions[i].x = width;
						}
					}
					else if (tokenSize == 3) //single frame
					{
						int index;
						try
						{
							index = std::stoi(tokens[1]);
						}
						catch (...)
						{
							index = 1;
						}
						if (index >= 0 && index < prFrameCount)
						{
							try
							{
								pfFrameDimensions[index].x = std::stoi(tokens[1]);
							}
							catch (...)
							{
								pfFrameDimensions[index].x = 1;
							}
						}
					}
				}
				else if (tokens[0] == "frameHeight")
				{
					if (tokenSize == 2) //all
					{
						int height;
						try
						{
							height = std::stoi(tokens[1]);
						}
						catch (...)
						{
							height = 1;
						}
						for (int i = 0; i < pfFrameDimensions.size(); i++)
						{
							pfFrameDimensions[i].y = height;
						}
					}
					else if (tokenSize == 3) //single frame
					{
						int index;
						try
						{
							index = std::stoi(tokens[1]);
						}
						catch (...)
						{
							index = 1;
						}
						if (index >= 0 && index < prFrameCount)
						{
							try
							{
								pfFrameDimensions[index].y = std::stoi(tokens[1]);
							}
							catch (...)
							{
								pfFrameDimensions[index].y = 1;
							}
						}
					}
				}
				else if (tokens[0] == "frameTime")
				{
					if (tokenSize == 3) //only single frame changes
					{
						int index;
						try
						{
							index = std::stoi(tokens[1]);
						}
						catch (...)
						{
							index = 1;
						}
						if (index >= 0 && index < prFrameCount)
						{
							try
							{
								pfTime[index] = std::stoi(tokens[1]);
							}
							catch (...)
							{
								pfTime[index] = 1;
							}
						}
					}
				}
				if (tokens[0] == "close")
				{
					double timeFrame = 0.0;
					glm::ivec2 cursor;
					cursor.x = 0;
					cursor.y = atlasDimensions.y - (flaggedRow * rowHeight);
					for (int i = 0; i < prFrameCount; i++)
					{
						currentAnimation->addFrame(flaggedRow, cursor, pfFrameDimensions[i], atlasDimensions, pixelToWorld, timeFrame, pfTime[i]);
						cursor.x += pfFrameDimensions[i].x;
						timeFrame += pfTime[i];
					}

					flag = false;
				}
			}
		} //end anti-comment check
	} //per line iteration

	for (int count = 0; count < rows; count++)
	{
		if (!checklist[count])
		{
			double timeFrame = 0.0;
			glm::ivec2 cursor;
			cursor.x = 0;
			cursor.y = atlasDimensions.y - (count * rowHeight);
			for (int i = 0; i < defaultFrameCount; i++)
			{
				currentAnimation->addFrame(count, cursor, defaultFrameDimensions, atlasDimensions, pixelToWorld, timeFrame, defaultTimePerFrame);
				cursor.x += defaultFrameDimensions.x;
				timeFrame += defaultTimePerFrame;
			}
		}
	}

	currentAnimation->setName(name);
	currentAnimation->setAtlas(this->m_textureRef.create(atlasName, atlasPath));

	return this->m_animationRef.addAnimation(currentAnimation);
}

Parser::~Parser()
{

}
*/



/*

end Main Parser

*/