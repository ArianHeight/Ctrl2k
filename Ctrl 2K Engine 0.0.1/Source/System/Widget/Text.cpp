#include <Ctrl 2kPCH.h>

/*

Text object

*/

//functions needed
void drawStringInBox(int fontIndex, std::string& string, glm::dvec2 cursor, double width, float size, glm::vec4 colour);
void drawString(int fontIndex, std::string &string, glm::dvec2 cursor, float size, glm::vec4 colour);

Text::Text(std::string name, FontManager &fonts, glm::vec2 &spxw, int index, std::string &str, glm::vec2 topLeft, glm::vec2 bottomRight, int orientation, float size, glm::vec4 colour) :
	source(fonts), singlePxToWindow(spxw), Widget(name, topLeft, bottomRight, -1), m_fontIndex(index), 
	m_text(str), m_textOrientation(orientation), m_textSize(size), m_colour(colour)
{
	//cstr
	if (this->m_textOrientation == 1)
	{
		this->recalculateCenter();
	}
}

void Text::render()
{
	std::string tempName = "PosScreen";
	Component* posScreen = this->getComponent(tempName); //grabs the parent's component and uses the position values from that
	FourPoints* hb = posScreen->getPhysicsObject()->getHB();

	if (this->m_textOrientation == 0) //left
	{
		drawStringInBox(this->m_fontIndex, this->m_text, hb->tl, hb->br.x - hb->tl.x, this->m_textSize, this->m_colour);
	}
	else if (this->m_textOrientation == 1) //center
	{
		Font *selectFont = source.data(this->m_fontIndex);

		glm::dvec2 cursor(hb->tl);
		double lineR = hb->br.x; //rightmost boundary
		std::string temp = ""; //temporary string used to hold line being printed

		for (char c : this->m_text)
		{
			FontChar character = selectFont->m_characters.at(c);

			temp += c;
			cursor.x += character.m_advance * this->m_textSize * this->singlePxToWindow.x;

			if (cursor.x > lineR)
			{
				if (c == ' ')
				{
					cursor.x -= character.m_advance * this->m_textSize * this->singlePxToWindow.x; //undo the cursor move of the ' ' character
																								   //gets width of the line offset (lineR - cursor.x) and adds it to the left most boundary for drawing
					drawString(this->m_fontIndex, temp, glm::dvec2(this->m_centeredPosScreen.x - ((cursor.x - hb->tl.x) / 2.0), cursor.y), this->m_textSize, this->m_colour);

					//resets some values
					cursor.x = hb->tl.x;
					cursor.y -= this->m_textSize * this->singlePxToWindow.y;
					temp.clear();
				}
			}
		}

		if (temp != "")
		{
			drawString(this->m_fontIndex, temp, glm::dvec2(this->m_centeredPosScreen.x - ((cursor.x - hb->tl.x) / 2.0), cursor.y), this->m_textSize, this->m_colour);
		}
	}
	else //right
	{
		Font *selectFont = source.data(this->m_fontIndex);
		
		glm::dvec2 cursor(hb->tl); 
		double lineR = hb->br.x; //rightmost boundary
		std::string temp = ""; //temporary string used to hold line being printed

		for (char c : this->m_text)
		{
			FontChar character = selectFont->m_characters.at(c);

			temp += c;
			cursor.x += character.m_advance * this->m_textSize * this->singlePxToWindow.x;

			if (cursor.x > lineR)
			{
				if (c == ' ')
				{
					cursor.x -= character.m_advance * this->m_textSize * this->singlePxToWindow.x; //undo the cursor move of the ' ' character
					//gets width of the line offset (lineR - cursor.x) and adds it to the left most boundary for drawing
					drawString(this->m_fontIndex, temp, glm::dvec2(hb->br.x - cursor.x + hb->tl.x, cursor.y), this->m_textSize, this->m_colour);

					//resets some values
					cursor.x = hb->tl.x;
					cursor.y -= this->m_textSize * this->singlePxToWindow.y;
					temp.clear();
				}
			}
		}

		if (temp != "")
		{
			drawString(this->m_fontIndex, temp, glm::dvec2(hb->br.x - cursor.x + hb->tl.x, cursor.y), this->m_textSize, this->m_colour);
		}
	}
}

/*

accessors

*/

glm::vec4 Text::getColour()
{
	return this->m_colour;
}

glm::vec2 Text::getCenteredPos()
{
	return this->m_centeredPosScreen;
}

std::string Text::getString()
{
	return this->m_text;
}

int Text::getOrientation()
{
	return this->m_textOrientation;
}

float Text::getTextSize()
{
	return this->m_textSize;
}

int Text::getFontIndex()
{
	return this->m_fontIndex;
}

/*

mutators

*/

void Text::setColour(glm::vec4 colour)
{
	this->m_colour = colour;
}

void Text::recalculateCenter()
{
	std::string tempName = "PosScreen";
	Component* posScreen = this->getComponent(tempName); //grabs the parent's component and uses the position values from that
	FourPoints* hb = posScreen->getPhysicsObject()->getHB();

	this->m_centeredPosScreen = (hb->tl + hb->br) / 2.0f;
}

void Text::setString(std::string str)
{
	this->m_text = str;
}

void Text::setOrientation(int orientation)
{
	this->m_textOrientation = orientation;

	if (this->m_textOrientation == 1) //center
	{
		this->recalculateCenter();
	}
}

void Text::setSize(float size)
{
	this->m_textSize = size;
}

void Text::setFontIndex(int index)
{
	this->m_fontIndex = index;
}

Text::~Text()
{
	//dstr
}

/*

end Text object

*/