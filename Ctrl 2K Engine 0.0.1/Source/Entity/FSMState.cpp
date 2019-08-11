#include <Ctrl 2kPCH.h>

//helper function
template <typename T>
bool inVector(T in, std::vector<T>& theList);

/*

FSMState class

*/

FSMState::FSMState(std::string name) :
	m_stateName(name)
{
	//cstr
}

FSMState::FSMState(std::map<std::string, int> effects, std::string name) :
	m_effects(effects), m_stateName(name)
{
	//cstr
}

//mutators
void FSMState::setStateName(std::string name)
{
	this->m_stateName = name;
}

void FSMState::addEffect(std::string str, int importance)
{
	this->m_effects.insert(std::pair<std::string, int>(str, importance));
}

//accessors
std::string FSMState::getStateName()
{
	return this->m_stateName;
}

std::vector<std::string> FSMState::getAllEffects()
{
	std::vector<std::string> returnValue;

	for (std::pair<std::string, int> value : this->m_effects)
	{
		returnValue.push_back(value.first);
	}

	return returnValue;
}

std::string FSMState::getMostImportantEffect()
{
	std::pair<std::string, int> highestValue("", -1);

	for (std::pair<std::string, int> value : this->m_effects)
	{
		if (value.second > highestValue.second) //higher importance
		{
			highestValue = value;
		}
	}

	return highestValue.first;
}

FSMState::~FSMState()
{

}

/*

end FSMState class

*/

/*

ConditionsToChange

*/

ConditionToChange::ConditionToChange(int target) :
	targetState(target)
{
	//cstr
}

ConditionToChange::~ConditionToChange()
{
	//dstr
	this->required.clear();
	this->requiredMissing.clear();
}

/*

end ConditionsToChange

*/

/*

Modular State

*/

ModularState::ModularState(std::string& name, std::vector<std::string> *consPtr, int currentState) :
	m_name(name), conditionsPtr(consPtr), m_currentState(currentState)
{
	//cstr
}

std::string& ModularState::getCurrentState()
{
	if (this->m_currentState != -1) //only returns a substate nane if there are substates
	{
		return this->m_subStates[this->m_currentState]->getCurrentState();
	}
	return this->m_name;
}

std::string& ModularState::getState()
{
	return this->m_name;
}

void ModularState::addState(ModularState* state, bool setCurrent) //passes ownership
{
	this->m_subStates.push_back(state);
	if (setCurrent) //if setcurrent is true, newly added state will be set as current state
	{
		this->m_currentState = this->m_subStates.size() - 1;
	}
}

void ModularState::addStateChangeCons(ConditionToChange *con)
{
	this->m_changeConditions.push_back(con);
}

int ModularState::changeState()
{
	bool change = false; //temp variable for deciding whether or not a state change will occur
	for (int count = 0; count < this->m_changeConditions.size(); count++) //iterates through all conditions required to change states
	{
		change = true;
		for (int i = 0; i < this->m_changeConditions[count]->required.size(); i++) //iterates through all required
		{
			if (!inVector(this->m_changeConditions[count]->required[i], *(this->conditionsPtr))) //if required for state change are not in condiiotns, no change
			{
				change = false;
				break;
			}
		}
		for (int i = 0; i < this->m_changeConditions[count]->requiredMissing.size(); i++) //iterates through all required missing
		{
			if (inVector(this->m_changeConditions[count]->requiredMissing[i], *(this->conditionsPtr))) //if required missing for state change are in conditions, no change
			{
				change = false;
				break;
			}
		}
		if (change) //if a state change is plausible, early return
		{
			return this->m_changeConditions[count]->targetState;
		}
	}

	return -1; //no change
}

void ModularState::update()
{
	if (this->m_currentState != -1) //only if there are substates
	{
		int newState = this->m_subStates[this->m_currentState]->changeState();
		if (newState != -1)
		{
			this->m_currentState = newState;
		}
		this->m_subStates[this->m_currentState]->update();
	}
}


ModularState::~ModularState()
{
	//dstr
	this->conditionsPtr = nullptr;

	for (int i = 0; i < this->m_changeConditions.size(); i++)
	{
		if (this->m_changeConditions[i] != nullptr) //nullptr guard
		{
			delete this->m_changeConditions[i];
			this->m_changeConditions[i] = nullptr;
		}
	}

	for (int i = 0; i < this->m_subStates.size(); i++)
	{
		if (this->m_subStates[i] != nullptr) //nullptr guard
		{
			delete this->m_subStates[i];
			this->m_subStates[i] = nullptr;
		}
	}

	//clears vectors
	this->m_changeConditions.clear();
	this->m_subStates.clear();
}

/*

end Modular State

*/