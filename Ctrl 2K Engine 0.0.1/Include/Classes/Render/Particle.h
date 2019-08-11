#pragma once

/*

A single particle

*/

class ParticleEffect; //for friending

class Particle
{
public:
	Particle(glm::vec2 pos, glm::vec2 startVelocity, double lifeTime, glm::vec2 acceleration = glm::vec2(0.0f), glm::vec4 tint = glm::vec4(1), std::string *animationIndex = nullptr, std::string *textureIndex = nullptr);
	~Particle();

	friend class ParticleEffect; //easy access of data
private:
	glm::vec2 m_pos{0, 0}; //world position at center of particle at the current time
	glm::vec2 m_velocity{0, 0}; //m/s at the current time
	glm::vec2 m_acceleration{0, 0}; //constance acceleration
	glm::vec4 m_colourTint{0, 0, 0, 0}; //tint of particle
	std::string *m_texture; //"" for non texture, will use colour instead
	std::string *m_animation; //"" for non animation, will use texture instead
	double m_age{0.0}; //seconds since emission
	double m_lifeSpan{0.0}; //seconds from start of emission to particle death, -250.0 for infinite lifetime

public:
	void emit(glm::vec2 pos, glm::vec2 startVelocity); //resets the particle if particle is dead
	bool isDead(); //if the particle is dead or not

	//mutators
	void setPos(glm::vec2 newPos);
	void addPos(glm::vec2 distance);
	void setVelocity(glm::vec2 newVel);
	void updateTime(double t);

	//accessors
	glm::vec2 getPos();
	glm::vec2 getVelocity();
	glm::vec2 getAcceleration();
	double getAge();
	std::string* getAnimationName();
	std::string* getTextureName();
};

/*

end Particle

*/

/*

a particle effect, like fire or beam effects

*/

class ParticleEffect
{
public:
	ParticleEffect(glm::vec2 spawnPos);
	virtual ~ParticleEffect();

protected:
	Spawner m_rng;
	std::vector<Particle*> m_allParticles; //active and non active
	glm::vec2 m_pos; //position of particle effect

	std::string m_texture; //"" for non texture, will use colour instead
	std::string m_animation; //"" for non animation, will use texture instead

public:
	bool isDead(); //returns whether or not all particles have died

	//accessor
	std::vector<Particle*>& getBufferRef();
};

/*

end particle effect

*/

/*

child classes of Particle Effect

*/

class ExplosiveFX : public ParticleEffect
{
public:
	ExplosiveFX(glm::vec2 spawnPos, double perParticleLifeTime, float totalAcceleration, float generalVelocityMagnitude, int numOfParticles);
	~ExplosiveFX();
};

class SparkFX : public ParticleEffect 
{
public:
	SparkFX(glm::vec2 spawnPos, double perParticleLifeTime, glm::vec2 entryVelocity, int numOfParticles);
	~SparkFX();
};