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

	std::vector<std::string> m_renderQueue; //all visualdata classes about to be rendered
	std::vector<GameObject*> m_children; //all children classes
	std::map<std::string, Component*> m_components; //all components
	std::vector<std::string> m_inputs; //all inputs per frame
	std::vector<std::string> m_properties; //properties which the engines will use to define how obj interacts with the world

	void initComponents(); //initialize components - should only be used in cstr!!!
	void removeFromCam(); //remove this object from camera's target
	void removeFromHud(); //remove this object from hud's target
public:
	void linkParent(GameObject *obj); //links to parent
	void linkGame(Game *g); //links to game
	void setLinkCam(Camera *c); //links to render camera
	bool isCamLinked(); //is the camera locked onto this obj
	void setLinkHud(GUI *h); //links to the displaying hud
	bool isHudLinked(); //is the hud locked onto this obj

	void addComponent(std::string &name, Component *component); //adds a component to the object
	void addChild(GameObject *obj); //adds a child object to the GO
	void clearChildrenPtr(int lvl = 0); //deletes all children ***Just The PTR NOT With The Actual Object***!!! This is DANGEROUS AND IS ONLY MEANT FOR USE IN MAP OBJECTS!!!
	void deleteGO(); //queues for delete

	void addProperty(std::string &str); //adds a property
	bool hasProperty(std::string &pro); //returns whether or not the obj has the following property
	void clearAllProperties(); //clears all property, DONOT USE THIS UNLESS U KNOW WHAT U R DOING!!!!!!!!!!!!!!

	virtual void update(); //main update method
	virtual void render(); //render method
	void inputCommand(std::string& str);
	void inputCommand(std::vector<std::string>* in);

	//virtual methods for children
	virtual bool doDamage(GameObject *target);

	//accessors
	virtual std::string getName();
	bool getDelete();
	Component* getComponent(std::string &name); //grabs component depending on string
	int checkForComponent(std::string &name); //checks if component exist in class
	std::vector<std::string>* getRenderQueue(); //returns the current queue for rendering
	void getAllComponentByType(std::vector<Component*> &output, ComponentType type); //appends ptrs of all components of one type into a std::vector ref
	void getAllComponentByTypeB(std::vector<Component*> &output, ComponentType type); //appends ptrs of all components of one type into a std::vector ref from all children and itself
	GameObject* getChild(std::string &name); //no ownership passed
};

/*

end game object

*/