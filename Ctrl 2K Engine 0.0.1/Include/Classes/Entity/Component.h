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
	Component(const ComponentType& type);
	virtual ~Component();

	const ComponentType getType() const; //returns type of component
	void setGameObject(GameObject* const obj); //sets the link to parent object, same across all child classes of component
	GameObject* const getGameObjectPtr() const; //returns bound gameobject

	//Transformation
	virtual void setDisp(const glm::vec2& newDisp);
	virtual void changeDisp(const glm::vec2& deltaDisp);
	virtual void setSize(const float& newSize);
	virtual void changeSize(const float& deltaSize);
	virtual void setDirection(const Orientation& d);
	virtual const glm::vec2 getDisplacement() const;
	virtual const float getSize() const;
	virtual const Orientation getDirection() const;
	virtual const glm::vec2 getDirNormals() const;

	//Renderer
	//virtual RenderType renderBind(RenderEngine *rEngine);

	//PhysComponent
	virtual PhysObject* const getPhysicsObject() const;
	virtual const bool canCollide() const;
	virtual const bool inScreenPos() const;
	virtual const bool hasMouseCollision() const;
	virtual void enableMCollision();

	//Connector
	virtual std::string getTarget();
	virtual FourPoints* const getSelfHB();
	virtual FourPoints* const getTargetHB();

	//VisualData
	virtual const RenderType getCurrentRenderTarget(int& targetIndex) const;
	virtual double& const getTimerRef();
	virtual const glm::vec2 getAR() const;

	//EntityData
	virtual const bool useHealth(const float& amount);
	virtual const bool useStamina(const float& amount);
	virtual const bool useMagic(const float& amount);
	virtual void setDeathState(const bool& state);
	virtual const float getHealth() const;
	virtual const float getStamina() const;
	virtual const float getMagic() const;
	virtual const bool getDeathState() const;

	//DmgValue
	virtual const int outputDmg(glm::bvec3& strikeProperties);

protected:
	GameObject* m_object = nullptr;
	const ComponentType m_type;
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
	Transformation(const glm::vec2& disp = glm::vec2(0.0f), const float& s = 1.0f, const Orientation& d = UP);
	~Transformation();

	//mutators
	void setDisp(const glm::vec2& newDisp) override;
	void changeDisp(const glm::vec2& deltaDisp) override;
	void setSize(const float& newSize) override;
	void changeSize(const float& deltaSize) override;
	void setDirection(const Orientation& d) override;

	//accessors
	glm::vec2 const getDisplacement() const override;
	float const getSize() const override;
	Orientation const getDirection() const override;
	glm::vec2 const getDirNormals() const;

	//op override
	Transformation& operator=(Transformation& input); //bound GameObject does not change!!!!!
private:
	glm::vec2 m_displacement;
	float m_size;
	Orientation m_dir;
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
	PhysComponent(PhysObject* const obj, const bool& screenPos = false, const bool& collision = true, const bool& mCollision = false); //the phys object passed to this cstr is also passing ownership
	~PhysComponent(); //will delete phys object

	PhysObject* const getPhysicsObject() const override; //returns a ptr to the physobject
	const bool canCollide() const override; //returns whether or not no-clip is turned on
	const bool inScreenPos() const override; //returns whether or not the physobject is already in screen space coordinates or not
	const bool hasMouseCollision() const override; //returns whether or not the physobject can collide with mouse
	void enableMCollision() override; //enables mouse collision
private:
	PhysObject* m_hitbox;
	bool m_collision; //whether or no collision is turned on
	bool m_screenPos; //in screen pos or not
	bool m_mouseCollision; //whether or not can collide with mouse
};

//connector holding the maps - FOR MAP USE ONLY!!!
class Connector : public Component
{
public:
	Connector(const FourPoints &selfHB, const std::string &targetName, const FourPoints &targetHB); //first hitbox is connector area for this obj
	~Connector();

	std::string getTarget() override;
	FourPoints* const getSelfHB() override;
	FourPoints* const getTargetHB() override;
private:
	FourPoints m_selfHB;
	FourPoints m_targetHB;
	std::string m_targetName;
};

//holds data like textures and animations
class VisualData : public Component 
{
public:
	VisualData(const RenderType& type, const int& i, const glm::vec2& aspectRatio);
	~VisualData();

	const RenderType getCurrentRenderTarget(int& targetIndex) const override; //returns either texture, animation, or otherwise. target name will be set to the target's name
	double& const getTimerRef() override; //returns ref to animation timer
	const glm::vec2 getAR() const override; //returns the aspect ratio of animation/texture
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
	EntityData(const std::string& name, const float& mHealth, const float& mStamina,
		const float& mMagic, const bool& interval, const float& hps, const float& sps, const float& mps);
	~EntityData();

	void update(double &time);
	virtual const bool useHealth(const float& amount) override; //returns false if entity has no health to kill, in this case entity will still use the health and end up with negative
	virtual const bool useStamina(const float& amount) override; //returns false if not enough stamina to use
	virtual const bool useMagic(const float& amount) override; //returns false if not enough magic to use
	virtual void setDeathState(const bool& state) override;

	//accessors
	virtual const float getHealth() const override;
	virtual const float getStamina() const override;
	virtual const float getMagic() const override;
	virtual const bool getDeathState() const override;
	std::string getName() const;
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
	MoveData(const float& speedOne, const float& speedTwo, const float& speedThree);
	~MoveData();

	void setMovement(const int& newMove);
	const float getSpeed();
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
	DmgValue(const int& baseDamage, const float& critChance, const float& critMultiplier, 
		const float& pierceChance, const float& pierceMultiplier, const float& bluntChance, const float& bluntMultiplier);
	~DmgValue();

	const int outputDmg(glm::bvec3& strikeProperties) override; //does damage calculations, strikeProperties.x = critcal strike, .y = piercing hit. .z = blunt strike
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