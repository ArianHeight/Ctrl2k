#include <Ctrl 2kPCH.h>

/*

Map Manager class

*/

MapManager::MapManager(TextureManager &source, glm::vec2 &ptw) :
	textureSource(source), pixelToWin(ptw)
{
	//cstr
}

//internal method used by the manager class
void MapManager::updateSize()
{
	this->m_bufferSize = this->m_buffer.size();
}

int MapManager::search(const std::string &name)
{
	for (int count = 0; count < this->m_bufferSize; count++)
	{
		if (this->m_buffer[count]->m_name == name)
		{
			return count;
		}
	}

	return -1;
}

std::string MapManager::getName(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		return this->m_buffer[index]->m_name;
	}

	return std::string("");
}

void MapManager::changeLoadState()
{
	if (this->m_state == PRELOAD_SURROUNDINGS_1)
	{
		this->m_state = LOAD_SCREEN_ENABLED;
	}
	else
	{
		this->m_state = PRELOAD_SURROUNDINGS_1;
	}
}

/*
std::thread::id MapManager::getActiveThread()
{
	if (this->m_threadIsActive)
	{
		return this->m_activeThread;
	}

	return std::thread::id();
}

bool MapManager::hasActiveThread()
{
	return this->m_threadIsActive;
}

void MapManager::newThread(std::thread::id id)
{
	this->m_activeThread = id;
	this->m_threadIsActive = true;
}

void MapManager::deactivateThread()
{
	if (this->m_threadIsActive)
	{
		this->m_threadIsActive = false;
		this->m_activeThread = std::thread::id();
	}
}

void MapManager::setThreadActive()
{
	this->m_threadIsActive = true;
}
*/
void MapManager::updateAllTextureBinds()
{
	for (int count = 0; count < this->m_bufferSize; count++)
	{
		this->m_buffer[count]->updateIndices();
	}
}

void MapManager::updateAllAdjMapIndices()
{
	for (int count = 0; count < this->m_bufferSize; count++)
	{
		this->m_buffer[count]->m_adjMapIndices.clear(); //clears the adjmapindices

		//iterates through stores the indices of adjacent maps into each map's adjmapindices
		this->m_buffer[count]->m_connected = this->m_buffer[count]->m_adjMaps.size();
		for (int i = 0; i < this->m_buffer[count]->m_connected; i++)
		{
			this->m_buffer[count]->m_adjMapIndices.push_back(this->search(this->m_buffer[count]->m_adjMaps[i]));
		}
		this->m_buffer[count]->m_adjMapIndices.shrink_to_fit();
	}
}

void MapManager::updateMapsLoaded()
{
	this->m_mapsLoaded.clear();
	this->m_mapsLoadedIndices.clear();
	for (int count = 0; count < this->m_bufferSize; count++) //finds currently loaded maps
	{
		if (this->m_buffer[count]->m_state == LOADED)
		{
			this->m_mapsLoaded.push_back(this->m_buffer[count]->m_name);
		}
	}
	for (int i = 0; i < this->m_mapsLoaded.size(); i++) //updates indices
	{
		this->m_mapsLoadedIndices.push_back(this->search(this->m_mapsLoaded[i]));
	}
}

//give in order tl, tr, bl, br
void MapManager::addConnector(int mapOneIndex, int mapTwoIndex, const FourPoints &mapOnePoints, const FourPoints &mapTwoPoints)
{
	//map 1
	this->m_buffer[mapOneIndex]->m_adjMaps.push_back(this->m_buffer[mapTwoIndex]->m_name); //add name
	this->m_buffer[mapOneIndex]->m_transPointsWP.push_back(mapOnePoints); //add points

	//map 2
	this->m_buffer[mapTwoIndex]->m_adjMaps.push_back(this->m_buffer[mapOneIndex]->m_name); //add name
	this->m_buffer[mapTwoIndex]->m_transPointsWP.push_back(mapTwoPoints); //add points
}

//give in order tl, tr, bl, br. connectors are the teleport squares that moves the player between maps
void MapManager::addConnector(const std::string &mapOne, const std::string &mapTwo, const FourPoints &mapOnePoints, const FourPoints &mapTwoPoints)
{
	this->addConnector(this->search(mapOne), this->search(mapTwo), mapOnePoints, mapTwoPoints);
}

void MapManager::useMap(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		if (this->m_state == PRELOAD_SURROUNDINGS_1)
		{
			bool inList = true;

			this->m_buffer[index]->load(); //loads most important map
			this->m_buffer[index]->calcCamClampValues(this->pixelToWin);

			for (int i = 0; i < this->m_mapsLoaded.size(); i++) //unloads unecessary maps
			{
				inList = false;
				for (int j = 0; j < this->m_buffer[index]->m_connected; j++) //checks if in list
				{
					if (this->m_mapsLoaded[i] == this->m_buffer[index]->m_name || this->m_mapsLoaded[i] == this->m_buffer[index]->m_adjMaps[j])
					{
						inList = true;
					}
				}

				if (!inList) //unloads if not needed
				{
					this->m_buffer[this->m_mapsLoadedIndices[i]]->unload();
				}
			}

			for (int i = 0; i < this->m_buffer[index]->m_connected; i++) //loads all necessary maps
			{
				this->m_buffer[this->m_buffer[index]->m_adjMapIndices[i]]->load();
			}

			this->updateMapsLoaded(); //updates this->m_mapsLoaded and its indices
		}
		else if (this->m_state == LOAD_SCREEN_ENABLED)
		{
			for (int i = 0; i < this->m_mapsLoaded.size(); i++) //unloads unecessary maps
			{
				this->m_buffer[this->m_mapsLoadedIndices[i]]->unload();
			}

			this->m_buffer[index]->load(); //loads the important map
			this->m_buffer[index]->calcCamClampValues(this->pixelToWin);

			this->updateMapsLoaded();
		}
	}
}

void MapManager::useMap(const std::string &name)
{
	this->useMap(this->search(name));
}

void MapManager::bindStandard(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		this->m_buffer[index]->bindStandard();
	}
}

void MapManager::bindStandard(const std::string &name)
{
	this->bindStandard(this->search(name));
}

void MapManager::unload(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		this->m_buffer[index]->unload();
	}
}

void MapManager::unload(const std::string &name)
{
	this->unload(this->search(name));
}

void MapManager::load(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		this->m_buffer[index]->load();
	}
}

void MapManager::load(const std::string &name)
{
	this->load(this->search(name));
}

int MapManager::create(const std::string &name, const std::string &visualName, const std::string &collisionName, const std::string &depthName, float size, glm::vec2 wp, float ar)
{
	int index = this->search(name);

	if (index != -1) //map exists or name overlap
	{
		return index;
	}

	//creates the map
	this->m_buffer.push_back(new Map(this->textureSource, name, visualName, collisionName, depthName, size, wp, ar));
	this->updateSize();

	index = this->search(name);
	return index;
}

bool MapManager::del(int index)
{
	if (index < 0 || index >= this->m_bufferSize) //out of range
	{
		return false;
	}

	delete this->m_buffer[index];
	this->m_buffer.erase(this->m_buffer.begin() + index);
	this->updateSize();

	return true;
}

bool MapManager::del(const std::string &name)
{
	return this->del(this->search(name));
}

Map* MapManager::data(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		return this->m_buffer[index];
	}
	return this->m_buffer[0];
}

Map* MapManager::data(const std::string &name)
{
	return this->data(this->search(name));
}

void MapManager::reserve(int size)
{
	if (size > this->m_bufferSize)
	{
		this->m_buffer.reserve(size);
	}
}

void MapManager::reserve(const std::string &name)
{
	this->reserve(this->search(name));
}

float MapManager::getSize(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		return this->m_buffer[index]->m_sizeWP;
	}
	return 0.0f;
}

float MapManager::getSize(const std::string &name)
{
	return this->getSize(this->search(name));
}

float MapManager::getAR(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		return this->m_buffer[index]->m_ar;
	}
	return 0.0f;
}

float MapManager::getAR(const std::string &name)
{
	return this->getAR(this->search(name));
}

glm::vec2 MapManager::getWP(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		return this->m_buffer[index]->m_wp;
	}
	return glm::vec2(0.0f);
}

glm::vec2 MapManager::getWP(const std::string &name)
{
	return this->getWP(this->search(name));
}

glm::vec2 MapManager::getMinCamWP(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		return this->m_buffer[index]->m_minCamWP;
	}
	return glm::vec2(0.0f);
}

glm::vec2 MapManager::getMinCamWP(const std::string &name)
{
	return this->getMinCamWP(this->search(name));
}

glm::vec2 MapManager::getMaxCamWP(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		return this->m_buffer[index]->m_maxCamWP;
	}
	return glm::vec2(0.0f);
}

glm::vec2 MapManager::getMaxCamWP(const std::string &name)
{
	return this->getMaxCamWP(this->search(name));
}

FourPoints MapManager::getTPArea(int index, int mapIndex)
{
	//mapIndex is the target area which is connected
	if (index >= 0 && index < this->m_bufferSize)
	{
		if (mapIndex >= 0 && mapIndex < this->m_buffer[index]->m_connected)
		{
			return this->m_buffer[index]->m_transPointsWP[mapIndex]; //the rectangle that teleports the player
		}
	}
	return FourPoints{glm::vec2(0.0f), glm::vec2(0.0f) , glm::vec2(0.0f) , glm::vec2(0.0f)};
}

FourPoints MapManager::getTPArea(const std::string &name, const std::string &map)
{
	int index = this->search(name);
	int mapIndex = 0;

	for (int i = 0; i < this->m_buffer[index]->m_connected; i++)
	{
		if (map == this->m_buffer[index]->m_adjMaps[i])
		{
			mapIndex = i;
			break;
		}
	}

	return this->getTPArea(index, mapIndex);
}


int MapManager::getTextureIndexR(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		return this->m_buffer[index]->m_textureIndices[0]; //returns the visual part of the map
	}
	return 0;
}

int MapManager::getTextureIndexR(std::string &name)
{
	return this->getTextureIndexR(this->search(name));
}

MapManager::~MapManager()
{
	//dstr
	for (Map *i : this->m_buffer)
	{
		i->unload();
	}
	this->m_buffer.clear();
}

/*

end Map Manager class

*/