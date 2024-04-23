#pragma once

/*

child class of entity used for specific turret-esk enemies

*/

class NonMovingShooter : public Entity
{
public:
	NonMovingShooter(
		int shotTexture,
		double &time,
		const std::string &name,
		PhysObject *hitbox,
		glm::vec2 position = glm::vec2(0.0f),
		EntityType type = AGGRESSIVE,
		float speedOne = 4.0f,
		float speedTwo = 8.0f,
		float speedThree = 18.0f, 
		float range = 15.0f, 
		Orientation dir = UP
	);
	~NonMovingShooter();
protected:
	int m_shotTexture; //temporary variable for texture??
	glm::vec2 m_playerNormal = glm::vec2(-1.0f, 0.0f); //temp variable for direction to player
	double m_spr = 1.2; //seconds per round
	double m_shotTimer = 0.0; //counts up from zero
	float m_range = 0.0; //aggro range

	bool inView(float angle); //checks if an angle is in viewing range, requires an angle in range [-180, 180]

	virtual void updateHealth(); //no regen
	virtual void updateStamina(); //stamina is turret "heat"
	virtual void updateMagic(); //none
	virtual void updatePain(); //no pain, its a turret
	virtual void updateAI();

	virtual void attack();

public:
	virtual void updateOrientation(glm::vec2 &newDir);
	//virtual void updateMainBody();
	virtual void updateAggro(Entity *player);
};

/*

end NonMovingShooter

*/