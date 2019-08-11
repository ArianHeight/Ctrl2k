#include <Ctrl 2kPCH.h>

/*

StatusBar object

*/

//needed function
void drawTexture(int textureIndex, glm::dvec2 topLeft, glm::dvec2 bottomRight, glm::vec4 tintColour);

StatusBar::StatusBar(std::string name, glm::vec2 topLeft, glm::vec2 bottomRight, int textureIndex, glm::vec2 fillerTL, glm::vec2 fillerBR, int fillerTexture, double percentage, glm::vec4 colour) :
	Widget(name, topLeft, bottomRight, textureIndex), m_fillerPosTL(fillerTL), m_fillerPosBR(fillerBR), m_textureFillerIndex(fillerTexture), m_displayPercentage(percentage), m_colour(colour)
{
	//cstr
	this->m_displayPercentage = glm::clamp(this->m_displayPercentage, 0.0, 1.0);

	this->update();
}

void StatusBar::render()
{
	Widget::render(); //draws the bar background

	//drawing filler
	glm::vec2 tl = this->m_fillerPosTLScreen;
	glm::vec2 br = this->m_fillerPosBRScreen;

	//assumes increase to right for now
	br.x = tl.x + (br.x - tl.x) * this->m_displayPercentage;

	drawTexture(this->m_textureFillerIndex, tl, br, this->m_colour);
}

//updates the filler's positions in screen coords
void StatusBar::update()
{
	Widget::update();

	std::string tempName = "PosScreen";
	Component* posScreen = this->getComponent(tempName); //grabs the parent's component and uses the position values from that
	FourPoints* hb = posScreen->getPhysicsObject()->getHB();
	glm::vec2 sizeRatio = glm::abs(hb->tl - hb->br) / 2.0f;

	this->m_fillerPosTLScreen = hb->tl - (glm::vec2(-1, 1) - this->m_fillerPosTL) * sizeRatio;
	this->m_fillerPosBRScreen = hb->br - (glm::vec2(1, -1) - this->m_fillerPosBR) * sizeRatio;
}

/*

accessors

*/

glm::vec2 StatusBar::getFillerTL()
{
	return this->m_fillerPosTL;
}

glm::vec2 StatusBar::getFillerBR()
{
	return this->m_fillerPosBR;
}

glm::vec4 StatusBar::getColour()
{
	return this->m_colour;
}

int StatusBar::getFillerTextureIndex()
{
	return this->m_textureFillerIndex;
}

double StatusBar::getDisplayPercentage()
{
	return this->m_displayPercentage;
}

/*

mutators

*/

void StatusBar::setFillerTL(glm::vec2 pos)
{
	this->m_fillerPosTL = pos;
	this->update();
}

void StatusBar::setFillerBR(glm::vec2 pos)
{
	this->m_fillerPosBR = pos;
	this->update();
}

void StatusBar::setColour(glm::vec4 newColour)
{
	this->m_colour = newColour;
}

void StatusBar::addColour(glm::vec4 addingColour)
{
	this->m_colour += addingColour;
}

void StatusBar::setPercentage(double value)
{
	this->m_displayPercentage = glm::clamp(value, 0.0, 1.0);
}

StatusBar::~StatusBar()
{
	//empty dstr
}

/*

end StatusBar object

*/