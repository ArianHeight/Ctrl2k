#pragma once

/*

Text object which appears on screen

*/

class Text : public Widget
{
public:
	Text(std::string name, FontManager &fonts, glm::vec2 &spxw, int index, std::string &str, glm::vec2 topLeft, glm::vec2 bottomRight, int orientation = 0, float size = 64.0f, glm::vec4 colour = glm::vec4(1.0f));
	virtual ~Text();

private:
	FontManager &source;
	glm::vec2 &singlePxToWindow;

	int m_textOrientation; //0 left, 1 centered, 2 right
	glm::vec2 m_centeredPosScreen; //centered position in screen pos

	std::string m_text; //the text to be displayed
	glm::vec4 m_colour; //text colour
	float m_textSize; //text size, 1 is 64 pixels in a 1280x720 but most text aren't that large
	int m_fontIndex; //the font

public:
	void render(); //draws widget onto screen

	//accessors
	glm::vec4 getColour();
	glm::vec2 getCenteredPos();
	std::string getString();
	int getOrientation();
	float getTextSize();
	int getFontIndex();

	//mutators
	void setColour(glm::vec4 colour);
	void recalculateCenter();
	void setString(std::string str);
	void setOrientation(int orientation);
	void setSize(float size);
	void setFontIndex(int index);
};

/*

end Text object

*/