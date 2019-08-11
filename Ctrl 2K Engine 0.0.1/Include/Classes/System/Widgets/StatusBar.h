#pragma once

/*

Status Bar object which shows a bar on screen

*/

//remember to call StatusBar::updateFillerScreenPos() everytime any position is changed
class StatusBar : public Widget
{
public:
	StatusBar(std::string name, glm::vec2 topLeft, glm::vec2 bottomRight, int textureIndex,
			  glm::vec2 fillerTL, glm::vec2 fillerBR, int fillerTexture, double percentage = 1.0, glm::vec4 colour = glm::vec4(1.0f)); //first 3 used for superclass cstr
	virtual ~StatusBar();

private:
	glm::vec2 m_fillerPosTL; //the bar filler's topleft pos in widget coords(relative to the bar's coordinates, topleft corner of the bar is -1, 1 etc...)
	glm::vec2 m_fillerPosBR; //the bar filler's bottomright pos in widget coords
	glm::vec2 m_fillerPosTLScreen; //the bar filler's topleft pos in screen coords
	glm::vec2 m_fillerPosBRScreen; //bottomright pos in screen coords

	glm::vec4 m_colour; //the colour the bar is to be filled with
	int m_textureFillerIndex; //the texture index of the filler

	double m_displayPercentage; //will be clamped to 0 and 1

public:
	void render(); //draws widget onto screen
	void update(); //updates all screen coordinates

	//accessors
	glm::vec2 getFillerTL();
	glm::vec2 getFillerBR();
	glm::vec4 getColour();
	int getFillerTextureIndex();
	double getDisplayPercentage();

	//mutators
	void setFillerTL(glm::vec2 pos);
	void setFillerBR(glm::vec2 pos);
	void setColour(glm::vec4 newColour);
	void addColour(glm::vec4 addingColour); //could be used to subtract as well
	void setPercentage(double value);
};

/*

end status bar object

*/