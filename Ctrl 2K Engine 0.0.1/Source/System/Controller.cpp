#include <Ctrl 2kPCH.h>

/*

Controller class

*/
/*
Controller::Controller(std::vector<Action> *actionDictionary) :
	m_actionDictionary(actionDictionary)
{
	//cstr
	this->m_mode = 0;
}
*/
Controller::Controller(CoreEngine& parent, std::map<char, std::string> &keyBinds, std::map<char, std::string> &keyBindsC) :
	m_parent(parent), m_keyBindsHeld(keyBinds), m_keyBindsClicked(keyBindsC)
{
	//cstr
	this->m_mode = 1;
}

Controller::Controller(CoreEngine& parent) :
	m_parent(parent)
{
	//cstr
	this->m_mode = 1;
}

void Controller::convertInput()
{
	switch (this->m_mode)
	{
	case 0: //AI
		break;
	case 1: //Player
		this->m_actionsToPerform.clear();
		//for (int index = 0; index < pressedKeys.size(); index++)
		for (auto it = this->pressedKeys.begin(); it != this->pressedKeys.end(); it++) //iterates through for keys being held
		{
			if (it->second) //key is pressed, use dictionary
			{
				this->m_actionsToPerform.push_back(this->m_keyBindsHeld[it->first]);
			}
		}
		for (int i = 0; i < this->clickedKeys.size(); i++) //iterates through for keys being clicked
		{
			this->m_actionsToPerform.push_back(this->m_keyBindsClicked[this->clickedKeys[i]]);
		}
		this->clickedKeys.clear(); //clears up the keys in clicked keys, might be changed to accomodate a timer
		break;
	}
}

std::vector<std::string>* Controller::queryOutput()
{
	this->convertInput();
	return &(this->m_actionsToPerform);
}

void Controller::changeKeyBinding(char c, const std::string& str, bool held)
{
	if (held)
	{
		this->m_keyBindsHeld[c] = str;
		return;
	}
	this->m_keyBindsClicked[c] = str;
}

void Controller::pressKey(char c)
{
	this->pressedKeys[c] = true;
	this->clickedKeys.push_back(c);
}

void Controller::offKey(char c)
{
	this->pressedKeys[c] = false;
}

void Controller::pressMouseButton(char button)
{
	this->pressedKeys[button] = true;
	this->clickedKeys.push_back(button);
}

void Controller::offMouseButton(char button)
{
	this->pressedKeys[button] = false;
}

void Controller::mousePosition(glm::vec2 pos)
{
	this->mousePos = pos;
}

Controller::~Controller()
{

}

/*

end Controller class

*/