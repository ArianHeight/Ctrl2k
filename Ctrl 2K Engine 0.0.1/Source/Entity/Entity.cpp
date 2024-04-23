#include <Ctrl 2kPCH.h>

/*
Entity superclass
*/

//helper functions
template <typename T>
bool inVector(T in, std::vector<T>& theList);

template <typename T>
bool inVectorAndRemove(T in, std::vector<T>& theList)
{
	int i = theList.size();
	for (int index = 0; index < i; index++)
	{
		if (in == theList[index])
		{
			theList.erase(theList.begin() + index);
			return true;
		}
	}
	return false;
}

//converts Orientation struct into a vec2
glm::vec2 orientationToVecTwo(Orientation dir)
{
	float hypotenuse = 0.7071067811865475f;
	switch (dir)
	{
	case UP:
		return glm::vec2(0.0f, 1.0f);
	case DOWN:
		return glm::vec2(0.0f, -1.0f);
	case LEFT:
		return glm::vec2(-1.0f, 0.0f);
	case RIGHT:
		return glm::vec2(1.0f, 0.0f);
	case UPPER_LEFT:
		return glm::vec2(-hypotenuse, hypotenuse);
	case UPPER_RIGHT:
		return glm::vec2(hypotenuse, hypotenuse);
	case LOWER_LEFT:
		return glm::vec2(-hypotenuse, -hypotenuse);
	case LOWER_RIGHT:
		return glm::vec2(hypotenuse, -hypotenuse);
	}
	return glm::vec2(0.0f);
}

Entity::Entity(double &time, const std::string &name, PhysObject *hitbox, glm::vec2 position, EntityType type, float speedOne, float speedTwo, float speedThree) :
	m_name(name), m_timeRef(time), m_type(type)
{
	//adds components
	std::string tempName = "Position";
	Component *transform = this->getComponent(0); //ptr to transformation component
	transform->setDisp(position);

	//tempName = "Mainbody"; 3
	this->m_mainbody = new PhysComponent(hitbox); //ptr to hitbox
	this->addComponent(this->m_mainbody, true);

	//tempName = "Statistics"; 4
	this->m_statistics = new EntityData(name, 100.0f, 100.0f, 100.0f, true, 2.0f, 1.0f, 1.0f);
	this->addComponent(this->m_statistics, true); //adds it to components

	//tempName = "Movement"; 5
	this->m_movement = new MoveData(speedOne, speedTwo, speedThree);
	this->addComponent(this->m_movement, true);

	//adds properties
	tempName = "CollisionOn";
	this->addProperty(tempName);
	
}

void Entity::updateOrientation(glm::vec2 &newDir)
{
	Component* transform = this->m_components[0];

	if (newDir.x > 0.7) //right side
	{
		if (newDir.x < 0.85) //one of the slants
		{
			if (newDir.y > 0) //upright
			{
				transform->setDirection(UPPER_RIGHT);
			}
			else //lowright
			{
				transform->setDirection(LOWER_RIGHT);
			}
		}
		else //straight right
		{
			transform->setDirection(RIGHT);
		}
	}
	else if (newDir.x < -0.7) //left side
	{
		if (newDir.x > -0.85) //one of the slants
		{
			if (newDir.y > 0) //upleft
			{
				transform->setDirection(UPPER_LEFT);
			}
			else //lowleft
			{
				transform->setDirection(LOWER_LEFT);
			}
		}
		else //straight left
		{
			transform->setDirection(LEFT);
		}
	}
	else //up or down
	{
		if (newDir.y > 0) //up
		{
			transform->setDirection(UP);
		}
		else //down
		{
			transform->setDirection(DOWN);
		}
	}
}

void Entity::move(Orientation newDir)
{
	this->move(orientationToVecTwo(newDir));
}

void Entity::move(glm::vec2 direction)
{
	Component *transform = this->getComponent(0);

	//updates orientation
	this->updateOrientation(direction);

	if ((this->m_collidingSide[0] && direction.y > 0) || (this->m_collidingSide[1] && direction.y < 0))
	{
		direction.y = 0;
	}
	if ((this->m_collidingSide[2] && direction.x < 0) || (this->m_collidingSide[3] && direction.x > 0))
	{
		direction.x = 0;
	}

	//moves entity
	transform->changeDisp(direction * this->m_movement->getSpeed() * (float)this->m_timeRef);
	this->m_lastMove = direction * this->m_movement->getSpeed() * (float)this->m_timeRef;

	//for sides touching
	if (direction.x > 0)
	{
		this->m_collidingSide[2] = false;
		this->m_collidingSide[0] = false;
		this->m_collidingSide[1] = false;
	}
	else if (direction.x < 0)
	{
		this->m_collidingSide[3] = false;
		this->m_collidingSide[0] = false;
		this->m_collidingSide[1] = false;
	}
	if (direction.y > 0)
	{
		this->m_collidingSide[1] = false;
		this->m_collidingSide[2] = false;
		this->m_collidingSide[3] = false;
	}
	else if (direction.y < 0)
	{
		this->m_collidingSide[0] = false;
		this->m_collidingSide[2] = false;
		this->m_collidingSide[3] = false;
	}

	this->m_moved = true;
}

void Entity::processInput()
{
	//for movement
	bool up = (inVectorAndRemove(std::string("MOVE_UP"), this->m_inputs));
	bool down = (inVectorAndRemove(std::string("MOVE_DOWN"), this->m_inputs));
	bool left = (inVectorAndRemove(std::string("MOVE_LEFT"), this->m_inputs));
	bool right = (inVectorAndRemove(std::string("MOVE_RIGHT"), this->m_inputs));
	int directions = int(up) + int(down) + int(left) + int(right);
	
	if (directions == 4 || directions == 0) //animation stuff
	{
		if (this->m_activeComponents[VISUAL_DATA][0] != 6) //IDLE
		{
			disableComponent(m_activeComponents[VISUAL_DATA][0]);
			activateComponent(6); //IDLE
		}
	}
	else
	{
		if (this->m_activeComponents[VISUAL_DATA][0] != 8) //WALKING
		{
			disableComponent(m_activeComponents[VISUAL_DATA][0]);
			activateComponent(8); //WALKING
		}
	}

	if (directions == 3) //4 means wasd all pressed, no movement
	{
		if (!up) //down
		{
			this->move(DOWN);
		}
		else if (!down) //up
		{
			this->move(UP);
		}
		else if (!left) //right
		{
			this->move(RIGHT);
		}
		else //left
		{
			this->move(LEFT);
		}
	}
	else if (directions == 2) //for diagonal movements only
	{
		if (up && left)
		{
			this->move(UPPER_LEFT);
		}
		else if (up && right)
		{
			this->move(UPPER_RIGHT);
		}
		else if (down && left)
		{
			this->move(LOWER_LEFT);
		}
		else if (down && right)
		{
			this->move(LOWER_RIGHT);
		}
	}
	else if (directions == 1) //only one direction
	{
		if (up)
		{
			this->move(UP);
		}
		else if (down)
		{
			this->move(DOWN);
		}
		else if (left)
		{
			this->move(LEFT);
		}
		else if (right)
		{
			this->move(RIGHT);
		}
	}
}

void Entity::engineMove(glm::vec3 distance)
{
	Component *transform = this->getComponent(0); //ptr to transformation component

	transform->changeDisp(glm::vec2(distance.x, distance.y));
	/*
	if (distance.x > 0)
	{
		this->m_collidingSide[2] = true;
	}
	else if (distance.x < 0)
	{
		this->m_collidingSide[3] = true;
	}
	if (distance.y > 0)
	{
		this->m_collidingSide[1] = true;
	}
	else if (distance.y < 0)
	{
		this->m_collidingSide[0] = true;
	}
	*/
}

void Entity::engineMove(Manifest distance)
{
	Component *transform = this->getComponent(0); //ptr to transformation component

	transform->changeDisp(distance.magnitude * distance.normal * this->m_movement->getSpeed() * (float)this->m_timeRef);
	/*
	if (distance.x > 0)
	{
	this->m_collidingSide[2] = true;
	}
	else if (distance.x < 0)
	{
	this->m_collidingSide[3] = true;
	}
	if (distance.y > 0)
	{
	this->m_collidingSide[1] = true;
	}
	else if (distance.y < 0)
	{
	this->m_collidingSide[0] = true;
	}
	*/
}

bool Entity::updateHealth() //false for dead, true for alive
{
	Component *status = this->getComponent(4);

	//health gate to death
	if (status->getHealth() < 0.0f)
	{
		status->setDeathState(true);
		disableComponent(m_activeComponents[VISUAL_DATA][0]);
		activateComponent(7); //DEATH
		return false;
	}

	return true;
}

void Entity::update()
{
	Component *transform = this->getComponent(0); //ptr to transformation component

	this->updateLastPos();
	this->updateTimers();
	//this->updateAggro(this->game->getPlayer());

	if (this->updateHealth())
	{
		this->updateAI();
		this->m_movedLastFrame = this->m_moved;
		this->m_moved = false;

		this->processInput();
	}

	GameObject::update();
}

void Entity::updateLastPos()
{
	Component *transform = this->getComponent(0); //ptr to transformation component
	Component* t = this->getComponent(1);

	t->setDisp(transform->getDisplacement());
	t->setDirection(transform->getDirection());
	t->setSize(transform->getSize());
}

void Entity::updateTimers()
{
	this->updateAnimationTimer();

	this->m_statistics->update(this->m_timeRef);
}

void Entity::updateAnimationTimer()
{
	if (this->m_activeComponents[VISUAL_DATA].size() != 0)
	{
		m_components[m_activeComponents[VISUAL_DATA][0]]->getTimerRef() += this->m_timeRef;
	}
}

void Entity::updateAggro(Entity *player)
{
	//empty for now
}

void Entity::updateAI()
{
	if (this->m_ai != nullptr)
	{
		this->m_ai->mainUpdate();
	}
}

glm::vec2 Entity::getPos()
{
	Component *transform = this->getComponent(0); //ptr to transformation component
	return transform->getDisplacement();
}

glm::vec2 Entity::getLastMove()
{
	return this->m_lastMove;
}

glm::vec2 Entity::getLastPos()
{
	return this->getComponent(1)->getDisplacement();
}

float Entity::getDepth()
{
	return 0.5f;
}

EntityType Entity::getType()
{
	return this->m_type;
}

std::string Entity::getName()
{
	return this->m_statistics->getName();
}

float Entity::getSize()
{
	return this->getComponent(0)->getSize();
}

FourPoints* Entity::getHitboxPtr()
{
	return this->m_mainbody->getPhysicsObject()->getHB();
}

int Entity::getHealth()
{
	return this->m_statistics->getHealth();
}

int Entity::getStamina()
{
	return this->m_statistics->getHealth();
}

int Entity::getMagic()
{
	return this->m_statistics->getMagic();
}

Orientation Entity::getDirection()
{
	return this->getComponent(0)->getDirection();
}

int Entity::getActiveAnimation()
{
	int returnValue;
	if (this->m_activeComponents[VISUAL_DATA].size() != 0)
	{
		m_components[m_activeComponents[VISUAL_DATA][0]]->getCurrentRenderTarget(returnValue);
	}
	else
	{
		returnValue = -1;
	}
	return returnValue;
}

double& Entity::getAnimationTimerRef()
{ 
	if (this->m_activeComponents[VISUAL_DATA].size() != 0)
	{
		return m_components[m_activeComponents[VISUAL_DATA][0]]->getTimerRef();
	}
	return this->m_timeRef;
}

PhysObject* Entity::getMainBodyPtr() //no ownership passed
{
	return this->m_mainbody->getPhysicsObject();
}

bool Entity::getIsAggro()
{
	return this->m_isAggro;
}

void Entity::setPos(glm::vec2 target)
{
	Component *transform = this->getComponent(0); //ptr to transformation component
	transform->setDisp(target);
}

void Entity::setPos(glm::vec3 target)
{
	Component *transform = this->getComponent(0); //ptr to transformation component
	transform->setDisp(glm::vec2(target.x, target.y));
	//this->m_depth = target.z;
}

void Entity::setCollidingSide(int side)
{
	this->m_collidingSide[side] = true;
}

void Entity::clearCollidingSide()
{
	this->m_collidingSide = glm::bvec4(false);
}

void Entity::setAI(AI *newAI)
{
	if (this->m_ai != nullptr)
	{
		delete this->m_ai;
	}

	this->m_ai = newAI;
}

Entity::~Entity()
{
	//empty dstr
	this->setAI(nullptr);  //deletes ai
}

/*
end Entity superclass
*/