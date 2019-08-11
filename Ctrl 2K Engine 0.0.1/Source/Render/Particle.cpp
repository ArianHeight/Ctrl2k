#include <Ctrl 2kPCH.h>

/*

code for Particle

*/

Particle::Particle(glm::vec2 pos, glm::vec2 startVelocity, double lifeTime, glm::vec2 acceleration, glm::vec4 tint, std::string *animationIndex, std::string *textureIndex) :
	m_pos(pos), m_velocity(startVelocity), m_lifeSpan(lifeTime), m_acceleration(acceleration), m_colourTint(tint), m_animation(animationIndex), m_texture(textureIndex)
{
	//cstr
}

void Particle::emit(glm::vec2 pos, glm::vec2 startVelocity)
{
	this->m_pos = pos;
	this->m_velocity = startVelocity;
	this->m_age = 0.0;
}

bool Particle::isDead()
{
	return (this->m_age > this->m_lifeSpan);
}

//mutators
void Particle::setPos(glm::vec2 newPos)
{
	this->m_pos = newPos;
}

void Particle::addPos(glm::vec2 distance)
{
	this->m_pos += distance;
}

void Particle::setVelocity(glm::vec2 newVel)
{
	this->m_velocity = newVel;
}

void Particle::updateTime(double t)
{
	this->m_age += t;
}

//accessors
glm::vec2 Particle::getPos()
{
	return this->m_pos;
}

glm::vec2 Particle::getVelocity()
{
	return this->m_velocity;
}

glm::vec2 Particle::getAcceleration()
{
	return this->m_acceleration;
}

double Particle::getAge()
{
	return this->m_age;
}

std::string* Particle::getAnimationName()
{
	return this->m_animation;
}

std::string* Particle::getTextureName()
{
	return this->m_texture;
}

Particle::~Particle()
{
	//dstr
}

/*

end Particle

*/

/*

Particle Effect

*/

ParticleEffect::ParticleEffect(glm::vec2 spawnPos) :
	m_pos(spawnPos)
{
	//cstr
}

bool ParticleEffect::isDead()
{
	for (int i = 0; i < this->m_allParticles.size(); i++) //iterates through all particles
	{
		if (!this->m_allParticles[i]->isDead()) //sees if the particle is still alvie
		{
			return false;
		}
	}

	return true;
}

//accessor
std::vector<Particle*>& ParticleEffect::getBufferRef()
{
	return this->m_allParticles;
}

ParticleEffect::~ParticleEffect()
{
	//dstr
	for (int i = 0; i < this->m_allParticles.size(); i++)
	{
		if (this->m_allParticles[i] != nullptr)
		{
			delete this->m_allParticles[i];
			this->m_allParticles[i] = nullptr;
		}
	}
	this->m_allParticles.clear();
}

/*

end Particle Effect

*/

/*

child classes of ParticleEffect

*/

ExplosiveFX::ExplosiveFX(glm::vec2 spawnPos, double perParticleLifeTime, float totalAcceleration, float generalVelocityMagnitude, int numOfParticles) :
	ParticleEffect(spawnPos)
{
	//cstr
	if (numOfParticles > 64) //max 64
	{
		numOfParticles = 64;
	}

	this->m_animation = "FireEffect";
	this->m_texture = "";

	for (int i = 0; i < numOfParticles; i++) //iteration
	{
		this->m_allParticles.push_back(new Particle(this->m_pos, this->m_rng.randomDirection() * generalVelocityMagnitude, perParticleLifeTime, glm::vec2(0, totalAcceleration), glm::vec4(1), &(this->m_animation)));
	}
}

ExplosiveFX::~ExplosiveFX()
{
	//dstr
}

SparkFX::SparkFX(glm::vec2 spawnPos, double perParticleLifeTime, glm::vec2 entryVelocity, int numOfParticles) :
	ParticleEffect(spawnPos)
{
	//cstr
	if (numOfParticles > 64) //max 64
	{
		numOfParticles = 64;
	}

	this->m_animation = "FireEffect";
	this->m_texture = "";
	float generalVelocityMagnitude = glm::sqrt(entryVelocity.x * entryVelocity.x + entryVelocity.y * entryVelocity.y);
	glm::vec2 entryDirection = - glm::normalize(entryVelocity);
	float angleVariance = 1.25f / (generalVelocityMagnitude + 1.5f); //function for variance

	for (int i = 0; i < numOfParticles; i++) //iteration
	{
		this->m_allParticles.push_back(new Particle(this->m_pos, this->m_rng.fanSpawnDir(entryDirection, angleVariance) * this->m_rng.varyByPercentage(generalVelocityMagnitude / 2.0f, 0.35f), this->m_rng.varyByPercentage(perParticleLifeTime, 0.35f), glm::vec2(0), glm::vec4(1), &(this->m_animation)));
	}
}

SparkFX::~SparkFX()
{
	//dstr
}