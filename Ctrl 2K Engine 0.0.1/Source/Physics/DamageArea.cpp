#include <Ctrl 2kPCH.h>

//pre-declare helper
bool epsilonEqual(double inOne, double inTwo, double epsilon);

/*

an area in world space where entities will take damage

*/

DamageArea::DamageArea(GameObject* initiator, double &time, Component *dmg, Component *hb, Component *visual, Component *transform, Component *velocity, double duration) :
	m_attackInitiator(initiator), m_timeRef(time), m_effectiveDuration(duration)
{
	//cstr
	m_timer = 0;
	this->m_immune.push_back(initiator); //no ownership
	
	//components
	std::string tempName = "Position"; //sets transformation (data copy)
	Component* Cptr = this->getComponent(tempName);
	Cptr->setDisp(transform->getDisplacement());
	Cptr->setDirection(transform->getDirection());
	tempName = "Last Position"; //sets last transformation
	this->getComponent(tempName)->setDisp(Cptr->getDisplacement());
	tempName = "Velocity"; //change in position every second
	this->addComponent(tempName, velocity);
	tempName = "Visual"; //for rendering data, visual is probably only for DA
	this->addComponent(tempName, visual); //passes ownership
	tempName = "Hitbox"; //for physobject, Hitbox is probably global for all which have one
	this->addComponent(tempName, hb); //passes ownership
	tempName = "Damage"; //for damage values
	this->addComponent(tempName, dmg); //passes ownership

	//properties
	tempName = "DoesDamage";
	this->addProperty(tempName);
}

void DamageArea::update()
{
	GameObject::update();

	if (this->updateDA()) //needs deleting
	{
		this->deleteGO();
	}
}

//main method that is run by idle() every tick
bool DamageArea::updateDA()
{
	bool returnValue = this->updateTimer();

	//does stuff
	this->getComponent(std::string("Position"))->changeDisp(this->getComponent(std::string("Velocity"))->getDisplacement()); //updates position in accordance to velocity

	return returnValue;
}

bool DamageArea::updateTimer()
{
	this->m_timer += m_timeRef;

	if (this->m_timer > this->m_effectiveDuration)
	{
		return true;
	}

	return false;
}

bool DamageArea::doDamage(GameObject *target)
{
	for (int i = 0; i < this->m_immune.size(); i++) //early return if object is immune
	{
		if (target == this->m_immune[i])
		{
			return false;
		}
	}

	//target->damageHealth(this->m_damage, this->m_givenEffects, this->m_pierce); //add crit chance in later
	std::string tempName = "Statistics";
	Component *targetStats = target->getComponent(tempName);
	tempName = "Damage";
	Component *dmgValue = this->getComponent(tempName);

	if (targetStats != nullptr && dmgValue != nullptr) //check if object actually has health
	{
		glm::bvec3 strike;
		int dmg = dmgValue->outputDmg(strike);
		targetStats->useHealth(dmg); //actually takes damage
	}

	this->m_immune.push_back(target); //add target to immune once it has dmg dealt
	return true;
}

//accessors
bool DamageArea::getDraw()
{
	return this->m_draw;
}

int DamageArea::getAnimation()
{
	return this->m_animation;
}

double& DamageArea::getAnimationTimerRef()
{
	return this->m_animationTimer;
}

PhysObject* DamageArea::getHitboxPtr()
{
	return &(this->m_hitbox);
}

GameObject* DamageArea::getInitiatorPtr()
{
	return this->m_attackInitiator;
}

std::vector<GameObject*>& DamageArea::getImmuneRef()
{
	return this->m_immune;
}

DamageArea::~DamageArea()
{

}

/*

end DamageArea class

*/

/*

Melee Hitbox class

*/

MeleeDmgArea::MeleeDmgArea(GameObject* initiator, double &time, Component *dmg, Component *hb, Component *visual, Component *transform) :
	DamageArea(initiator, time, dmg, hb, visual, transform, new Transformation(), 0.5)
{
	//cstr
	//properties
	std::string tempName = "ParticleSpawnOnCollision";
	//this->addProperty(tempName);
}

MeleeDmgArea::~MeleeDmgArea()
{
	//dstr
}

/*

end Melee Hitbox class

*/

/*

Projectile Hitbox class

*/

ProjectileDmgArea::ProjectileDmgArea(GameObject* initiator, double &time, Component *dmg, Component *hb, Component *visual, Component *transform, Component *velocity, double duration) :
	DamageArea(initiator, time, dmg, hb, visual, transform, velocity, duration)
{
	//cstr
	//properties
	std::string tempName = "DeleteOnCollision";
	this->addProperty(tempName);
	tempName = "ParticleSpawnOnCollision";
	this->addProperty(tempName);
}

ProjectileDmgArea::~ProjectileDmgArea()
{
	//dstr
}

/*

end Projectile Hitbox class

*/