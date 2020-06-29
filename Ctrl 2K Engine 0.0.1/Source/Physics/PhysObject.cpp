#include <Ctrl 2kPCH.h>

//adjustment functions for adjusting positions of a hitbox
const FourPoints operator+(const FourPoints &in, const glm::vec2 &change)
{
	FourPoints out = in;
	out.tl += change;
	out.tr += change;
	out.bl += change;
	out.br += change;
	return out;
}

const FourPoints operator-(const FourPoints &in, const glm::vec2 &change)
{
	FourPoints out = in;
	out.tl -= change;
	out.tr -= change;
	out.bl -= change;
	out.br -= change;
	return out;
}

const FourPoints operator*(const FourPoints &one, const float &two)
{
	return FourPoints{ one.tl * two, one.tr * two, one.bl * two, one.br * two };
}

/*

PhysObject

*/

PhysObject::PhysObject()
{
	//default cstr
	this->m_center = glm::vec2(0.0f);
	this->m_mass = 1.0f;
	this->updateHitbox();
}

PhysObject::PhysObject(glm::vec2 center, float mass, bool backgroundCollision) :
	m_center(center), m_mass(mass), m_backgroundCollision(backgroundCollision)
{
	//cstr
	this->updateHitbox();
}

//self-managing methods

//updates the world pos of the hitbox
void PhysObject::updateHitbox()
{
	//here set colliding to false
	this->m_colliding = false;
}

//accessors
bool PhysObject::getBC()
{
	return this->m_backgroundCollision;
}

bool PhysObject::isColliding()
{
	return this->m_colliding;
}

float PhysObject::getMass()
{
	return this->m_mass;
}

PhysType PhysObject::getType()
{
	return this->m_type;
}

//mutators
void PhysObject::setBC(bool in)
{
	this->m_backgroundCollision = in;
}

void PhysObject::changeMass(float addValue)
{
	this->m_mass += addValue;
}

void PhysObject::setMass(float newWeight)
{
	this->m_mass = newWeight;
}

//child class functions default returns
FourPoints* PhysObject::getHB() { return nullptr; }
glm::vec2 PhysObject::getCenter() { return this->m_center; }
float PhysObject::getRadius() { return 0.0f; }
glm::vec2 PhysObject::getAngle() { return glm::vec2(0.0f); }

PhysObject::~PhysObject()
{
	//dstr
}

/*

end PhysObject

*/
/*
Rectangle hitbox
*/
RectangleHB::RectangleHB(glm::vec2 center, glm::vec2 dimensions, float mass) :
	PhysObject(center, mass)
{
	//cstr
	float half_w = dimensions.x / 2;
	float half_h = dimensions.y / 2;

	//sets points for the hitbox
	this->m_hitbox.tl = glm::vec2(center.x - half_w, center.y + half_h);
	this->m_hitbox.tr = glm::vec2(center.x + half_w, center.y + half_h);
	this->m_hitbox.bl = glm::vec2(center.x - half_w, center.y - half_h);
	this->m_hitbox.br = glm::vec2(center.x + half_w, center.y - half_h);

	//set type
	this->m_type = RECTANGLE;
}

RectangleHB::RectangleHB(FourPoints& hb, float mass)
{
	//cstr
	this->m_hitbox = hb;

	//set type
	this->m_type = RECTANGLE;
}

FourPoints* RectangleHB::getHB()
{
	return &(this->m_hitbox);
}

RectangleHB::~RectangleHB()
{
	//dstr
}
/*
end Rectangle Hitbox
*/
/*
Circular Hitbox
*/
CircularHB::CircularHB(glm::vec2 center, float radius, float mass) :
	PhysObject(center, mass), m_radius(radius)
{
	//cstr
	//set type
	this->m_type = CIRCLE;
}

glm::vec2 CircularHB::getCenter()
{
	return this->m_center;
}

float CircularHB::getRadius()
{
	return this->m_radius;
}

CircularHB::~CircularHB()
{
	//dstr
}
/*
end Circular Hitbox
*/
/*
Fan Hitbox
*/
FanHB::FanHB(glm::vec2 center, float radius, glm::vec2 degrees, float mass) :
	CircularHB(center, radius, mass), m_angle(degrees)
{
	//cstr
	//set type
	this->m_type = FAN;
}

glm::vec2 FanHB::getAngle()
{
	return this->m_angle;
}

FanHB::~FanHB()
{
	//dstr
}
/*
end Fan Hitbox
*/