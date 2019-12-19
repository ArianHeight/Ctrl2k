#include <Ctrl 2kPCH.h>

/*
Camera class
*/

Camera::Camera(RenderEngine &engine, const std::string &name, glm::vec2 position, double &time, int &windowW, int &windowH) :
	m_name(name), m_pos(position), m_timeRef(time), windowDimensions(glm::vec2(windowW, windowH)), 
	windowWidthRef(windowW), windowHeightRef(windowH), rEngine(engine)
{
	this->updateWindowDimensions();
}

//LERPs the position of camera from given camera center
void Camera::calculateSelfMovement()
{
	Component *transform = this->targetObj->getComponent(0); //ptr to the transformation that holds the position
	glm::vec2 objPos = transform->getDisplacement();

	this->m_pos = glm::mix(this->m_pos, objPos, this->m_timeRef * this->m_acceleration); //LERPS the position for "smooth" cam movement
	this->m_pos = glm::clamp(this->m_pos, this->m_minClamp, this->m_maxClamp); //clamps camera to edge of map
}

glm::vec2 Camera::getPos()
{
	return this->m_pos;
}

void Camera::setPos(glm::vec2 target)
{
	this->m_pos = target;
}

//moves camera by distance
void Camera::moveCam(glm::vec2 distance)
{
	this->m_pos += distance;
}

//updates window dimensions variable the camera uses
void Camera::updateWindowDimensions()
{
	this->windowDimensions = glm::vec2(this->windowWidthRef, this->windowHeightRef);
}

//adjusts camera snap
void Camera::adjustSnap(float acceleration)
{
	this->m_acceleration = acceleration;
}

//changes the min and max camera positional values
void Camera::changeClampValues(glm::vec2 &minCam, glm::vec2 &maxCam)
{
	this->m_minClamp = minCam;
	this->m_maxClamp = maxCam;
}

//method called by idle() every frame
void Camera::mainPhrase()
{
	if (this->targetObj != nullptr) //check if camera is centered on an object
	{
		this->calculateSelfMovement(); //calc movement if it is

		if (this->targetObj->getDelete()) //set ptr to nullptr if obj is getting deleted
		{
			this->targetObj = nullptr;
		}
	}
	this->updateWindowDimensions();
}

void Camera::setTarget(GameObject *obj)
{
	//cleans up for last target
	if (this->targetObj != nullptr && !this->targetObj->getDelete())
	{
		this->targetObj->setLinkCam(nullptr);
	}

	this->targetObj = obj;
	if (this->targetObj != nullptr) //ptr guard
	{
		this->targetObj->setLinkCam(this); //links camera to object
	}
}

//accessors
std::string Camera::getName()
{
	return this->m_name;
}

GameObject* Camera::getTarget()
{
	return this->targetObj;
}

Camera::~Camera()
{
	this->m_name.clear();
}

/*
end Camera class
*/