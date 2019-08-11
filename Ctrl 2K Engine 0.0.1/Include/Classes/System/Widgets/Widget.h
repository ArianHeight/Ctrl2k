#pragma once

/*

a widget that appears on screen

*/

class Widget : public GameObject
{
public:
	Widget(std::string name, glm::vec2 topLeft, glm::vec2 bottomRight, int textureIndex = -1);
	virtual ~Widget(); //destroys all children classes

protected:
	std::string m_name;

	glm::vec2 m_posTLParent; //topleft corner in parent space
	glm::vec2 m_posBRParent; //bottom right corner in parent space

	int m_textureBufferIndex; //background texture of widget

public:
	virtual void render(); //draws widget onto screen
	virtual void update(); //updates the screen pos of this widget and the screen pos of all its children

	//accessors
	glm::vec2 getTopLeftPos();
	glm::vec2 getBottomRightPos();
	int getTextureIndex();
	std::string getName();

	//mutators
	void setTopLeftPos(glm::vec2 pos); //changes the tl pos in parent space
	void setBottomRightPos(glm::vec2 pos); //changes the br pos in parent space
	void setTextureIndex(int index);
	void setName(std::string name);
	void setNullParent();
};

/*

end widget

*/