#pragma once

/*

Game User Interface

*/

class GUI : public Widget
{
public:
	GUI(std::string name, glm::vec2 topLeft, glm::vec2 bottomRight, int textureIndex = -1);
	virtual ~GUI();

protected:
	GameObject *targetObj = nullptr; //target object

public:
	void setTarget(GameObject *obj); //links the widgets onto a game object
	virtual void render();
	virtual void update();
	std::vector<GameObject*>& getChildren();

	GameObject* getTarget();
};

/*

end GUI object

*/