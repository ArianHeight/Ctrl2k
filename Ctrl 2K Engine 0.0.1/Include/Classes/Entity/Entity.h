#pragma once

/*
Entity class which is a gameobject
*/

class DamageArea; //pre-declaration

class Entity : public GameObject
{
public:
	Entity(
		double &time, 
		const std::string &name, 
		PhysObject *hitbox,
		glm::vec2 position = glm::vec2(0.0f), 
		EntityType type = AGGRESSIVE,
		float speedOne = 4.0f,
		float speedTwo = 8.0f,
		float speedThree = 18.0f
		);
	virtual ~Entity();

protected:
	double &m_timeRef; //secondsElapsed since last frame
	EntityData* m_statistics; //ptr to the datas for game
	MoveData* m_movement; //ptr to the datas for game
	PhysComponent* m_mainbody; //ptr to the datas for game

	std::string m_name;
	EntityType m_type;
	AI *m_ai = nullptr; //this is tied to the entity and needs to be deleted when obj is discarded

	glm::bvec4 m_collidingSide = glm::bvec4(false); //top bottom left right
	Entity *m_aggro; //aggro entity
	bool m_isAggro; //is the entity aggroed on something

	bool m_movedLastFrame = false;
	bool m_moved = false;
	glm::vec2 m_lastMove = glm::vec2(0.0f);

	//active stats
	//Orientation m_direction = DOWN;

public:
	virtual void updateOrientation(glm::vec2 &newDir);
	void move(glm::vec2 direction);
	void move(Orientation newDir);
	virtual void processInput(); //recognizes the input and uses it
	void engineMove(glm::vec3 distance); //exclusive for phys engine
	void engineMove(Manifest distance);

	virtual void update();

protected:
	void updateLastPos();
	void updateTimers();
	void updateAnimationTimer();
	virtual void updateAI();
	bool updateHealth();

public:
	virtual void updateAggro(Entity *player);

	glm::vec2 getPos();
	glm::vec2 getLastMove();
	glm::vec2 getLastPos();
	float getDepth();
	EntityType getType();
	virtual std::string getName();
	//float getAR();
	float getSize();
	FourPoints* getHitboxPtr();
	int getHealth();
	int getStamina();
	int getMagic();
	Orientation getDirection();
	int getActiveAnimation();
	double& getAnimationTimerRef();
	PhysObject* getMainBodyPtr();
	bool getIsAggro();

	void setPos(glm::vec2 target);
	void setPos(glm::vec3 target);
	void setCollidingSide(int side);
	void clearCollidingSide();
	void setAI(AI *newAI);
};

/*
end Entity class
*/