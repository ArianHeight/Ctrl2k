#pragma once

/*
main physics engine
*/

class PhysEngine
{
public:
	PhysEngine(Game *game, Map *tile, double &timeRef); //no ownership passed
	~PhysEngine();

private:
	double &m_timeRef; //reference point to time
	Game *m_game; //ref to the current game

	Map *playgroundTile; //current collision map

	//runtime build lists
	std::vector<GameObject*> m_possibleCollisions;
	std::vector<GameObject*> m_collidingAABB;
	std::vector<PhysEvent> m_collisionEvents;

	//singles
	std::vector<Component*> m_backgroundColliding; //list of PhysComponents that have collided with the background
	std::vector<Component*> m_hitboxList; //list of PhysComponents loaded in

public:
	void mapChange(Map *tile); //changes the current playgroundTile

	void compileHitboxList(); //iterates through the list of hitboxes and deletes the unnecessary ones

	void backgroundCollision(); //test against playgroundTile
	void buildCollisionList(bool &mapChange, int &newMap); //find all possible collisions as well as background collisions
	void updateCollidingObjects(); //uses m_possibleCollisions
	void updateObjects();

	//math
	float getDistanceFromVelocity(float initVelocity, float downAcceleration, double time);
	float getFVelocityFromDistance(float initVelocity, float downAcceleration, float distance);
	glm::vec2 getDistanceFromVelocity(glm::vec2 initVelocity, glm::vec2 downAcceleration, double time);
	glm::vec2 getFVelocityFromDistance(glm::vec2 initVelocity, glm::vec2 downAcceleration, glm::vec2 distance);

	//for particles
	void updateParticle(Particle *p);
	void updateParticleFX(ParticleEffect *vfx);
	void spawnExplosiveParticleFx(PhysEvent *e, bool invert = false); //false spawns at hbOne, true spawns at hbTwo
	void spawnSparkParticleFx(PhysEvent *e, bool invert = false); //false spawns at hbOne, true spawns at hbTwo

	//collision detection
	bool aabbCheck(const glm::vec2 &wpOne, FourPoints *wpTwo); //checks point against hitbox
	bool aabbCheck(FourPoints *wpOne, FourPoints *wpTwo); //checks aabb hitbox against hitbox

	bool circleCheck(const glm::vec2 &point, const glm::vec2& origin, float radius); //checks point against circle hitbox
	bool circleCheck(const glm::vec2 &originOne, float radiusOne, const glm::vec2& originTwo, float radiusTwo); //checks circle against circle hitbox

	bool lineIntersectCheck(DamageArea *lineOne, DamageArea *lineTwo);
	bool collision(Component *one, Component *two);

	//collision response
	void playerMapChange(Component *position, Component *connector);
	bool dmgResponse(PhysEvent *e, bool invert = false); //false return means no collision
	void voidResponse(PhysEvent *e, bool invert = false); //false deletes hbOne, true deletes hbTwo
	void aabbResponse(PhysEvent *e);
	void circleResponse(PhysEvent *e);
	void collisionResponse(PhysEvent *e);
};

/*
end main physics engine
*/