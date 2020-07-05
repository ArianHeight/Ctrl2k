#pragma once

/*

A game object that exists within the scene

*/

//pre-declarations
class Game;
class Camera;
class GUI;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

protected:
	Camera *renderCam = nullptr; //may be mullptr or may not
	Game *game = nullptr; //ptr to the game the obj is created in
	GameObject *m_parent = nullptr; //ptr to parent object
	GUI *displayingHud = nullptr; //may be null or may not, points to a hud that will grab data off this obj to display
	bool m_delete{ false }; //delete var for queue

	std::vector<Indices> m_activeComponents; //vector of a vector of all indices to components that are active(accessed with corresponding type)
	std::vector<GameObject*> m_children; //all children classes
	std::vector<Component*> m_components;
	std::vector<std::string> m_inputs; //all inputs per frame
	std::vector<std::string> m_properties; //properties which the engines will use to define how obj interacts with the world

	void initComponents(); //initialize components - should only be used in cstr!!!
	void removeFromCam(); //remove this object from camera's target
	void removeFromHud(); //remove this object from hud's target
public:
	void linkParent(GameObject* const& obj); //links to parent
	void linkGame(Game* const& g); //links to game
	void setLinkCam(Camera* const& c); //links to render camera
	bool isCamLinked(); //is the camera locked onto this obj
	void setLinkHud(GUI* const& h); //links to the displaying hud
	bool isHudLinked(); //is the hud locked onto this obj

	void activateComponent(const int& index); //puts the component index into m_activeComponents[type]
	void disableComponent(const int& index); //removes the component index from m_activeComponents[type]
	int addComponent(Component* const& component, const bool& active = false); //adds a component to the object, returns the index
	void addChild(GameObject* const& obj); //adds a child object to the GO
	void clearChildrenPtr(const int& lvl = 0); //deletes all children ***Just The PTR NOT With The Actual Object***!!! This is DANGEROUS AND IS ONLY MEANT FOR USE IN MAP OBJECTS!!!
	void deleteGO(); //queues for delete

	void addProperty(const std::string& str); //adds a property
	bool hasProperty(const std::string& pro); //returns whether or not the obj has the following property
	void clearAllProperties(); //clears all property, DONOT USE THIS UNLESS U KNOW WHAT U R DOING!!!!!!!!!!!!!!

	virtual void update(); //main update method
	virtual void render(); //render method
	void inputCommand(const std::string& str);
	void inputCommand(std::vector<std::string>* const& in);

	//virtual methods for children
	virtual bool doDamage(GameObject* target);

	//accessors
	virtual std::string getName();
	bool getDelete();
	Component* const& getComponent(const int& index); //grabs component depending on index
	const int checkForComponent(const int& index); //checks if component exist in class
	const Indices& getAllActiveComponentIndices(const ComponentType& type); //returns all current active components of type in this object
	void getAllActiveComponentsC(std::vector<Component*> &output, const ComponentType& type); //appends ptrs of all active components of one type(from all children as well as this obj) into a std::vector ref
	GameObject* getChild(const std::string& name); //no ownership passed
};

/*

end game object

*/