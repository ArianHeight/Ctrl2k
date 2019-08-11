#include <Ctrl 2kPCH.h>

/*

Component class

*/

Component::Component() {} //empty cstr

ComponentType Component::getType()
{
	return this->m_type;
}

//returns a bunch of default values, this is set up for the child classes
//Transformation
void Component::setDisp(glm::vec2 newDisp) {}
void Component::changeDisp(glm::vec2 deltaDisp) {}
void Component::setSize(float newSize) {}
void Component::changeSize(float deltaSize) {}
void Component::setDirection(Orientation d) {}
glm::vec2 Component::getDisplacement() { return glm::vec2(0.0f); }
float Component::getSize() { return 0.0f; }
Orientation Component::getDirection() { return UP; }
glm::vec2 Component::getDirNormals() { return glm::vec2(0.0f, 1.0f); }

//Renderer
RenderType Component::renderBind(RenderEngine *rEngine) { return TEXTURE; }

//PhysComponent
PhysObject* Component::getPhysicsObject() { return nullptr; }
bool Component::canCollide() { return false; }
bool Component::inScreenPos() { return false; }
bool Component::hasMouseCollision() { return false; }
void Component::enableMCollision() {  }

//Connector
std::string Component::getTarget() { return ""; }
FourPoints* Component::getSelfHB() { return nullptr; }
FourPoints* Component::getTargetHB() { return nullptr; }

//VisualData
RenderType Component::getCurrentRenderTarget(int &targetIndex) { return TEXTURE; }
double& Component::getTimerRef() { double d = 0.0; return d; } //will cause error
glm::vec2 Component::getAR() { return glm::vec2(1.0f); }

//Entity Data
bool Component::useHealth(float amount) { return false; }
bool Component::useStamina(float amount) { return false; }
bool Component::useMagic(float amount) { return false; }
void Component::setDeathState(bool state) {}
float Component::getHealth() { return 0.0f; }
float Component::getStamina() { return 0.0f; }
float Component::getMagic() { return 0.0f; }
bool Component::getDeathState() { return true; }

//DmgValue
int Component::outputDmg(glm::bvec3 &strikeProperties) { return 0; }

void Component::setGameObject(GameObject *obj)
{
	this->object = obj;
}

GameObject* Component::getGameObjectPtr()
{
	return this->object;
}

Component::~Component() {} //empty dstr

/*

end Component class

*/

/*

rest of the child classes of component

*/

/*
Transformation Class
*/
Transformation::Transformation(glm::vec2 disp, float s, Orientation d) :
	displacement(disp), size(s), dir(d)
{
	this->m_type = TRANSFORMATION; //sets type
}

//mutators
void Transformation::setDisp(glm::vec2 newDisp)
{
	this->displacement = newDisp;
}

void Transformation::changeDisp(glm::vec2 deltaDisp)
{
	this->displacement += deltaDisp;
}

void Transformation::setSize(float newSize)
{
	this->size = newSize;
}

void Transformation::changeSize(float deltaSize)
{
	this->size += deltaSize;
}

void Transformation::setDirection(Orientation d)
{
	this->dir = d;
}

//accessors
glm::vec2 Transformation::getDisplacement()
{
	return this->displacement;
}

float Transformation::getSize()
{
	return this->size;
}

Orientation Transformation::getDirection()
{
	return this->dir;
}

glm::vec2 Transformation::getDirNormals()
{
	switch (this->dir)
	{
	case UP:
		break;
	case DOWN:
		return glm::vec2(0.0f, -1.0f);
	case LEFT:
		return glm::vec2(-1.0f, 0.0f);
	case RIGHT:
		return glm::vec2(1.0f, 0.0f);
	case UPPER_LEFT:
		return glm::vec2(-0.70710678118f, 0.70710678118f);
	case LOWER_LEFT:
		return glm::vec2(-0.70710678118f);
	case UPPER_RIGHT:
		return glm::vec2(0.70710678118f);
	case LOWER_RIGHT:
		return glm::vec2(0.70710678118f, -0.70710678118f);
	}

	return glm::vec2(0.0f, 1.0f);
}

Transformation& Transformation::operator=(Transformation& input)
{
	this->setDisp(input.getDisplacement());
	this->setDirection(input.getDirection());
	this->setSize(input.getSize());

	return *this;
}

Transformation::~Transformation() {} //empty dstr
/*
end Transformation Class
*/

/*
Renderer Class
*/
Renderer::Renderer()
{
	this->m_type = RENDERER;
}

RenderType Renderer::renderBind(RenderEngine *rEngine)
{
	if (!(this->object->checkForComponent(std::string("Position")))) //guard check for if renderer has all equipped components
	{
		return NONE; //early return for not fit for rendering
	}

	std::vector<std::string> *rQueue = this->object->getRenderQueue();
	std::string atName; //animation/texture name
	Component *vData = nullptr; //VisualData Class
	Component *transform = this->object->getComponent(std::string("Position")); //Transformation Class
	int index = 0;
	RenderType type = TEXTURE; //default value to start off
	Orientation direction = UP; //default value for direction used in animations

	for (int i = 0; i < rQueue->size(); i++) //iterates through all animations needed to be drawn
	{
		atName = rQueue->at(i);
		if (this->object->checkForComponent(atName)) //checks if component exists
		{
			//only if exists
			vData = this->object->getComponent(atName); //VisualData Class
			type = vData->getCurrentRenderTarget(index);
		}
		else
		{
			type = NONE; //doesn't exist
		}

		switch (type) //draws each according to type, doesn't do anything to NONE types
		{
		case TEXTURE:
			rEngine->drawTexture(index, transform->getDisplacement(), vData->getAR(), transform->getSize());
			break;
		case ANIMATION:
			direction = transform->getDirection();
			rEngine->drawAnimation(index, vData->getTimerRef(), transform->getDisplacement(), 0.0f, direction); //remember to impliment directions later
			break;
		}
	}

	return type;
}

Renderer::~Renderer() {} //empty dstr
/*
end Renderer Class
*/

/*
Texture Renderer Class
*/
TextureRenderer::TextureRenderer()
{
	//empty cstr
}

RenderType TextureRenderer::renderBind(RenderEngine *rEngine)
{
	return TEXTURE;
}

TextureRenderer::~TextureRenderer() {} //empty dstr
/*
end Texture Renderer Class
*/

/*
PhysComponent Class
*/
PhysComponent::PhysComponent(PhysObject *obj, bool screenPos, bool collision, bool mCollision) :
	hitbox(obj), m_screenPos(screenPos), m_collision(collision), m_mouseCollision(mCollision)
{
	this->m_type = PHYSOBJECT; //cstr
}

PhysObject* PhysComponent::getPhysicsObject()
{
	return this->hitbox;
}

bool PhysComponent::canCollide()
{
	return m_collision;
}

bool PhysComponent::inScreenPos()
{
	return this->m_screenPos;
}

bool PhysComponent::hasMouseCollision()
{
	return this->m_mouseCollision;
}

void PhysComponent::enableMCollision()
{
	this->m_mouseCollision = true;
}

PhysComponent::~PhysComponent()
{
	//dstr
	if (this->hitbox != nullptr) //ptr guard
	{
		delete this->hitbox;
		this->hitbox = nullptr;
	}
}
/*
end PhysComponent Class
*/

/*
Connector Class
*/
Connector::Connector(FourPoints &selfHB, std::string &targetName, FourPoints &targetHB) :
	m_selfHB(selfHB), m_targetName(targetName), m_targetHB(targetHB)
{
	//cstr
	this->m_type = CONNECTOR;
}

std::string Connector::getTarget()
{
	return this->m_targetName;
}

FourPoints* Connector::getSelfHB()
{
	return &(this->m_selfHB);
}

FourPoints* Connector::getTargetHB()
{
	return &(this->m_targetHB);
}

Connector::~Connector() {}
/*
end Connector Class
*/

/*
Visual Data Class
*/
VisualData::VisualData(RenderType type, int i, glm::vec2 aspectRatio) :
	m_renderType(type), index(i), ar(aspectRatio)
{
	//empty cstr
	this->m_type = VISUAL_DATA;
}

RenderType VisualData::getCurrentRenderTarget(int &targetIndex)
{
	targetIndex = index;
	return this->m_renderType;
}

double& VisualData::getTimerRef()
{
	return this->m_timer;
}

glm::vec2 VisualData::getAR()
{
	return this->ar;
}

VisualData::~VisualData() {} //empty dstr
/*
end Visual Data Class
*/

/*
EntityData Class
*/
EntityData::EntityData(std::string& name, float mHealth, float mStamina, float mMagic, bool interval, float hps, float sps, float mps) :
	m_maxHealth(mHealth), m_health(mHealth), m_maxStamina(mStamina), m_stamina(mStamina), m_maxMagic(mMagic), m_magic(mMagic), 
	m_interval(interval), m_hps(hps), m_sps(sps), m_mps(mps), m_name(name)
{
	//cstr
	this->m_type = ENTITY_DATA;
}

void EntityData::updateValue(double &timer, double &time, float &spu, float &value) //all in seconds except value is in units
{
	timer += time;

	while (timer >= spu)
	{
		value++; //increment
		timer -= spu;
	}
}

//*****requires optimizing for sure!!!
void EntityData::update(double &time)
{
	if (!this->m_isDead) //only updates if not dead
	{
		switch (this->m_interval)
		{
		case true: //interval regen, does things by seconds
			this->updateValue(this->m_healthTimer, time, this->m_hps, this->m_health);
			this->updateValue(this->m_staminaTimer, time, this->m_sps, this->m_stamina);
			this->updateValue(this->m_magicTimer, time, this->m_mps, this->m_magic);
			break;
		case false: //non interval regen, does things using units per second
			this->m_health += time * this->m_hps;
			this->m_stamina += time * this->m_sps;
			this->m_magic += time * this->m_mps;
		}
	}

	//healt limit
	this->m_health = glm::min(this->m_health, this->m_maxHealth);
	this->m_stamina = glm::min(this->m_stamina, this->m_maxStamina);
	this->m_magic = glm::min(this->m_magic, this->m_maxMagic);
}

bool EntityData::useHealth(float amount)
{
	this->m_health -= amount;
	return (this->m_health >= 0);
}

bool EntityData::useStamina(float amount)
{
	if (this->m_stamina < amount)
	{
		return false;
	}

	this->m_stamina -= amount;
	return true;
}

bool EntityData::useMagic(float amount)
{
	if (this->m_magic < amount)
	{
		return false;
	}

	this->m_magic -= amount;
	return true;
}

void EntityData::setDeathState(bool state)
{
	this->m_isDead = state;
}

//accessors
float EntityData::getHealth()
{
	return this->m_health;
}

float EntityData::getStamina()
{
	return this->m_stamina;
}

float EntityData::getMagic()
{
	return this->m_magic;
}

bool EntityData::getDeathState()
{
	return this->m_isDead;
}

std::string EntityData::getName()
{
	return this->m_name;
}

EntityData::~EntityData()
{
	//dstr
}
/*
end EntityData Class
*/

/*
MoveData Class
*/
MoveData::MoveData(float speedOne, float speedTwo, float speedThree) :
	m_walkSpeed(speedOne), m_runSpeed(speedTwo), m_sprintSpeed(speedThree)
{
	this->m_type = MOVE_DATA; //cstr
}

void MoveData::setMovement(int newMove)
{
	if (newMove >= 0 && newMove <= 2)
	{
		this->m_movement = newMove;
	}
}

float MoveData::getSpeed()
{
	switch (this->m_movement)
	{
	case 0:
		return this->m_walkSpeed;
	case 1:
		return this->m_runSpeed;
	case 2:
		return this->m_sprintSpeed;
	}

	return 0.0f;
}

MoveData::~MoveData() {}
/*
end MoveData Class
*/

/*
DmgValue Class
*/
DmgValue::DmgValue(int baseDamage, float critChance, float critMultiplier, float pierceChance, float pierceMultiplier, float bluntChance, float bluntMultiplier) :
	m_base(baseDamage), m_critChance(critChance), m_critMulti(critMultiplier), m_pierceChance(pierceChance), m_pierceMulti(pierceMultiplier), m_bluntChance(bluntChance), m_bluntMulti(bluntMultiplier)
{
	//cstr
	if (this->m_critChance > 1.0f) //clamping values
	{
		this->m_critChance = 1.0f;
	}
	else if (this->m_critChance < 0.0f)
	{
		this->m_critChance = 0.0f;
	}
	if (this->m_pierceChance > 1.0f) //clamping values
	{
		this->m_pierceChance = 1.0f;
	}
	else if (this->m_pierceChance < 0.0f)
	{
		this->m_pierceChance = 0.0f;
	}
	if (this->m_bluntChance > 1.0f) //clamping values
	{
		this->m_bluntChance = 1.0f;
	}
	else if (this->m_bluntChance < 0.0f)
	{
		this->m_bluntChance = 0.0f;
	}
}

int DmgValue::outputDmg(glm::bvec3 &strikeProperties)
{
	//sets basic values
	strikeProperties.x = false;
	strikeProperties.y = false;
	strikeProperties.z = false;
	float totalDmg = this->m_base;

	//chance calculations
	float percentage = this->m_rng.spawn(0.0f, 1.0f);
	if (percentage < this->m_critChance) //critical strike = true
	{
		totalDmg *= this->m_critMulti;
		strikeProperties.x = true;
	}
	percentage = this->m_rng.spawn(0.0f, 1.0f);
	if (percentage < this->m_pierceChance) //piercing hit = true
	{
		totalDmg += this->m_base * this->m_pierceMulti;
		strikeProperties.y = true;
	}
	percentage = this->m_rng.spawn(0.0f, 1.0f);
	if (percentage < this->m_bluntChance) //blunt strike = true
	{
		totalDmg += this->m_base * this->m_pierceMulti;
		strikeProperties.z = true;
	}

	return int(totalDmg);
}

DmgValue::~DmgValue() {}
/*
end DmgValue Class
*/

/*

end child classes of component

*/