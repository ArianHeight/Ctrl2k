#pragma once

/*
Player class
*/

class Player : public Entity
{
public:
	Player(
		double &time,
		const std::string &name,
		PhysObject *hitbox,
		glm::vec2 position = glm::vec2(0.0f),
		float speedOne = 4.0f,
		float speedTwo = 8.0f,
		float speedThree = 18.0f
		);
	virtual ~Player();

private:
	virtual void attack();
	virtual void processInput();

public:
	void usePlayer();
	void stopPlayer();
	virtual void update();
};

/*
end Player class
*/