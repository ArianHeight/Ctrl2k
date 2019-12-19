#pragma once

//gameobject needed to be defined
class GameObject;
class RenderEngine;
class PhysObject;

/*

a component that describes the nature of a game object
also an abstract class

*/

class Component
{
public:
	Component();
	virtual ~Component();

	ComponentType getType(); //returns type of component
	void setGameObject(GameObject *obj); //sets the link to parent object, same across all child classes of component
	GameObject* getGameObjectPtr(); //returns bound gameobject

	//Transformation
	virtual void setDisp(glm::vec2 newDisp);
	virtual void changeDisp(glm::vec2 deltaDisp);
	virtual void setSize(float newSize);
	virtual void changeSize(float deltaSize);
	virtual void setDirection(Orientation d);
	virtual glm::vec2 getDisplacement();
	virtual float getSize();
	virtual Orientation getDirection();
	virtual glm::vec2 getDirNormals();

	//Renderer
	//virtual RenderType renderBind(RenderEngine *rEngine);

	//PhysComponent
	virtual PhysObject* getPhysicsObject();
	virtual bool canCollide();
	virtual bool inScreenPos();
	virtual bool hasMouseCollision();
	virtual void enableMCollision();

	//Connector
	virtual std::string getTarget();
	virtual FourPoints* getSelfHB();
	virtual FourPoints* getTargetHB();

	//VisualData
	virtual RenderType getCurrentRenderTarget(int &targetIndex);
	virtual double& getTimerRef();
	virtual glm::vec2 getAR();

	//EntityData
	virtual bool useHealth(float amount);
	virtual bool useStamina(float amount);
	virtual bool useMagic(float amount);
	virtual void setDeathState(bool state);
	virtual float getHealth();
	virtual float getStamina();
	virtual float getMagic();
	virtual bool getDeathState();

	//DmgValue
	virtual int outputDmg(glm::bvec3 &strikeProperties);

protected:
	GameObject* object = nullptr;
	ComponentType m_type;
};

/*

end component

*/

/*

different varieties of components

*/

//holds a transformation
class Transformation : public Component
{
public:
	Transformation(glm::vec2 disp = glm::vec2(0.0f), float s = 1.0f, Orientation d = UP);
	~Transformation();

	//mutators
	void setDisp(glm::vec2 newDisp) override;
	void changeDisp(glm::vec2 deltaDisp) override;
	void setSize(float newSize) override;
	void changeSize(float deltaSize) override;
	void setDirection(Orientation d) override;

	//accessors
	glm::vec2 getDisplacement() override;
	float getSize() override;
	Orientation getDirection() override;
	glm::vec2 getDirNormals();

	//op override
	Transformation& operator=(Transformation& input); //bound GameObject does not change!!!!!
private:
	glm::vec2 displacement;
	float size;
	Orientation dir;
};

//can be passed to render engine for rendering
class Renderer : public Component
{
public:
	Renderer();
	virtual ~Renderer();

	//virtual RenderType renderBind(RenderEngine *rEngine);
protected:

};

//renderer specifically targeted to texture rendering
class TextureRenderer : public Renderer
{
public:
	TextureRenderer();
	~TextureRenderer();

	//RenderType renderBind(RenderEngine *rEngine);
private:

};

//renderer specifically targeted to animation rendering
class AnimationRenderer : public Renderer
{
public:
	AnimationRenderer();
	~AnimationRenderer();

	//RenderType renderBind(RenderEngine *rEngine);
private:

};

//holds a physics object for physics engine to compute things
class PhysComponent : public Component
{
public:
	PhysComponent(PhysObject *obj, bool screenPos = false, bool collision = true, bool mCollision = false); //the phys object passed to this cstr is also passing ownership
	~PhysComponent(); //will delete phys object

	PhysObject* getPhysicsObject(); //returns a ptr to the physobject
	bool canCollide(); //returns whether or not no-clip is turned on
	bool inScreenPos(); //returns whether or not the physobject is already in screen space coordinates or not
	bool hasMouseCollision(); //returns whether or not the physobject can collide with mouse
	void enableMCollision(); //enables mouse collision
private:
	PhysObject *hitbox;
	bool m_collision; //whether or no collision is turned on
	bool m_screenPos; //in screen pos or not
	bool m_mouseCollision; //whether or not can collide with mouse
};

//connector holding the maps - FOR MAP USE ONLY!!!
class Connector : public Component
{
public:
	Connector(FourPoints &selfHB, std::string &targetName, FourPoints &targetHB); //first hitbox is connector area for this obj
	~Connector();

	std::string getTarget();
	FourPoints* getSelfHB();
	FourPoints* getTargetHB();
private:
	FourPoints m_selfHB;
	FourPoints m_targetHB;
	std::string m_targetName;
};

//holds data like textures and animations
class VisualData : public Component 
{
public:
	VisualData(RenderType type, int i, glm::vec2 aspectRatio);
	~VisualData();

	RenderType getCurrentRenderTarget(int &targetIndex); //returns either texture, animation, or otherwise. target name will be set to the target's name
	double& getTimerRef(); //returns ref to animation timer
	glm::vec2 getAR(); //returns the aspect ratio of animation/texture
private:
	std::string m_name;
	RenderType m_renderType;
	double m_timer{0.0};
	int index{0}; //either animation or texture managers
	glm::vec2 ar; //aspect ratio
};

//holds data for entity-based classes
class EntityData : public Component
{
public:
	EntityData(std::string& name, float mHealth, float mStamina, float mMagic, bool interval, float hps, float sps, float mps);
	~EntityData();

	void update(double &time);
	virtual bool useHealth(float amount); //returns false if entity has no health to kill, in this case entity will still use the health and end up with negative
	virtual bool useStamina(float amount); //returns false if not enough stamina to use
	virtual bool useMagic(float amount); //returns false if not enough magic to use
	virtual void setDeathState(bool state);

	//accessors
	virtual float getHealth();
	virtual float getStamina();
	virtual float getMagic();
	virtual bool getDeathState();
	std::string getName();
private:
	std::string m_name; //name of entity

	//max stat
	float m_maxHealth;
	float m_maxStamina;
	float m_maxMagic;

	//regen stats
	bool m_interval; //whether or no health regens on intervals
	double m_healthTimer{0.0};
	double m_staminaTimer{0.0};
	double m_magicTimer{0.0};
	float m_hps; //health per second(if interval then seconds per health)
	float m_sps; //stamina per second(if interval then seconds per stamina)
	float m_mps; //magic per second(if interval then seconds per magic)

	//basic stats
	float m_health;
	float m_stamina;
	float m_magic;
	bool m_isDead{false};

	//buffer for active effects
	std::vector<ActiveEffect> m_CurrentEffects; //igore for now add in later

	void updateValue(double &timer, double &time, float &spu, float &value); //abstract value update requiring timer, time, seconds per unit and value
};

//to hold movement data classes
class MoveData : public Component
{
public:
	MoveData(float speedOne, float speedTwo, float speedThree);
	~MoveData();

	void setMovement(int newMove);
	float getSpeed();
private:
	int m_movement{ 1 }; //0 1 2 is walking running sprinting
	float m_walkSpeed;
	float m_runSpeed;
	float m_sprintSpeed;
};

//to hold damage values
class DmgValue : public Component
{
public:
	DmgValue(int baseDamage, float critChance, float critMultiplier, float pierceChance, float pierceMultiplier, float bluntChance, float bluntMultiplier);
	~DmgValue();

	int outputDmg(glm::bvec3 &strikeProperties); //does damage calculations, strikeProperties.x = critcal strike, .y = piercing hit. .z = blunt strike
private:
	Spawner m_rng; //using mt19937

	int m_base;
	float m_critChance;
	float m_critMulti; //actual multiplier
	float m_pierceChance;
	float m_pierceMulti; //this is additive multiplier
	float m_bluntChance;
	float m_bluntMulti; //this is also additive multiplier
};

/*

end components

*/