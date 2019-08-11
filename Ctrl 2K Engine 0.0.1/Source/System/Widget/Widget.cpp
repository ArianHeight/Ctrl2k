#include <Ctrl 2kPCH.h>

/*

Widget object

*/

//needed function
void drawTexture(int textureIndex, glm::dvec2 topLeft, glm::dvec2 bottomRight, glm::vec4 tintColour);

Widget::Widget(std::string name, glm::vec2 topLeft, glm::vec2 bottomRight, int textureIndex) :
	m_name(name), m_posTLParent(topLeft), m_posBRParent(bottomRight), m_textureBufferIndex(textureIndex)
{
	//cstr
	std::string temp = ""; //temporary var for names

	temp = "PosScreen"; //screen position
	FourPoints hb;
	hb.tl = this->m_posTLParent;
	hb.br = this->m_posBRParent;
	hb.tr.x = hb.br.x;
	hb.tr.y = hb.tl.y;
	hb.bl.x = hb.tl.x;
	hb.bl.y = hb.br.y;
	this->addComponent(temp, new PhysComponent(new RectangleHB(hb), true, false));

	this->update();
}

//draws the widget to screen
void Widget::render()
{
	if (this->m_textureBufferIndex != -1)
	{
		std::string tempName = "PosScreen";
		FourPoints* hb = this->getComponent(tempName)->getPhysicsObject()->getHB();
		this->game->getEngine().getRenderEngine().drawTextureN(this->m_textureBufferIndex, hb->tl, hb->br/*, glm::vec4(1.0f)*/);
		//drawTexture(this->m_textureBufferIndex, hb->tl, hb->br, glm::vec4(1.0f));
	}
}

void Widget::update()
{
	std::string tempName = "PosScreen";
	if (this->m_parent != nullptr)
	{
		if (this->m_parent->checkForComponent(tempName)) //if this is not in the parent
		{
			Component* posScreen = this->m_parent->getComponent(tempName); //grabs the parent's component and uses the position values from that
			FourPoints* hbParent = posScreen->getPhysicsObject()->getHB();
			glm::vec2 sizeRatio = glm::abs(hbParent->tl - hbParent->br) / 2.0f;

			Component* thisScreen = this->getComponent(tempName); //the position component of this object
			FourPoints* thisHB = thisScreen->getPhysicsObject()->getHB();

			thisHB->tl = hbParent->tl - (glm::vec2(-1, 1) - this->m_posTLParent) * sizeRatio; //scales and updates the positions of the widget
			thisHB->br = hbParent->br - (glm::vec2(1, -1) - this->m_posBRParent) * sizeRatio;
			thisHB->tr.x = thisHB->br.x;
			thisHB->tr.y = thisHB->tl.y;
			thisHB->bl.x = thisHB->tl.x;
			thisHB->bl.y = thisHB->br.y;
		}
	}

	for (int i = 0; i < this->m_children.size(); i++)
	{
		this->m_children[i]->update();
	}

	GameObject::update(); //calls the original update()
}

/*

accessors

*/

glm::vec2 Widget::getTopLeftPos()
{
	return this->m_parent->getComponent(std::string("PosScreen"))->getPhysicsObject()->getHB()->tl;
}

glm::vec2 Widget::getBottomRightPos()
{
	return this->m_parent->getComponent(std::string("PosScreen"))->getPhysicsObject()->getHB()->br;
}

int Widget::getTextureIndex()
{
	return this->m_textureBufferIndex;
}

std::string Widget::getName()
{
	return this->m_name;
}

/*

mutators

*/

void Widget::setTopLeftPos(glm::vec2 pos)
{
	this->m_posTLParent = pos;
	this->update();
}

void Widget::setBottomRightPos(glm::vec2 pos)
{
	this->m_posBRParent = pos;
	this->update();
}

void Widget::setTextureIndex(int index)
{
	this->m_textureBufferIndex = index;
}

void Widget::setName(std::string name)
{
	this->m_name = name;
}

void Widget::setNullParent() //****IMPORTANT! DOES NOT DELETE AND DEALLOCATE PARENT WIDGET'S MEM, ONLY SETS POINTER TO NULL!!!!!
{
	this->m_parent = nullptr;
}

Widget::~Widget()
{
	//dstr - cleanup all done in GameObject::~GameObject()
}

/*

end Widget object

*/