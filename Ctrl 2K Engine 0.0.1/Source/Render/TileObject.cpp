#include <Ctrl 2kPCH.h>

/*
TileObject class
*/

//tileObject is actually a playable map
TileObject::TileObject(TileType format, std::string name, GLuint texture, GLuint depth, GLuint normal, float unitsWidth, float aspectRatio) :
	m_type(format), m_name(name), m_texture(texture), m_depthMap(depth), m_normalMap(normal), m_unitsWidth(unitsWidth), m_ar(aspectRatio)
{
	if (m_ar == 1.0)
	{
		this->updateAR();
	}

	//creates the collision map if this tile is a playground map
	if (this->m_type == PLAYGROUND_TILE)
	{
		this->createCollisionBitMap();
	}
}

//updates the aspect ratio of the map
void TileObject::updateAR()
{
	int width {0};
	int height {0};

	glBindTexture(GL_TEXTURE_2D, this->m_texture);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	glBindTexture(GL_TEXTURE_2D, 0);

	this->m_ar = (float)width / (float)height;
}

void TileObject::createCollisionBitMap()
{
	this->m_collisionMap = new CollisionMap(this->m_depthMap, &this->m_unitsWidth);
	//std::string temp1 = "Resource/Texture/bgTestDepth.png";
	//this->m_collisionMap = new CollisionMap(temp1, &this->m_unitsWidth);
}

TileObject::~TileObject()
{
	if (this->m_collisionMap != nullptr)
	{
		delete this->m_collisionMap;
		this->m_collisionMap = nullptr;
	}
}

/*
end TileObject
*/