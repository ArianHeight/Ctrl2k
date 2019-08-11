#pragma once

/*

This class describes an action an entity can perform

*/

class EntityInput; //for friending

class Action
{
public:
	Action(std::string displacement, std::string power, std::string range, std::vector<std::string> &effects, std::string target, std::vector<std::string> &specifications);
	~Action();

	friend class EntityInput;

private:
	std::string m_displacement;
	std::string m_power;
	std::string m_range;
	std::vector<std::string> m_effects;
	std::string m_target;
	std::vector<std::string> m_specifications;

public:

};

/*

end Action obj

*/