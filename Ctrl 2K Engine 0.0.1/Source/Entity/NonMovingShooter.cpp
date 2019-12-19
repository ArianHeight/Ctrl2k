#include <Ctrl 2kPCH.h>

/*

non moving turret like entities

*/

NonMovingShooter::NonMovingShooter(int shotTexture, double &time, std::string &name, PhysObject *hitbox, glm::vec2 position, EntityType type, float speedOne, float speedTwo, float speedThree, float range, Orientation dir) :
	Entity(time, name, hitbox, position, type, speedOne, speedTwo, speedThree), m_shotTexture(shotTexture), m_range(range)
{
	//cstr
	//this->m_direction = dir;

	this->setAI(new NonMovingShooterAI(this));
}

void NonMovingShooter::updateOrientation(glm::vec2 &newDir)
{
	if (this->m_ai->queryMostImportantOutput() == "NOTHING")
	{
		return; //early return
	}

	Entity::updateOrientation(newDir);
}

bool NonMovingShooter::inView(float angle)
{
	switch (this->getComponent(0)->getDirection()) //40 degree field of vision
	{
	case UP:
		if (angle >= 70 && 110 >= angle)
		{
			return true;
		}
		break;
	case DOWN:
		if (angle <= -70 && -110 <= angle)
		{
			return true;
		}
		break;
	case LEFT:
		if (angle >= 160 || -160 >= angle)
		{
			return true;
		}
		break;
	case RIGHT:
		if (angle <= 20 && -20 <= angle)
		{
			return true;
		}
		break;
	}
	return false;
}

void NonMovingShooter::updateAI()
{
	this->m_ai->mainUpdate();
	this->attack();
	std::string command = this->m_ai->queryMostImportantOutput();
	if (command == "ATTACK")
	{
		//RRxk
		this->attack();
	}
	else
	{
		//this->m_shotTimer = 0.0;
	}
}

void NonMovingShooter::updateHealth()
{
	//empty method
}

void NonMovingShooter::updateStamina()
{/*
	if (this->m_stamina < this->m_stats.max_stamina)
	{
		//only use fast stamina regen because its a turret
		int increase = (int)(this->m_staminaTimer / this->m_stats.stamina_fastRegen_seconds);
		this->m_stamina += increase;
		this->m_staminaTimer = this->m_staminaTimer - (increase * this->m_stats.stamina_fastRegen_seconds);

		if (this->m_stamina > this->m_stats.max_stamina)
		{
			this->m_stamina = this->m_stats.max_stamina;
		}
	}
	else
	{
		this->m_staminaRegen = false;
		this->m_staminaTimer = 0.0;
	}*/
}

void NonMovingShooter::updateMagic()
{
	//empty
}

void NonMovingShooter::updatePain()
{
	//empty
}

void NonMovingShooter::updateAggro(Entity *player)
{
	//simple fan-shaped aggro
	glm::vec2 dist = player->getPos() - this->getPos();
	glm::vec2 normal = glm::normalize(dist); //angle to player
	this->m_playerNormal = normal; //for shooting the projectile
	float angleToPlayer = glm::degrees(glm::asin(normal.y / sqrt(normal.x * normal.x + normal.y * normal.y)));
	if (normal.x < 0) //player to the left
	{
		if (angleToPlayer >= 0) //topside
		{
			angleToPlayer = 180.0f - angleToPlayer;
		}
		else
		{
			angleToPlayer = -180.0f - angleToPlayer;
		}
	}

	if (sqrt(dist.x * dist.x + dist.y * dist.y) < 15.0f && this->inView(angleToPlayer))
	{
		this->m_aggro = player;
		this->m_isAggro = true;
	}
	else
	{
		this->m_aggro = nullptr;
		this->m_isAggro = false;
	}
}

void NonMovingShooter::attack() //assumed that m_aggro is not nullptr
{
	this->m_shotTimer += this->m_timeRef;

	if (this->m_shotTimer >= this->m_spr)
	{
		//shoot
		this->m_shotTimer -= this->m_spr;
		std::cout << "bam" << '\n';
		this->addChild(new ProjectileDmgArea(this, this->m_timeRef, 
			new DmgValue(27, 0.05f, 2.0f, 0.0f, 0.0f, 0.05f, 0.06f), 
			new PhysComponent(new RectangleHB(glm::vec2(0.0f), glm::vec2(0.5f))), 
			new VisualData(TEXTURE, this->m_shotTexture, glm::vec2(0.25f)), 
			this->getComponent(0), new Transformation(this->m_playerNormal * 0.35f), 1.25));
		/*
		this->m_dmgAreaRef.push_back(new DamageArea(this, this->m_timeRef, 0.0f, 0.0f, 30, this->getPos(),
			FourPoints{ glm::vec2(-0.25f, 0.25f), glm::vec2(0.25f, 0.25f), glm::vec2(-0.25f, -0.25f), 
			glm::vec2(0.25f, -0.25f) }, 2.0, this->m_playerNormal * 0.35f, this->m_shotTexture));*/
	}
}

NonMovingShooter::~NonMovingShooter()
{
	//dstr
	this->setAI(nullptr);  //deletes ai
}

/*

end NonMovingShooter

*/