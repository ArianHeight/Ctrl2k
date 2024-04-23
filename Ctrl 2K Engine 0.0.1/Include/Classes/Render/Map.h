#pragma once

/*

Map object

*/

//friends
class MapManager;
class PhysEngine;
class GameObject;

class Map : public GameObject
{
public:
	Map(TextureManager &textureBuffer, const std::string &name, const std::string &visualName, const std::string &collisionName, const std::string &depthName, float size = 1.0f, glm::vec2 wp = glm::vec2(0.0f), float ar = 1.0f);
	~Map();

	friend class MapManager;
	friend class PhysEngine;

private:
	TextureManager &textureSource;

	std::string m_name;
	LoadState m_state{UNLOADED};
	float m_sizeWP;
	float m_ar; //aspect ratio, width / height
	glm::vec2 m_wp; //world position
	glm::vec2 m_pixelDimensions; //of playground collision

	//visual playground layer, playground collision, visual depth
	std::string m_textureNames[3];
	int m_textureIndices[3];

	//adjacent maps and connection points
	int m_connected{0};
	std::vector<std::string> m_adjMaps;
	std::vector<int> m_adjMapIndices;
	std::vector<FourPoints> m_transPointsWP; //tl, tr, bl, br

	//for camera stuff
	glm::vec2 m_minCamWP; //0, 0 on screen
	glm::vec2 m_maxCamWP; //1, 1 on screen

	CollisionMap *m_collisionMap = nullptr; //collisionMap object

	void createCollisionBitMap();

public:
	void load(); //does not apply to playground collision
	void unload(); //does not apply to playground collision
	void bindStandard(); //visual, depth

	void changeSource(TextureManager &textureBuffer); //change texture source??
	void updateIndices(); //updates m_adjMapIndices
	void calcCamClampValues(glm::vec2 pixelToWin);

	virtual void render();

	CollisionMap* getCollisionData();
	virtual std::string getName();
	LoadState getState();
};

/*

end Map object

*/