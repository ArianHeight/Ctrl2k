#pragma once

//pre-declaration of helper functions
const FourPoints& operator+(const FourPoints& in, const glm::vec2& change);
const FourPoints& operator-(const FourPoints& in, const glm::vec2& change);
const FourPoints& operator*(const FourPoints& one, const float& two);

/*

Physics Object class is what the physics engine deals with
PhysObject is a hitbox

*/

class PhysEngine;

class PhysObject
{
	friend class PhysEngine;

public:
	PhysObject(); //default cstr
	PhysObject(glm::vec2 center, float mass = 1.0f, bool backgroundCollision = true);
	virtual ~PhysObject();

protected:
	PhysType m_type{RECTANGLE};
	bool m_backgroundCollision;
	bool m_colliding{ false };

	glm::vec2 m_center; //0, 0 in most cases
	float m_mass;

public:
	//accessors
	bool getBC();
	bool isColliding();
	float getMass();
	PhysType getType();

	//mutators
	void setBC(bool in);
	void changeMass(float addValue);
	void setMass(float newWeight);

	//for self managing
	virtual void updateHitbox();

	//virtual functions for child classes
	virtual FourPoints* getHB(); //for rectangle
	virtual glm::vec2 getCenter(); //for circles and fans
	virtual float getRadius(); //for circles and fans
	virtual glm::vec2 getAngle(); //for fans
};

/*

end PhysObject

*/

/*
rectangle hitbox
*/
class RectangleHB : public PhysObject
{
	friend PhysEngine;

public:
	RectangleHB(glm::vec2 center, glm::vec2 dimensions, float mass = -250.0f); //both are in object space and not wp
	RectangleHB(FourPoints& hb, float mass = -250.0f); //other constructor
	~RectangleHB();

	FourPoints* getHB();
private:
	FourPoints m_hitbox;
};
/*
end rectangle
*/
/*
circularHB
*/
class CircularHB : public PhysObject
{
	friend PhysEngine;

public:
	CircularHB(glm::vec2 center, float radius, float mass = -250.0f); //object space
	virtual ~CircularHB();

	glm::vec2 getCenter();
	float getRadius();
protected:
	float m_radius;
};
/*
end circularHB
*/
/*
fan
*/
class FanHB : public CircularHB
{
	friend PhysEngine;

public:
	FanHB(glm::vec2 center, float radius, glm::vec2 degrees, float mass = -250.0f); //object space. the degrees is the two sides of the fan going cw
	~FanHB();

	glm::vec2 getAngle();
private:
	glm::vec2 m_angle;
};
/*
end fan
*/