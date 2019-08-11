#include <Ctrl 2kPCH.h>

/*

clickable buttons!

*/

Button::Button(std::string name, glm::vec2 topLeft, glm::vec2 bottomRight, int textureIndex, bool &activation) :
	Widget(name, topLeft, bottomRight, textureIndex), m_activation(activation)
{
	//cstr
	this->getComponent(std::string("PosScreen"))->enableMCollision(); //enables collision with the mouse
}

void Button::update()
{
	Widget::update();

	for (int i = 0; i < this->m_inputs.size(); i++) //goes through input list
	{
		if (this->m_inputs[i] == "MOUSE_HIT")
		{
			this->m_pressed = true;
		}
	}

	if (this->m_pressed && !this->m_lastPressed) //outputs signal
	{
		this->m_activation = true;
	}
	else //closes signal
	{
		this->m_activation = false;
	}

	this->m_lastPressed = this->m_pressed; //resets m_pressed and moves the previous value to m_lastPressed
	this->m_pressed = false;
}

void Button::render()
{
	Widget::render();
}

Button::~Button()
{
	//dstr
}

/*

end Button class

*/