#pragma once

/*

A string based state system

*/

class FSMState
{
public:
	FSMState(std::string name);
	FSMState(std::map<std::string, int> effects, std::string name);
	~FSMState();

private:
	std::string m_stateName;
	std::map<std::string, int> m_effects; //effect name, importance. if used in base fsm, only use highest importance effect

public:
	void setStateName(std::string name);
	void addEffect(std::string str, int importance); //pls no repeating importance in range -1 - 999, much thanks

	std::string getStateName();
	std::vector<std::string> getAllEffects(); //for hierarchy based FSM
	std::string getMostImportantEffect(); //for base FSM
};

/*

string based state

*/

/*

ConditionsToChange
just a data block class

*/

class ConditionToChange
{
public:
	ConditionToChange(int target);
	~ConditionToChange();

	int targetState;
	std::vector<std::string> required; //conditions required for a change to the target state
	std::vector<std::string> requiredMissing; //conditions required to be missing for a change to target state
};

/*

end ConditionsToChange

*/

/*

Modular State

*/

class ModularState
{
public:
	ModularState(std::string& name, std::vector<std::string> *consPtr = nullptr, int currentState = -1);
	~ModularState();

private:
	std::string m_name;
	std::vector<std::string>* conditionsPtr; //ptr to all conditions that the entity is situated in
	std::vector<ModularState*> m_subStates; //children states
	std::vector<ConditionToChange*> m_changeConditions; //the change coditions to the current level(not for the children but from itself to other states of same lvl)
	int m_currentState; //-1 for no sub-state

public:
	std::string& getCurrentState(); //gets the name of the most specific state
	std::string& getState(); //gets the name of this state
	void addState(ModularState* state, bool setCurrent = false); //passes ownership, adds a sub state
	void addStateChangeCons(ConditionToChange *con); //add state change conditions to this state(not for its sub states)
	int changeState(); //called by update every time a change in state is requested
	void update(); //called every time by entity if state needs to be refreshed/udpated

};

/*

end Modular State

*/