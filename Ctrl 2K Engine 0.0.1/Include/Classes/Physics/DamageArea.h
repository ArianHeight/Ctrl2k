#pragma once

/*

an area in world space where entities will take damage

*/

//friend classes
class PhysEngine;

class DamageArea : public GameObject
{
public:
	DamageArea(GameObject* initiator, double &time, Component *dmg, Component *hb, Component *visual, Component *transform, Component *velocity, double duration = 30.0); //ownership passed with ptr
	virtual ~DamageArea();

	friend class PhysEngine;

protected:
	//fps
	double &m_timeRef;

	double m_effectiveDuration; //in seconds
	double m_timer; //counts up from zero

	//physics object
	PhysObject m_hitbox; //uses m_origin as the center pos

	//damage values
	std::vector<Effects> m_givenEffects; //will give to any entities hit
	GameObject* m_attackInitiator; //entity that initiated this damageArea
	std::vector<GameObject*> m_immune; //all entities immune to this area

	//for drawing
	int m_animation;
	double m_animationTimer{ 0.0 };
	bool m_draw = false; //false for no draw, true for draw
	
public:
	virtual void update(); //standard update function that is called once every frame
	//virtual void render(); //standard rendering function
	bool updateDA(); //main update returns whether the dmg area is still active or not

	bool updateTimer(); //returns true if object is needed to be destroyed
	bool doDamage(GameObject *target);

	bool getDraw();
	int getAnimation();
	double& getAnimationTimerRef();
	PhysObject* getHitboxPtr(); //ownership is with DA
	GameObject* getInitiatorPtr(); //no ownership
	std::vector<GameObject*>& getImmuneRef(); //no ownership
};

/*

end DamageArea class

*/

/*

Melee Hitbox

*/

class MeleeDmgArea : public DamageArea
{
public:
	MeleeDmgArea(GameObject* initiator, double &time, Component *dmg, Component *hb, Component *visual, Component *transform);
	virtual ~MeleeDmgArea();

	friend class PhysEngine;
};

/*

end Melee Hitbox

*/

/*

Projectile Hitbox

*/

class ProjectileDmgArea : public DamageArea
{
public:
	ProjectileDmgArea(GameObject* initiator, double &time, Component *dmg, Component *hb, Component *visual, Component *transform, Component *velocity, double duration = 30.0);
	virtual ~ProjectileDmgArea();

	friend class PhysEngine;
};

/*

end Projectile Hitbox

*/