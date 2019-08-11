#include <Ctrl 2kPCH.h>

/*

code for Spawner class

*/

//for statics
std::random_device Spawner::m_random;
std::mt19937 Spawner::m_mt(Spawner::m_random());

Spawner::Spawner()
{
	//cstr
}

int Spawner::spawn(int lRange, int rRange) //inclusive both sides I think?
{
	std::uniform_int_distribution<int> distribution(lRange, rRange);
	return distribution(m_mt);
}

float Spawner::spawn(float lRange, float rRange)
{
	std::uniform_real_distribution<double> distribution(lRange, rRange);
	return float(distribution(m_mt));
}

float Spawner::varyByPercentage(float midValue, float percentage)
{
	std::uniform_real_distribution<double> distribution(-percentage, percentage);
	return midValue * (1.0f + float(distribution(m_mt)));
}

glm::vec2 Spawner::boxSpawn(glm::vec2 lRange, glm::vec2 rRange)
{
	std::uniform_real_distribution<double> dOne(lRange.x, rRange.x);
	std::uniform_real_distribution<double> dTwo(lRange.y, rRange.y);
	return glm::vec2(dOne(m_mt), dTwo(m_mt));
}

glm::vec2 Spawner::circleSpawn(glm::vec2 center, float radius)
{
	//finds the x
	std::uniform_real_distribution<double> dOne(-radius, radius);
	float diff = dOne(m_mt);
	glm::vec2 result = glm::vec2(center.x + diff);

	//get maximum possible y
	float maxY = glm::sqrt((radius * radius) - (diff * diff));
	std::uniform_real_distribution<double> dTwo(center.y - diff, center.y + diff);

	//actual y
	result.y = dTwo(m_mt);

	return result;
}

glm::vec2 Spawner::randomDirection()
{
	//finds the x
	std::uniform_real_distribution<double> dOne(0.0, 2.0 * glm::pi<double>());
	float radians = float(dOne(m_mt));
	glm::vec2 result = glm::vec2(glm::cos(radians), glm::sin(radians));

	return result;
}

glm::vec2 Spawner::fanSpawnDir(glm::vec2 midLine, float varyingRad)
{
	float tempSin = glm::asin(midLine.y);
	float mRad = glm::acos(midLine.x); //gets the radian of the line
	if (tempSin < 0)
	{
		mRad *= -1;
	}

	std::uniform_real_distribution<double> dOne(-varyingRad, varyingRad); //gets the variation in radian of the angle
	float actualRad = float(dOne(m_mt));

	mRad += actualRad;
	return glm::vec2(glm::cos(mRad), glm::sin(mRad));
}

Spawner::~Spawner()
{
	//dstr
}

/*

end Spawner

*/