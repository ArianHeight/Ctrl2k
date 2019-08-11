#pragma once

/*

Map Manager class

*/

class MapManager
{
public:
	MapManager(TextureManager &source, glm::vec2 &ptw);
	~MapManager();

	int m_bufferSize{0};
	PreLoadState m_state{PRELOAD_SURROUNDINGS_1};
	//PreLoadState m_state{ LOAD_SCREEN_ENABLED }; //temporary
	std::vector<std::string> m_mapsLoaded;
	std::vector<int> m_mapsLoadedIndices;

private:
	TextureManager &textureSource;
	glm::vec2 &pixelToWin;
	std::vector<Map*> m_buffer;
	/*std::thread::id m_activeThread;
	bool m_threadIsActive = false;*/

	void updateSize();

public:
	int search(std::string &name);
	std::string getName(int index);
	void changeLoadState();
	/*std::thread::id getActiveThread(); //returns thread id
	bool hasActiveThread(); //returns whether or not mgr has a thread running
	void newThread(std::thread::id id); //puts a new thread id in, please note this does not delete thread from threadManager
	void deactivateThread(); //deletes activeThread
	void setThreadActive();*/

	void updateAllTextureBinds();
	void updateAllAdjMapIndices();
	void updateMapsLoaded();

	//adds teleport area
	void addConnector(std::string &mapOne, std::string &mapTwo, FourPoints &mapOnePoints, FourPoints &mapTwoPoints);
	void addConnector(int mapOneIndex, int mapTwoIndex, FourPoints &mapOnePoints, FourPoints &mapTwoPoints);

	//useMap which has preloading and loading screen stuff
	void useMap(std::string &name);
	void useMap(int index);

	//binds map texture
	void bindStandard(std::string &name);
	void bindStandard(int index);

	//unloads a map
	void unload(std::string &name);
	void unload(int index);

	//loads a map into video memory and RAM
	void load(std::string &name);
	void load(int index);

	//makes new map given info
	int create(std::string &name, std::string &visualName, std::string &collisionName, std::string &depthName, float size = 1.0f, glm::vec2 wp = glm::vec2(0.0f), float ar = 1.0f);

	//deletes map
	bool del(std::string &name);
	bool del(int index);

	//ptr to map
	Map* data(std::string &name);
	Map* data(int index);

	//reserves amount of space in buffer
	void reserve(std::string &name);
	void reserve(int size);

	//number of maps
	float getSize(std::string &name);
	float getSize(int index);

	//aspect ratio of map
	float getAR(std::string &name);
	float getAR(int index);

	glm::vec2 getWP(std::string &name); //gets world position of map
	glm::vec2 getWP(int index); //gets world position of map

	//camera limit world positions of the 4 edges
	glm::vec2 getMinCamWP(std::string &name);
	glm::vec2 getMinCamWP(int index);
	glm::vec2 getMaxCamWP(std::string &name);
	glm::vec2 getMaxCamWP(int index);

	//teleport areas which lead to other maps
	FourPoints getTPArea(std::string &name, std::string &map);
	FourPoints getTPArea(int index, int mapIndex);

	//texture index for Rendering texture
	int getTextureIndexR(std::string &name);
	int getTextureIndexR(int index);
};

/*

end Map Manager class

*/