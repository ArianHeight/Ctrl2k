#include <Ctrl 2kPCH.h>

/*

Component class

*/

Component::Component(const ComponentType& type) : m_type(type), m_activeIndex(-1) {} //empty cstr

const ComponentType Component::getType() const {
	return m_type;
}

//returns a bunch of default values, this is set up for the child classes
//Transformation
void Component::setDisp(const glm::vec2& newDisp) {}
void Component::changeDisp(const glm::vec2& deltaDisp) {}
void Component::setSize(const float& newSize) {}
void Component::changeSize(const float& deltaSize) {}
void Component::setDirection(const Orientation& d) {}
const glm::vec2 Component::getDisplacement() const { return glm::vec2(0.0f); }
const float Component::getSize() const { return 0.0f; }
const Orientation Component::getDirection() const { return UP; }
const glm::vec2 Component::getDirNormals() const { return glm::vec2(0.0f, 1.0f); }

//Renderer
//RenderType Component::renderBind(RenderEngine *rEngine) { return TEXTURE; }

//PhysComponent
PhysObject* const Component::getPhysicsObject() const { return nullptr; }
const bool Component::canCollide() const { return false; }
const bool Component::inScreenPos() const { return false; }
const bool Component::hasMouseCollision() const { return false; }
void Component::enableMCollision() {  }

//Connector
std::string Component::getTarget() { return ""; }
FourPoints* const Component::getSelfHB() { return nullptr; }
FourPoints* const Component::getTargetHB() { return nullptr; }

//VisualData
const RenderType Component::getCurrentRenderTarget(int& targetIndex) const { return TEXTURE; }
double& const Component::getTimerRef() { double d = 0.0; return d; } //will cause error
const glm::vec2 Component::getAR() const { return glm::vec2(1.0f); }

//Entity Data
const bool Component::useHealth(const float& amount) { return false; }
const bool Component::useStamina(const float& amount) { return false; }
const bool Component::useMagic(const float& amount) { return false; }
void Component::setDeathState(const bool& state) {}
const float Component::getHealth() const { return 0.0f; }
const float Component::getStamina() const { return 0.0f; }
const float Component::getMagic() const { return 0.0f; }
const bool Component::getDeathState() const { return true; }

//DmgValue
const int Component::outputDmg(glm::bvec3& strikeProperties) { return 0; }

void Component::setGameObject(GameObject* const& obj) {
	m_object = obj;
}

GameObject* const Component::getGameObjectPtr() const {
	return m_object;
}

void Component::setActiveIndex(const int& index) {
	m_activeIndex = index;
}

const int Component::getActiveIndex() {
	return m_activeIndex;
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
Transformation::Transformation(const glm::vec2& disp, const float& s, const Orientation& d) :
	Component(TRANSFORMATION), m_displacement(disp), m_size(s), m_dir(d) {
	
}

//mutators
void Transformation::setDisp(const glm::vec2& newDisp) {
	m_displacement = newDisp;
}

void Transformation::changeDisp(const glm::vec2& deltaDisp) {
	m_displacement += deltaDisp;
}

void Transformation::setSize(const float& newSize) {
	m_size = newSize;
}

void Transformation::changeSize(const float& deltaSize) {
	m_size += deltaSize;
}

void Transformation::setDirection(const Orientation& d) {
	m_dir = d;
}

//accessors
const glm::vec2 Transformation::getDisplacement() const {
	return m_displacement;
}

const float Transformation::getSize() const {
	return m_size;
}

const Orientation Transformation::getDirection() const {
	return m_dir;
}

const glm::vec2 Transformation::getDirNormals() const {
	switch (m_dir) {
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

Transformation& Transformation::operator=(Transformation& input) {
	setDisp(input.getDisplacement());
	setDirection(input.getDirection());
	setSize(input.getSize());

	return *this;
}

Transformation::~Transformation() {} //empty dstr
/*
end Transformation Class
*/

/*
Renderer Class
*/
Renderer::Renderer() : Component(RENDERER) {}

Renderer::~Renderer() {} //empty dstr
/*
end Renderer Class
*/

/*
Texture Renderer Class
*/
TextureRenderer::TextureRenderer() {
	//empty cstr
}

TextureRenderer::~TextureRenderer() {} //empty dstr
/*
end Texture Renderer Class
*/

/*
PhysComponent Class
*/
PhysComponent::PhysComponent(PhysObject* const obj, const bool& screenPos, const bool& collision, const bool& mCollision) :
	Component(PHYSOBJECT), m_hitbox(obj), m_screenPos(screenPos), m_collision(collision), m_mouseCollision(mCollision) {}

PhysObject* const PhysComponent::getPhysicsObject() const {
	return m_hitbox;
}

const bool PhysComponent::canCollide() const {
	return m_collision;
}

const bool PhysComponent::inScreenPos() const {
	return m_screenPos;
}

const bool PhysComponent::hasMouseCollision() const {
	return m_mouseCollision;
}

void PhysComponent::enableMCollision() {
	m_mouseCollision = true;
}

PhysComponent::~PhysComponent() {
	//dstr
	if (m_hitbox != nullptr) { //ptr guard
		delete m_hitbox;
		m_hitbox = nullptr;
	}
}
/*
end PhysComponent Class
*/

/*
Connector Class
*/
Connector::Connector(const FourPoints &selfHB, const std::string &targetName, const FourPoints &targetHB) :
	Component(CONNECTOR), m_selfHB(selfHB), m_targetName(targetName), m_targetHB(targetHB) {}

std::string Connector::getTarget() {
	return m_targetName;
}

FourPoints* const Connector::getSelfHB() {
	return &(m_selfHB);
}

FourPoints* const Connector::getTargetHB() {
	return &(m_targetHB);
}

Connector::~Connector() {}
/*
end Connector Class
*/

/*
Visual Data Class
*/
VisualData::VisualData(const RenderType& type, const int& i, const glm::vec2& aspectRatio) :
	Component(VISUAL_DATA), m_renderType(type), index(i), ar(aspectRatio) {}

const RenderType VisualData::getCurrentRenderTarget(int& targetIndex) const {
	targetIndex = index;
	return this->m_renderType;
}

double& const VisualData::getTimerRef() {
	return this->m_timer;
}

const glm::vec2 VisualData::getAR() const {
	return this->ar;
}

VisualData::~VisualData() {} //empty dstr
/*
end Visual Data Class
*/

/*
EntityData Class
*/
EntityData::EntityData(const std::string& name, const float& mHealth, const float& mStamina, const float& mMagic,
	const bool& interval, const float& hps, const float& sps, const float& mps) :
	Component(ENTITY_DATA), m_maxHealth(mHealth), m_health(mHealth), m_maxStamina(mStamina), m_stamina(mStamina), 
	m_maxMagic(mMagic), m_magic(mMagic), m_interval(interval), m_hps(hps), m_sps(sps), m_mps(mps), m_name(name) {}

void EntityData::updateValue(double &timer, double &time, float &spu, float &value) { //all in seconds except value is in units
	timer += time;

	while (timer >= spu) {
		value++; //increment
		timer -= spu;
	}
}

//*****requires optimizing for sure!!!
void EntityData::update(double &time) {
	if (!this->m_isDead) { //only updates if not dead
		switch (this->m_interval) {
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

const bool EntityData::useHealth(const float& amount) {
	this->m_health -= amount;
	return (this->m_health >= 0);
}

const bool EntityData::useStamina(const float& amount) {
	if (this->m_stamina < amount) {
		return false;
	}

	this->m_stamina -= amount;
	return true;
}

const bool EntityData::useMagic(const float& amount) {
	if (this->m_magic < amount) {
		return false;
	}

	this->m_magic -= amount;
	return true;
}

void EntityData::setDeathState(const bool& state) {
	this->m_isDead = state;
}

//accessors
const float EntityData::getHealth() const {
	return this->m_health;
}

const float EntityData::getStamina() const {
	return this->m_stamina;
}

const float EntityData::getMagic() const {
	return this->m_magic;
}

const bool EntityData::getDeathState() const {
	return this->m_isDead;
}

std::string EntityData::getName() const {
	return this->m_name;
}

EntityData::~EntityData() {
	//dstr
}
/*
end EntityData Class
*/

/*
MoveData Class
*/
MoveData::MoveData(const float& speedOne, const float& speedTwo, const float& speedThree) :
	Component(MOVE_DATA), m_walkSpeed(speedOne), m_runSpeed(speedTwo), m_sprintSpeed(speedThree) {}

void MoveData::setMovement(const int& newMove) {
	if (newMove >= 0 && newMove <= 2) {
		m_movement = newMove;
	}
}

const float MoveData::getSpeed() {
	switch (m_movement) {
	case 0:
		return m_walkSpeed;
	case 1:
		return m_runSpeed;
	case 2:
		return m_sprintSpeed;
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
DmgValue::DmgValue(const int& baseDamage, const float& critChance, const float& critMultiplier, 
	const float& pierceChance, const float& pierceMultiplier, const float& bluntChance, const float& bluntMultiplier) :
	Component(DMG_VALUE), m_base(baseDamage), m_critChance(critChance), m_critMulti(critMultiplier), 
	m_pierceChance(pierceChance), m_pierceMulti(pierceMultiplier), m_bluntChance(bluntChance), m_bluntMulti(bluntMultiplier) {
	//cstr
	if (m_critChance > 1.0f) { //clamping values
		m_critChance = 1.0f;
	}
	else if (m_critChance < 0.0f) {
		m_critChance = 0.0f;
	}
	if (m_pierceChance > 1.0f) { //clamping values
		m_pierceChance = 1.0f;
	}
	else if (m_pierceChance < 0.0f) {
		m_pierceChance = 0.0f;
	}
	if (m_bluntChance > 1.0f) { //clamping values
		m_bluntChance = 1.0f;
	}
	else if (m_bluntChance < 0.0f) {
		m_bluntChance = 0.0f;
	}
}

const int DmgValue::outputDmg(glm::bvec3& strikeProperties) {
	//sets basic values
	strikeProperties.x = false;
	strikeProperties.y = false;
	strikeProperties.z = false;
	float totalDmg = m_base;

	//chance calculations
	float percentage = m_rng.spawn(0.0f, 1.0f);
	if (percentage < m_critChance) { //critical strike = true
		totalDmg *= m_critMulti;
		strikeProperties.x = true;
	}
	percentage = m_rng.spawn(0.0f, 1.0f);
	if (percentage < m_pierceChance) { //piercing hit = true
		totalDmg += m_base * m_pierceMulti;
		strikeProperties.y = true;
	}
	percentage = m_rng.spawn(0.0f, 1.0f);
	if (percentage < m_bluntChance) { //blunt strike = true
		totalDmg += m_base * m_pierceMulti;
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