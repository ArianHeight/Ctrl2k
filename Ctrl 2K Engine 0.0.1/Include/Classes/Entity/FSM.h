#pragma once

/*

Simple Finite State Machine

*/

class BaseFSM
{
public:
	BaseFSM();
	virtual ~BaseFSM();

protected:
	std::vector<FSMState> m_possibleStates;
	FSMState m_currentState{FSMState("temp")}; //temporary state to avoid compiler error

public:
	//mutators
	void setCurrentState(int index);
	void setCurrentState(std::string state);
	void addNewState(FSMState newState);
	void removeState(std::string stateName);

	//accesssors
	std::string getCurrentState();
	std::vector<std::string> getAllEffects();
	std::string getEffect(); //returns the most important effect of current state
	std::vector<FSMState>& getStateList(); //returns list of possible states
};

/*

BasicFSM

*/