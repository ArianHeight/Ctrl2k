#pragma once

/*

Game
the class that stores everything in the game

*/

class CoreEngine;

class Game
{
public:
	Game(CoreEngine &engine);
	~Game();

private:
	CoreEngine &parentEngine;
	GameObject* globalActiveObjects; //string is name, eg widgets or objects. shared_ptr being passed will be unique!!

	GameObject* entities; //no ownership
	GameObject* playArea; //no ownership, but map can switch
	GameObject* widgets; //no ownership
	GameObject* player; //no ownership, player is not a category!!!
	int currentMapIndex; //index for current map being used, for mapmanager

	std::vector<GameEvent> globalEventsQueue; //list of events happening globally

public:
	void addObj(const std::string &category, GameObject* obj); //adds gameobject to list. entity or playArea for first arg
	void render(); //calls render on every gameObject
	void update(); //calls update on every gameObject
	void getAllPOs(std::vector<Component*> &output); //puts all physicsobjects into the output buffer
	void clearEventQueue(); //clears the globaly event queue

	void setMapTo(int index);
	void setPlayer(GameObject* newPlayer); //sets the current controlled player to newPlayer

	void debugSetup(); //the debug dataset will be set up

	CoreEngine& getEngine(); //no ownership is passed
	std::vector<GameEvent>& getEventQueue(); //no owner ship is passed
	int getMapIndex(); //returns the index of the current map
	int& getMapIndexRef(); //returns the index of the current map
	GameObject* getPlayer(); //ownership is not passed
	GameObject* getScene(); //ownership not passed
};

/*

end Game

*/