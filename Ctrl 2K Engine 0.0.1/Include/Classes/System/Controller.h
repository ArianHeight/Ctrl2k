#pragma once

/*

takes a signal from AI or input and converts it to something the entity class can use

*/
//for linking
class CoreEngine;

class Controller
{
public:
	//Controller(std::vector<Action> *actionDictionary);
	Controller(CoreEngine& parent, std::map<char, std::string> &keyBinds, std::map<char, std::string> &keyBindsC); //for player controlled classes
	Controller(CoreEngine& parent);
	~Controller();

protected:
	short m_mode; //0 is ai 1 is player
	CoreEngine& m_parent;

	std::map<char, std::string> m_keyBindsHeld; //used for player controlled classes only
	std::map<char, std::string> m_keyBindsClicked; //used for player controlled classes only
	std::map<char, bool> pressedKeys; //map of all keys keys being pressed
	std::vector<char> clickedKeys; //all the keys being clicked
	glm::vec2 mousePos; //position of mouse in image space

	//std::vector<Action> *m_actionDictionary;
	//std::vector<int> m_Controller;
	std::vector<std::string> m_actionsToPerform;

	void convertInput(); //"translates" player or AI input into something entity class can use

public:
	//void changeActionDictionary(); //changes the action dictionary
	std::vector<std::string>* queryOutput(); //returns the list of actions needed to be performed per frame
	void changeKeyBinding(char c, std::string &str, bool held = false);
	void pressKey(char c);
	void offKey(char c);
	void pressMouseButton(char button);
	void offMouseButton(char button);
	void mousePosition(glm::vec2 pos);
};

/*

end Controller

*/