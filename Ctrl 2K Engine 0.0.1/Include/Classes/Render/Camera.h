#pragma once

/*
Camera Class
*/

//pre-declaration
class RenderEngine;

class Camera
{
public:
	Camera(RenderEngine &engine, const std::string &name, glm::vec2 position, double &time, int &windowW, int &windowH);
	~Camera();

private:
	RenderEngine &rEngine;
	GameObject *targetObj = nullptr; //target object
	glm::vec2 m_pos;
	std::string m_name;
	
	//references to window width and height in the game window
	int &windowWidthRef;
	int &windowHeightRef;

	glm::vec2 windowDimensions;
	float m_acceleration = 7.0; //in worldPosUnits / second squared, adjusts the 'snap' of the camera
	glm::vec2 m_maxClamp;
	glm::vec2 m_minClamp;

	void calculateSelfMovement(); //lerps camera pos to player pos

public:
	double &m_timeRef;

	glm::vec2 getPos();
	void setPos(glm::vec2 target); //set camera position
	void moveCam(glm::vec2 distance); //moves camera by set distance
	void mainPhrase(); //main updater
	void updateWindowDimensions();
	void adjustSnap(float acceleration); //changes m_acceleration
	void changeClampValues(const glm::vec2 &minCam, const glm::vec2 &maxCam);
	void setTarget(GameObject *obj); //sets the camera to be centered on a game object

	std::string getName();
	GameObject* getTarget();
};

/*
end Camera Class
*/