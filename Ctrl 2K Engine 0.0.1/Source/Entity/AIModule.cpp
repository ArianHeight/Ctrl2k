#include <Ctrl 2kPCH.h>

/*

base modular ai

*/

AI::AI()
{
	//cstr
}

void  AI::mainUpdate()
{
	//empty for now
}

std::vector<std::string>& AI::queryOutput()
{
	return this->m_outputList;
}

std::string AI::queryMostImportantOutput()
{
	return this->m_fsm->getEffect();
}

void AI::setLink(Entity *linkedEntity)
{
	this->linkedEntityPtr = linkedEntity;
}

int AI::updateFSM()
{
	//empty for now
	return 0; //idle
}

AI::~AI()
{
	//dstr
	if (this->m_fsm != nullptr)
	{
		delete this->m_fsm;
		this->m_fsm = nullptr;
	}
}

/*

end base ai

*/

/*

ai for non moving turret type enemies

*/

NonMovingShooterAI::NonMovingShooterAI(Entity *self)
{
	this->setLink(self);
	this->m_fsm = new BaseFSM();
	std::map<std::string, int> tempFXBuffer; //for storing effects before putting them into states

	this->m_fsm->removeState("IDLE"); //starts with and IDLE state with effect FREEMOVE

	//adds states into the fsm
	tempFXBuffer.insert(std::pair<std::string, int>("NOTHING", 1));
	this->m_fsm->addNewState(FSMState(tempFXBuffer, "DEAD"));
	tempFXBuffer.clear();

	tempFXBuffer.insert(std::pair<std::string, int>("NOTHING", 1));
	this->m_fsm->addNewState(FSMState(tempFXBuffer, "IDLE"));
	tempFXBuffer.clear();

	tempFXBuffer.insert(std::pair<std::string, int>("ATTACK", 1));
	this->m_fsm->addNewState(FSMState(tempFXBuffer, "ATTACKING"));
	tempFXBuffer.clear();
}

int NonMovingShooterAI::updateFSM()
{
	if (!this->linkedEntityPtr->getIsAggro()) //idle
	{
		return 1; //early return for isAggro();
	}
	if (this->linkedEntityPtr->getHealth() < 0) //dead
	{
		return 0; //early return for dead
	}
	return 2; //attacking
}

void NonMovingShooterAI::mainUpdate()
{
	this->m_fsm->setCurrentState(this->updateFSM());
	this->m_outputList = this->m_fsm->getAllEffects();
}

NonMovingShooterAI::~NonMovingShooterAI()
{
	AI::~AI();
}

/*

end NonMovingShooterAI

*/