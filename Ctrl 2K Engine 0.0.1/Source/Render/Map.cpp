#include <Ctrl 2kPCH.h>

/*

Map object

*/

Map::Map(TextureManager &textureBuffer, std::string &name, std::string &visualName, std::string &collisionName, std::string &depthName, float size, glm::vec2 wp, float ar) :
	textureSource(textureBuffer), m_name(name), m_wp(wp), m_sizeWP(size), m_ar(ar)
{
	this->m_textureNames[0] = visualName;
	this->m_textureNames[1] = collisionName;
	this->m_textureNames[2] = depthName;

	this->updateIndices();
}

void Map::createCollisionBitMap()
{
	this->m_collisionMap = new CollisionMap(this->textureSource.getPath(this->m_textureIndices[1]), &this->m_sizeWP, this->m_wp);
	this->m_pixelDimensions = this->m_collisionMap->getDimensions();
	this->m_ar = (float)this->m_pixelDimensions.x / (float)this->m_pixelDimensions.y;
}

void Map::load()
{
	this->textureSource.activate(this->m_textureIndices[0]); //visual
	this->textureSource.activate(this->m_textureIndices[2]); //depth
	if (this->m_state == UNLOADED)
	{
		this->createCollisionBitMap();
	}
	this->m_state = LOADED;
}

void Map::unload()
{
	this->textureSource.disable(this->m_textureIndices[0]); //visual
	this->textureSource.disable(this->m_textureIndices[2]); //depth
	if (this->m_state == LOADED)
	{
		delete this->m_collisionMap;
		this->m_collisionMap = nullptr;
	}
	this->m_state = UNLOADED;
}

void Map::bindStandard()
{
	assert(this->m_state == LOADED, "Please load map before binding it....");
	glActiveTexture(GL_TEXTURE0);
	this->textureSource.bind(this->m_textureIndices[0]); //visual
	glActiveTexture(GL_TEXTURE1);
	this->textureSource.bind(this->m_textureIndices[2]); //depth
}

void Map::updateIndices()
{
	for (int i = 0; i < 3; i++)
	{
		this->m_textureIndices[i] = this->textureSource.search(this->m_textureNames[i]);
	}
}

//calculates camera clamps based on map size and such
void Map::calcCamClampValues(glm::vec2 pixelToWin)
{
	if (pixelToWin.x * pixelToWin.y == 0)
	{
		exit(-1);
	}

	glm::vec2 halfCamLength = 1.0f / pixelToWin; //pixelToWin is the single pixel to window size ratio

	//WP stands for world position
	this->m_minCamWP = glm::vec2(-this->m_ar * this->m_sizeWP + halfCamLength.x, -this->m_sizeWP + halfCamLength.y) + this->m_wp;
	this->m_maxCamWP = glm::vec2(this->m_ar * this->m_sizeWP - halfCamLength.x, this->m_sizeWP - halfCamLength.y) + this->m_wp;

	//adjusts x min and max if map is too small
	if (this->m_maxCamWP.x < this->m_minCamWP.x)
	{
		this->m_maxCamWP.x = (this->m_maxCamWP.x + this->m_minCamWP.x) / 2.0f;
		this->m_minCamWP.x = this->m_maxCamWP.x;
	}
	//adjusts y min and max if map is too small
	if (this->m_maxCamWP.y < this->m_minCamWP.y)
	{
		this->m_maxCamWP.y = (this->m_maxCamWP.y + this->m_minCamWP.y) / 2.0f;
		this->m_minCamWP.y = this->m_maxCamWP.y;
	}
}

void Map::render()
{
	RenderEngine& rEngine = this->game->getEngine().getRenderEngine(); //by this point, this->game is expected to be a not nullptr
	rEngine.drawMap(this->game->getMapIndex()); //render call
}

//returns a pointer to the collision map
CollisionMap* Map::getCollisionData()
{
	if (this->m_state == LOADED)
	{
		return this->m_collisionMap;
	}
	return nullptr;
}

std::string Map::getName()
{
	return this->m_name;
}

LoadState Map::getState()
{
	return this->m_state;
}

Map::~Map()
{
	if (this->m_collisionMap != nullptr)
	{
		delete[3] this->m_textureNames;
		delete[3] this->m_textureIndices;

		delete this->m_collisionMap;
		this->m_collisionMap = nullptr;
	}

	this->m_transPointsWP.clear();
}

/*

end Map object

*/