#pragma once

/*

a Button that appears on screen

*/

class Button : public Widget
{
public:
	Button(std::string name, glm::vec2 topLeft, glm::vec2 bottomRight, int textureIndex, bool &activation);
	virtual ~Button();

private:
	bool m_pressed; //whether or not button is active
	bool m_lastPressed{false}; //whether or not button was active last frame
	bool &m_activation; //ref to a var that will turn on for an actual signal

	int m_textureBufferPressedIndex; //texture index of the pressed button texture

public:
	void update(); //updates all data
	void render(); //draws widget onto screen
};

/*

end button object

*/