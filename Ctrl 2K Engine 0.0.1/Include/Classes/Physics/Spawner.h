#pragma once

/*

basically a class containing a rng which will control the spawning position of gameObjects

*/

class Spawner
{
public:
	Spawner();
	~Spawner();

private:
	static std::random_device m_random;
	static std::mt19937 m_mt;

public:
	int spawn(int lRange, int rRange);
	float spawn(float lRange, float rRange);
	float varyByPercentage(float midValue, float percentage);
	glm::vec2 boxSpawn(glm::vec2 lRange, glm::vec2 rRange);
	glm::vec2 circleSpawn(glm::vec2 center, float radius);
	glm::vec2 randomDirection();
	glm::vec2 fanSpawn(glm::vec2 origin, float radius, float lRad, float rRad);
	glm::vec2 fanSpawnDir(glm::vec2 midLine, float varyingRad);
};

/*

end Spawner

*/