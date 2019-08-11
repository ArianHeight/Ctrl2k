#include <Ctrl 2kPCH.h>

//helper declarations
template <typename T>
bool inVectorAndRemove(T in, std::vector<T>& theList);

/*
Player class
*/

Player::Player(double &time, std::string &name, PhysObject *hitbox, glm::vec2 position, float speedOne, float speedTwo, float speedThree) :
	Entity(time, name, hitbox, position, PLAYABLE, speedOne, speedTwo, speedThree)
{
	//cstr
}

void Player::attack()
{
	std::string n = "Position";
	Component *pos = this->getComponent(n);

	this->addChild(new MeleeDmgArea(this, this->m_timeRef,
		new DmgValue(27, 0.05f, 2.0f, 0.0f, 0.0f, 0.05f, 0.06f),
		new PhysComponent(new RectangleHB(glm::vec2(0.0f), glm::vec2(3.0f))),
		new VisualData(ANIMATION, 4, glm::vec2(1.0f)),
		new Transformation(pos->getDisplacement() + pos->getDirNormals(), 1.0f, pos->getDirection())));
}

void Player::usePlayer()
{
	this->m_type = CURRENT_PLAYER;
}

void Player::stopPlayer()
{
	this->m_type = PLAYABLE;
}

void Player::update()
{
	Entity::update();
}

void Player::processInput()
{
	Entity::processInput();

	bool atk = (inVectorAndRemove(std::string("SWING"), this->m_inputs));
	if (atk)
	{
		this->attack();
	}
}

Player::~Player()
{
	//empty dstr
}

/*
end Player class
*/