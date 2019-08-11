#pragma once

/*

modular based ai that uses BaseFSM
also an abstract class

*/

class Entity; //pre-declaration because AI needs it

class AI
{
public:
	AI();
	virtual ~AI();

	virtual int updateFSM();
	virtual void mainUpdate();
	virtual std::vector<std::string>& queryOutput();
	virtual std::string queryMostImportantOutput();
	virtual void setLink(Entity *linkedEntity);

protected:
	std::vector<std::string> m_outputList;
	BaseFSM* m_fsm;
	Entity* linkedEntityPtr;
};

/*

BaseAI

*/

/*

ai for non-moving turret like enemies

*/

class NonMovingShooterAI : public AI
{
public:
	NonMovingShooterAI(Entity *self);
	~NonMovingShooterAI();

private:
	//empty for now

public:
	int updateFSM();
	void mainUpdate(); //goes through all preconditions and picks the state
	//std::vector<std::string>& queryOutput();
};

/*

end NonMovingShooterAI

*/