#pragma once

/*

The Particle Engine

*/

class ParticleEngine
{
public:
	ParticleEngine(CoreEngine &engine);
	~ParticleEngine();

private:
	CoreEngine& parentEngine;
	std::vector<ParticleEffect*> m_buffer;
	int m_bufferSize;

public:
	void spawnEffect(); //some random effect particle
	void addEffect(ParticleEffect *pfx);
	void update(); //updates all of the effects and checks for particles to despawn
	void render(); //render all existent particles
};

/*

end Particle Engine

*/