#include <Ctrl 2kPCH.h>

/*

BaseFSM class

*/

BaseFSM::BaseFSM()
{
	//cstr
	FSMState idleState("IDLE");
	idleState.addEffect("FREEMOVE", 1);

	this->m_possibleStates.push_back(idleState);
	this->m_currentState = idleState;
}

//mutators
void BaseFSM::setCurrentState(int index)
{
	if (index >= 0 && index < this->m_possibleStates.size())
	{
		this->m_currentState = this->m_possibleStates[index];
	}
}

void BaseFSM::addNewState(FSMState newState)
{
	this->m_possibleStates.push_back(newState);
}

void BaseFSM::removeState(std::string stateName)
{
	int i = 0; //index of removing index
	for (int i = 0; i < this->m_possibleStates.size(); i++)
	{
		if (stateName == this->m_possibleStates[i].getStateName())
		{
			this->m_possibleStates.erase(this->m_possibleStates.begin() + i);
			return; //early return
		}
	}
}

//accesssors
std::string BaseFSM::getCurrentState()
{
	return this->m_currentState.getStateName();
}

std::string BaseFSM::getEffect()
{
	return this->m_currentState.getMostImportantEffect();
}

std::vector<std::string> BaseFSM::getAllEffects()
{
	return this->m_currentState.getAllEffects();
}

std::vector<FSMState>& BaseFSM::getStateList()
{
	return this->m_possibleStates;
}

BaseFSM::~BaseFSM()
{

}

/*

end BaseFSM class

*/