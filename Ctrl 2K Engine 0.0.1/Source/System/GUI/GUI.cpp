#include <Ctrl 2kPCH.h>

/*

Game User Interface

*/

GUI::GUI(std::string name, glm::vec2 topLeft, glm::vec2 bottomRight, int textureIndex) :
	Widget(name, topLeft, bottomRight, textureIndex)
{
	//cstr
	this->update();
}

void GUI::setTarget(GameObject *obj)
{
	//cleans up for last target
	if (this->targetObj != nullptr && !this->targetObj->getDelete())
	{
		this->targetObj->setLinkHud(nullptr);
	}

	this->targetObj = obj;
	if (this->targetObj != nullptr) //ptr guard
	{
		this->targetObj->setLinkHud(this); //links camera to object
	}
}

void GUI::render()
{
	Widget::render();

	for (int i = 0; i < this->m_children.size(); i++)
	{
		this->m_children[i]->render(); //render calls on all children
	}
}

void GUI::update()
{
	Widget::update();

	if (this->targetObj != nullptr && this->targetObj->getDelete()) //set ptr to nullptr if obj is getting deleted
	{
		this->targetObj = nullptr;
	}
}

std::vector<GameObject*>& GUI::getChildren()
{
	return this->m_children;
}

GameObject* GUI::getTarget()
{
	return this->targetObj;
}

GUI::~GUI()
{
	//dstr
}

/*

end GUI object

*/