#include <Ctrl 2kPCH.h>

/*

Collision Texture Object

*/

CollisionMap::CollisionMap(GLuint map, float *mapBigness, glm::vec2 worldPos, float size) :
	mapSize(mapBigness), m_texture(map), m_worldPos(worldPos), m_size(size)
{
	glBindTexture(GL_TEXTURE_2D, this->m_texture); //binds texture stuffs
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &m_width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &m_height);

	this->m_tlWorldPos = this->m_worldPos - glm::vec2(this->m_width / 2.0, this->m_height / 2.0) * this->m_size;

	GLubyte *depth;
	depth = new GLubyte[4 * this->m_width * this->m_height];

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, depth); //grab depth
	glBindTexture(GL_TEXTURE_2D, 0);

	//re-organize depth values into a 2D array 
	this->m_map = new float*[this->m_width];
	for (int i = 0; i < this->m_width; i++)
	{
		this->m_map[i] = new float[this->m_height];
	}

	int index = 0;
	for (int j = this->m_height - 1; j >= 0; j--)
	{
		for (int i = 0; i < this->m_width; i++, index += 4)
		{
			this->m_map[i][j] = depth[index] / 255.0;
		}
	}

	delete[] depth;
	depth = nullptr;
	/*
	this->m_corners.push_back(glm::vec2(-1.0, 1.0) * glm::vec2((float)this->m_width / (float)this->m_height, 1.0f) * *this->mapSize + this->m_worldPos);
	this->m_corners.push_back(glm::vec2(1.0, 1.0) * glm::vec2((float)this->m_width / (float)this->m_height, 1.0f) * *this->mapSize + this->m_worldPos);
	this->m_corners.push_back(glm::vec2(-1.0, -1.0) * glm::vec2((float)this->m_width / (float)this->m_height, 1.0f) * *this->mapSize + this->m_worldPos);
	this->m_corners.push_back(glm::vec2(1.0, -1.0) * glm::vec2((float)this->m_width / (float)this->m_height, 1.0f) * *this->mapSize + this->m_worldPos);
	*/

	this->m_corners->tl = (glm::vec2(-1.0, 1.0) * glm::vec2((float)this->m_width / (float)this->m_height, 1.0f) * *this->mapSize + this->m_worldPos);
	this->m_corners->tr = (glm::vec2(1.0, 1.0) * glm::vec2((float)this->m_width / (float)this->m_height, 1.0f) * *this->mapSize + this->m_worldPos);
	this->m_corners->bl = (glm::vec2(-1.0, -1.0) * glm::vec2((float)this->m_width / (float)this->m_height, 1.0f) * *this->mapSize + this->m_worldPos);
	this->m_corners->br = (glm::vec2(1.0, -1.0) * glm::vec2((float)this->m_width / (float)this->m_height, 1.0f) * *this->mapSize + this->m_worldPos);

	this->m_worldToPixel = glm::vec2((float)(this->m_width - 1.0f) / (this->m_corners->tr.x - this->m_corners->tl.x), (this->m_height - 1.0f) / (this->m_corners->tl.y - this->m_corners->bl.y));
}

CollisionMap::CollisionMap(const std::string &mapPath, float *mapBigness, glm::vec2 worldPos, float size) :
	mapSize(mapBigness), m_worldPos(worldPos), m_size(size)
{
	GLubyte *depth = SOIL_load_image(mapPath.c_str(), &this->m_width, &this->m_height, 0, SOIL_LOAD_RGBA); //loads depth map into a 1D array
	this->m_tlWorldPos = this->m_worldPos - glm::vec2(this->m_width / 2.0, this->m_height / 2.0) * this->m_size;

	//re-organize depth into a 2D array
	this->m_map = new float*[this->m_width];
	for (int i = 0; i < this->m_width; i++)
	{
		this->m_map[i] = new float[this->m_height];
	}
	int index = 0;
	for (int j = 0; j < this->m_height; j++)
	{
		for (int i = 0; i < this->m_width; i++, index += 4)
		{
			this->m_map[i][j] = depth[index] / 255.0;
		}
	}

	SOIL_free_image_data(depth);
	depth = nullptr;
	/*
	this->m_corners.push_back(glm::vec2(-1.0, 1.0) * glm::vec2((float)this->m_width / (float)this->m_height, 1.0f) * *this->mapSize + this->m_worldPos);
	this->m_corners.push_back(glm::vec2(1.0, 1.0) * glm::vec2((float)this->m_width / (float)this->m_height, 1.0f) * *this->mapSize + this->m_worldPos);
	this->m_corners.push_back(glm::vec2(-1.0, -1.0) * glm::vec2((float)this->m_width / (float)this->m_height, 1.0f) * *this->mapSize + this->m_worldPos);
	this->m_corners.push_back(glm::vec2(1.0, -1.0) * glm::vec2((float)this->m_width / (float)this->m_height, 1.0f) * *this->mapSize + this->m_worldPos);

	this->m_worldToPixel = glm::vec2((float)(this->m_width - 1.0f) / (this->m_corners[1].x - this->m_corners[0].x), (this->m_height - 1.0f) / (this->m_corners[0].y - this->m_corners[2].y));
	*/

	//updates the map corners in world space
	this->m_corners->tl = (glm::vec2(-1.0, 1.0) * glm::vec2((float)this->m_width / (float)this->m_height, 1.0f) * *this->mapSize + this->m_worldPos);
	this->m_corners->tr = (glm::vec2(1.0, 1.0) * glm::vec2((float)this->m_width / (float)this->m_height, 1.0f) * *this->mapSize + this->m_worldPos);
	this->m_corners->bl = (glm::vec2(-1.0, -1.0) * glm::vec2((float)this->m_width / (float)this->m_height, 1.0f) * *this->mapSize + this->m_worldPos);
	this->m_corners->br = (glm::vec2(1.0, -1.0) * glm::vec2((float)this->m_width / (float)this->m_height, 1.0f) * *this->mapSize + this->m_worldPos);

	this->m_worldToPixel = glm::vec2((float)(this->m_width - 1.0f) / (this->m_corners->tr.x - this->m_corners->tl.x), (this->m_height - 1.0f) / (this->m_corners->tl.y - this->m_corners->bl.y));
}

void CollisionMap::updateWindowAR(float ar)
{
	this->windowAR = ar;
}

//converts a world space coord into a pixel space coord
glm::vec2 CollisionMap::toPixelSpace(glm::vec2 worldPos)
{
	/*
	glm::vec2 imagePos = worldPos + glm::vec2(*this->mapSize * this->windowAR, *this->mapSize);
	imagePos.y = (*this->mapSize * 2.0f) - imagePos.y;
	imagePos *= glm::vec2((this->m_width - 1.0f) / ((*this->mapSize * 2.0f) * this->windowAR), (this->m_height - 1.0f) / (*this->mapSize * 2.0f));
	*/

	glm::vec2 imagePos;
	imagePos.x = (worldPos.x - this->m_corners->tl.x); //offsets x
	imagePos.y = this->m_corners->tl.y - worldPos.y; //invert y
	imagePos *= this->m_worldToPixel; //scale

	return imagePos;
}

//converts a pixel space coord into a world space coord
glm::vec2 CollisionMap::toWorldSpace(glm::vec2 pixelPos)
{
	glm::vec2 worldPos = pixelPos / this->m_worldToPixel; //scale
	worldPos.y = -worldPos.y + this->m_corners->tl.y; //inverts y
	worldPos.x = this->m_corners->tl.x + worldPos.x;//offsets x

	return worldPos;
}

//scales pixel space lengths into world space lengths
glm::vec2 CollisionMap::toWorldSize(glm::vec2 pixelPos)
{
	glm::vec2 worldPos = pixelPos / this->m_worldToPixel;

	return worldPos;
}

float CollisionMap::getPixel(glm::ivec2 pos)
{
	return this->m_map[pos.x][pos.y];
}

//returns the nearest pixel coord in pixel space
glm::ivec2 CollisionMap::getNearestPixelPosPixelSpace(glm::vec2 pos)
{
	//x to nearest whole number
	if (pos.x - (int)pos.x >= 0.5)
	{
		pos.x = (int)pos.x + 1;
	}
	else
	{
		pos.x = (int)pos.x;
	}
	//y to nearest whole number
	if (pos.y - (int)pos.y >= 0.5)
	{
		pos.y = (int)pos.y + 1;
	}
	else
	{
		pos.y = (int)pos.y;
	}

	return glm::ivec2(pos);
}

glm::ivec2 CollisionMap::getNearestPixelPos(glm::vec2 pos)
{
	//adjusts to pixel space
	pos = this->toPixelSpace(pos);

	//returns the nearest pixel in pixelSpace
	return this->getNearestPixelPosPixelSpace(pos);
}

float CollisionMap::getNearestPixel(glm::vec2 pos)
{
	return getPixel(getNearestPixelPos(pos));
}

glm::vec2 CollisionMap::getDimensions()
{
	return glm::vec2(this->m_width, this->m_height);
}

//using lerp to get the averaged depth value at the given position in world pos
float CollisionMap::getLinearPixel(glm::vec2 pos)
{
	//adjusts to pixel pos
	pos = this->toPixelSpace(pos);

	//multiplier weight on bottom right corner
	float xWeight = (pos.x - (int)pos.x) / 2.0f;
	float yWeight = (pos.y - (int)pos.y) / 2.0f;

	//averages everything using the weights
	float depth = this->m_map[(int)pos.x][(int)pos.y] * (xWeight + yWeight);
	depth += this->m_map[(int)pos.x + 1][(int)pos.y] * ((1.0f - xWeight) + yWeight);
	depth = this->m_map[(int)pos.x][(int)pos.y + 1] * (xWeight + (1.0f - yWeight));
	depth = this->m_map[(int)pos.x][(int)pos.y] * ((1.0f - xWeight) + (1.0f - yWeight));
	
	return depth;
}

//coarse collision check with map
bool CollisionMap::mapCollisionCheck(Component *objTransform, Component *hitbox)
{
	FourPoints hb = *(hitbox->getPhysicsObject()->getHB()) * objTransform->getSize() + objTransform->getDisplacement(); //moves to correct world pos

	glm::vec2 objCorners[3]; //world space hitbox
	objCorners[0] = hb.tl;
	objCorners[1] = hb.tr;
	objCorners[2] = hb.br;

	glm::ivec2 corners[4]; //pixel space "hitbox"
	corners[0] = glm::ivec2(this->toPixelSpace(objCorners[0])); //topleft
	corners[1] = glm::ivec2(this->toPixelSpace(objCorners[1])); //topright
	corners[2] = glm::ivec2(this->toPixelSpace(objCorners[2])); //bottom left
	corners[1].x += 1;
	corners[2].y += 1;

	if (corners[0].x < 0 || corners[1].x >(this->m_width - 1) || corners[0].y < 0 || corners[2].y >(this->m_height - 1)) //out of map range, no movement calculated
	{
		return false;
	}

	float bitValue = 1.0f;
	//sides minus corners, if touching will return true
	for (int i = corners[0].x; i < corners[1].x; i++) //up and down, x value changes only
	{
		for (int jIndex = 0; jIndex < 3; jIndex += 2)
		{
			bitValue = this->m_map[i][corners[jIndex].y];
			if (bitValue < 0.01f) //height diff greater than 0.01 means upassable wall
			{
				return true;
			}
		}
	}
	for (int j = corners[0].y; j < corners[2].y; j++) //left and right
	{
		for (int iIndex = 0; iIndex < 2; iIndex += 1)
		{
			bitValue = this->m_map[corners[iIndex].x][j];
			if (bitValue < 0.01f) //height diff greater than 0.01 means upassable wall
			{
				return true;
			}
		}
	}
	//end side testing

	return false;
}

//returns a manifest of the collision response
Manifest CollisionMap::mapCollisionResponse(Component *objTransform, Component *hitbox)
{
	FourPoints hb = *(hitbox->getPhysicsObject()->getHB()) * objTransform->getSize() + objTransform->getDisplacement(); //moves to correct world pos
	glm::vec2 centeredWorldPos = objTransform->getDisplacement(); //center of obj in world pos

	//hitbox in world position
	glm::vec2 objCorners[3];
	objCorners[0] = hb.tl;
	objCorners[1] = hb.tr;
	objCorners[2] = hb.br;

	//hitbox in pixel pos
	glm::ivec2 corners[4];
	corners[0] = glm::ivec2(this->toPixelSpace(objCorners[0])); //topleft
	corners[1] = glm::ivec2(this->toPixelSpace(objCorners[1])); //topright
	corners[2] = glm::ivec2(this->toPixelSpace(objCorners[2])); //bottom left
	corners[0].x--; //offsets the corners to counter inaccuracy with int based pixel positions
	corners[0].y--;
	corners[1].x++;
	corners[1].y--;
	corners[2].x--;
	corners[2].y++;

	if (corners[0].x < 0 || corners[1].x >(this->m_width - 1) || corners[0].y < 0 || corners[2].y >(this->m_height - 1)) //out of map range, no movement calculated
	{
		return Manifest{glm::vec2(0.0f), 0.0f, 0.0f}; //early return
	}

	float bitValue = 0.0f;
	int moreBitCount = 0;
	int lessBitCount = 0;
	int totalCount = 0;
	glm::vec2 centerDiff = glm::vec2((float)(corners[1].x - corners[0].x) / 2.0f, (float)(corners[2].y - corners[0].y) / 2.0f);
	glm::vec2 center = glm::vec2(corners[0]) + centerDiff;
	glm::vec2 moreBits = glm::vec2(0.0f);
	glm::vec2 lessBits = glm::vec2(0.0f);
	glm::vec2 tempBits = glm::vec2(0.0f);
	glm::vec2 pixelPos = glm::vec2(0.0f);
	glm::ivec4 touchingSides(0); //up down left right
	FourPoints positionalVecs { glm::vec2(0.0f, centerDiff.y), glm::vec2(0.0f, -centerDiff.y) , glm::vec2(centerDiff.x, 0.0f) , glm::vec2(-centerDiff.x, 0.0f) }; //up down left right, convex shape
	//relative corner - topleft.xy, topright.xy, bottomleft.xy, bottomright.xy
	FourPoints reverseVecs { glm::vec2(centerDiff.x, -centerDiff.y), glm::vec2(-centerDiff) , glm::vec2(centerDiff) , glm::vec2(-centerDiff.x, centerDiff.y) };
	FourPoints reverseCAreas { glm::vec2(-centerDiff.x, centerDiff.y), glm::vec2(centerDiff) , glm::vec2(-centerDiff) , glm::vec2(centerDiff.x, -centerDiff.y) }; //same as top, x is area
	glm::vec2 actualCorner = glm::vec2(0.0f);
	reverseVecs.tl = this->toWorldSize(reverseVecs.tl);
	reverseVecs.tr = this->toWorldSize(reverseVecs.tr);
	reverseVecs.bl = this->toWorldSize(reverseVecs.bl);
	reverseVecs.br = this->toWorldSize(reverseVecs.br);

	//sides minus corners, updates touching sides and various values needed for generalized direction testing
	for (int i = corners[0].x; i < corners[1].x + 1; i++) //up and down, x value changes only
	{
		for (int jIndex = 0; jIndex < 3; jIndex += 2)
		{
			bitValue = this->m_map[i][corners[jIndex].y];
			pixelPos = this->toWorldSpace(glm::vec2(i, corners[jIndex].y));
			pixelPos = glm::vec2(pixelPos.x - centeredWorldPos.x, -centeredWorldPos.y + objCorners[jIndex].y);
			//pixelPos = glm::vec2((float)i - center.x, center.y - (float)corners[jIndex].y);
			//pixelPos = this->toWorldSize(pixelPos);
			if (bitValue > 0.99f) //lower
			{
				lessBits += pixelPos;
				lessBitCount++;
			}
			else if (bitValue < 0.01f) //lower than 0.01
			{
				tempBits = pixelPos;
				moreBits += tempBits;
				moreBitCount++;

				if (!(i == corners[0].x || i == corners[1].x)) //corners don't count
				{
					touchingSides[jIndex / 2] = 1; //register touch, 0 / 2 = 0 which is x, 2 / 2 = 1 which is y
				}

				//relative corner updates
				if (jIndex == 0) //top only
				{
					if (pixelPos.x < reverseVecs.tl.x)
					{
						reverseVecs.tl.x = pixelPos.x;
					}
					if (pixelPos.x > reverseVecs.tr.x)
					{
						reverseVecs.tr.x = pixelPos.x;
					}
				}
				else //bottom only
				{
					if (pixelPos.x < reverseVecs.bl.x)
					{
						reverseVecs.bl.x = pixelPos.x;
					}
					if (pixelPos.x > reverseVecs.br.x)
					{
						reverseVecs.br.x = pixelPos.x;
					}
				}
			}
			totalCount++;
		}
	}
	for (int j = corners[0].y; j < corners[2].y + 1; j++) //left and right
	{
		for (int iIndex = 0; iIndex < 2; iIndex += 1)
		{
			bitValue = this->m_map[corners[iIndex].x][j];
			pixelPos = this->toWorldSpace(glm::vec2(corners[iIndex].x, j));
			pixelPos = glm::vec2(objCorners[iIndex].x - centeredWorldPos.x, -centeredWorldPos.y + pixelPos.y);
			//pixelPos = glm::vec2((float)corners[iIndex].x - center.x, center.y - (float)j);
			//pixelPos = this->toWorldSize(pixelPos);
			if (bitValue > 0.99f) //lower
			{
				lessBits += pixelPos;
				lessBitCount++;
			}
			else if (bitValue < 0.01f) //lower than 0.01
			{
				tempBits = pixelPos;
				moreBits += tempBits;
				moreBitCount++;

				if (!(j == corners[0].y || j == corners[2].y)) //corners don't count
				{
					touchingSides[iIndex + 2] = 1; //register touch, 0 + 2 = 2 which is z, 1 + 2 = 3 which is w
				}

				//relative corner updates
				if (iIndex == 0) //left only
				{
					if (pixelPos.y > reverseVecs.tl.y)
					{
						reverseVecs.tl.y = pixelPos.y;
					}
					if (pixelPos.y < reverseVecs.bl.y)
					{
						reverseVecs.bl.y = pixelPos.y;
					}
				}
				else //right only
				{
					if (pixelPos.y > reverseVecs.tr.y)
					{
						reverseVecs.tr.y = pixelPos.y;
					}
					if (pixelPos.y < reverseVecs.br.y)
					{
						reverseVecs.br.y = pixelPos.y;
					}
				}
			}
			totalCount++;
		}
	}
	//end side testing

	//calculate touching sides
	int sidesTouching = touchingSides.x + touchingSides.y + touchingSides.z + touchingSides.w;
	if (sidesTouching > 3)
	{
		touchingSides = glm::ivec4(0);
		if (moreBits.x > 0)
		{
			touchingSides.w = 1;
		}
		else if (moreBits.x < 0)
		{
			touchingSides.z = 1;
		}
		if (moreBits.y > 0)
		{
			touchingSides.x = 1;
		}
		else if (moreBits.y < 0)
		{
			touchingSides.y = 1;
		}
		sidesTouching = touchingSides.x + touchingSides.y + touchingSides.z + touchingSides.w;
	}

	centerDiff = glm::vec2(objCorners[1].x - centeredWorldPos.x, objCorners[2].y - centeredWorldPos.y);
	center = centeredWorldPos;
	positionalVecs.tl = this->toWorldSize(positionalVecs.tl);
	positionalVecs.tr = this->toWorldSize(positionalVecs.tr);
	positionalVecs.bl = this->toWorldSize(positionalVecs.bl);
	positionalVecs.br = this->toWorldSize(positionalVecs.br);
	reverseCAreas.tl = this->toWorldSize(reverseCAreas.tl);
	reverseCAreas.tr = this->toWorldSize(reverseCAreas.tr);
	reverseCAreas.bl = this->toWorldSize(reverseCAreas.bl);
	reverseCAreas.br = this->toWorldSize(reverseCAreas.br);

	//full reiteration
	glm::vec2 singlePxl = this->toWorldSize(glm::vec2(1));
	std::vector<glm::vec2> positions;
	for (int i = corners[0].x; i < corners[1].x + 1; i++)
	{
		positions.clear();
		for (int j = corners[0].y; j < corners[2].y + 1; j++)
		{
			bitValue = this->m_map[i][j];
			pixelPos = this->toWorldSpace(glm::ivec2(i, j));
			pixelPos = glm::vec2(pixelPos.x - center.x, -center.y + pixelPos.y);

			if (bitValue < 0.01f) //gets the approximate shape of collision inside entity hitbox
			{
				positions.push_back(pixelPos);
				//convex collision shape
				if (pixelPos.y > positionalVecs.tr.y)
				{
					positionalVecs.tr.y = pixelPos.y;
				}
				if (pixelPos.y < positionalVecs.tl.y)
				{
					positionalVecs.tl.y = pixelPos.y;
				}
				if (pixelPos.x < positionalVecs.bl.x)
				{
					positionalVecs.bl.x = pixelPos.x;
				}
				if (pixelPos.x > positionalVecs.br.x)
				{
					positionalVecs.br.x = pixelPos.x;
				}
				//concave collision shape
				if (sidesTouching > 1)
				{
					pixelPos.x -= singlePxl.x;
					pixelPos.y += singlePxl.y;
					if (pixelPos.x < reverseVecs.tl.x && pixelPos.y > reverseVecs.tl.y) //topleft change
					{
						if (abs((reverseCAreas.tl.x - pixelPos.x) * abs(reverseCAreas.tl.y - reverseVecs.tl.y)) > abs((reverseCAreas.tl.x - reverseVecs.tl.x) * abs(reverseCAreas.tl.y - pixelPos.y)))
						{
							reverseVecs.tl.x = pixelPos.x;
						}
						else
						{
							reverseVecs.tl.y = pixelPos.y;
						}
					}
					pixelPos.x += 2 * singlePxl.x;
					if (pixelPos.x > reverseVecs.tr.x && pixelPos.y > reverseVecs.tr.y) //topright
					{
						if (abs((reverseCAreas.tr.x - pixelPos.x) * abs(reverseCAreas.tr.y - reverseVecs.tr.y)) > abs((reverseCAreas.tr.x - reverseVecs.tr.x) * abs(reverseCAreas.tr.y - pixelPos.y)))
						{
							reverseVecs.tr.x = pixelPos.x;
						}
						else
						{
							reverseVecs.tr.y = pixelPos.y;
						}
					}
				}
			} //pass depth
		} //y
		for (int j = positions.size() - 1; j >= 0 ; j--)
		{
			pixelPos = positions[j];

			if (sidesTouching > 1)
			{
				pixelPos.x -= singlePxl.x;
				pixelPos.y -= singlePxl.y;
				if (pixelPos.x < reverseVecs.bl.x && pixelPos.y < reverseVecs.bl.y) //bottomleft
				{
					if (abs((reverseCAreas.bl.x - pixelPos.x) * abs(reverseCAreas.bl.y - reverseVecs.bl.y)) > abs((reverseCAreas.bl.x - reverseVecs.bl.x) * abs(reverseCAreas.bl.y - pixelPos.y)))
					{
						reverseVecs.bl.x = pixelPos.x;
					}
					else
					{
						reverseVecs.bl.y = pixelPos.y;
					}
				}
				pixelPos.x += 2 * singlePxl.x;
				if (pixelPos.x > reverseVecs.br.x && pixelPos.y < reverseVecs.br.y) //bottomright
				{
					if (abs((reverseCAreas.br.x - pixelPos.x) * abs(reverseCAreas.br.y - reverseVecs.br.y)) > abs((reverseCAreas.br.x - reverseVecs.br.x) * abs(reverseCAreas.br.y - pixelPos.y)))
					{
						reverseVecs.br.x = pixelPos.x;
					}
					else
					{
						reverseVecs.br.y = pixelPos.y - singlePxl.y;
					}
				}
			}
		}
	} //x

	//calculate corner touch areas
	/*
	for (int count = 0; count < 4; count++)
	{
		reverseCAreas[count].x = abs(reverseCAreas[count].x - reverseVecs[count].x) * abs(reverseCAreas[count].y - reverseVecs[count].y);
		reverseCAreas[count].y = 0;
	}*/
	reverseCAreas.tl.x = abs(reverseCAreas.tl.x - reverseVecs.tl.x) * abs(reverseCAreas.tl.y - reverseVecs.tl.y);
	reverseCAreas.tl.y = 0;
	reverseCAreas.tr.x = abs(reverseCAreas.tr.x - reverseVecs.tr.x) * abs(reverseCAreas.tr.y - reverseVecs.tr.y);
	reverseCAreas.tr.y = 0;
	reverseCAreas.bl.x = abs(reverseCAreas.bl.x - reverseVecs.bl.x) * abs(reverseCAreas.bl.y - reverseVecs.bl.y);
	reverseCAreas.bl.y = 0;
	reverseCAreas.br.x = abs(reverseCAreas.br.x - reverseVecs.br.x) * abs(reverseCAreas.br.y - reverseVecs.br.y);
	reverseCAreas.br.y = 0;

	if (sidesTouching == 2 && (touchingSides.x + touchingSides.y != 2) && (touchingSides.z + touchingSides.w != 2))
	{
		moreBits = -moreBits; //reverses direction
		moreBits = glm::normalize(moreBits);

		lessBits = -lessBits;
		lessBits = glm::normalize(lessBits);

		if (touchingSides.x && touchingSides.z) //topleft touches, push towards bottomright
		{
			//exludes topleft
			if (reverseCAreas.tr.x > reverseCAreas.bl.x  && reverseCAreas.tr.x > reverseCAreas.br.x) //topright largest
			{
				actualCorner = reverseVecs.tr - glm::vec2(-centerDiff.x, centerDiff.y);  //subtracts opposite corner
			}
			else if (reverseCAreas.bl.x > reverseCAreas.br.x) //bottomleft largest
			{
				actualCorner = reverseVecs.bl - glm::vec2(centerDiff.x, -centerDiff.y); //subtracts opposite corner
			}
			else //bottomright largest
			{
				actualCorner = reverseVecs.br - glm::vec2(-centerDiff.x, -centerDiff.y);  //subtracts opposite corner
			}
		}
		else if (touchingSides.x && touchingSides.w) //topright touches, push towards bottomleft
		{
			//exludes topright
			if (reverseCAreas.tl.x > reverseCAreas.bl.x  && reverseCAreas.tl.x > reverseCAreas.br.x) //topleft largest
			{
				actualCorner = reverseVecs.tl - glm::vec2(centerDiff.x, centerDiff.y);  //subtracts opposite corner
			}
			else if (reverseCAreas.bl.x > reverseCAreas.br.x) //bottomleft largest
			{
				actualCorner = reverseVecs.bl - glm::vec2(centerDiff.x, -centerDiff.y); //subtracts opposite corner
			}
			else //bottomright largest
			{
				actualCorner = reverseVecs.br - glm::vec2(-centerDiff.x, -centerDiff.y);  //subtracts opposite corner
			}
		}
		else if (touchingSides.y && touchingSides.z) //bottomleft touches, push towards topright
		{
			//exludes bottomleft
			if (reverseCAreas.br.x > reverseCAreas.tl.x  && reverseCAreas.br.x > reverseCAreas.tr.x) //bottomright largest
			{
				actualCorner = reverseVecs.br - glm::vec2(-centerDiff.x, -centerDiff.y);  //subtracts opposite corner
			}
			else if (reverseCAreas.tl.x > reverseCAreas.tr.x) //topleft largest
			{
				actualCorner = reverseVecs.tl - glm::vec2(centerDiff.x, centerDiff.y);  //subtracts opposite corner
			}
			else //topright largest
			{
				actualCorner = reverseVecs.tr - glm::vec2(-centerDiff.x, centerDiff.y);  //subtracts opposite corner
			}
			//actualCorner.x = glm::max(actualCorner.x, 0.0f);
			//actualCorner.y = glm::max(actualCorner.y, 0.0f);
		}
		else //bottomright touches, push towards topleft
		{
			//exludes bottomright
			if (reverseCAreas.bl.x > reverseCAreas.tl.x  && reverseCAreas.bl.x > reverseCAreas.tr.x) //bottomleft largest
			{
				actualCorner = reverseVecs.bl - glm::vec2(centerDiff.x, -centerDiff.y); //subtracts opposite corner
			}
			else if (reverseCAreas.tl.x > reverseCAreas.tr.x) //topleft largest
			{
				actualCorner = reverseVecs.tl - glm::vec2(centerDiff.x, centerDiff.y);  //subtracts opposite corner
			}
			else //topright largest
			{
				actualCorner = reverseVecs.tr - glm::vec2(-centerDiff.x, centerDiff.y);  //subtracts opposite corner
			}
		}
	}
	else if (sidesTouching == 1)
	{
		if (touchingSides.x) //up, so push down
		{
			moreBits = glm::vec2(0.0f, -1.0f);
		}
		else if (touchingSides.y) //down, so push up
		{
			moreBits = glm::vec2(0.0f, 1.0f);
		}
		else if (touchingSides.z) //left, so push right
		{
			moreBits = glm::vec2(1.0f, 0.0f);
		}
		else if (touchingSides.w) //right, so push left
		{
			moreBits = glm::vec2(-1.0f, 0.0f);
		}
	}
	else if (sidesTouching == 3)
	{
		if (touchingSides.z && touchingSides.w) //vertical move
		{
			if (touchingSides.x) //up, so push down
			{
				moreBits = glm::vec2(0.0f, -1.0f);

				//concave shape, top side
				if (reverseCAreas.bl.x > reverseCAreas.br.x) //top side touches, move is based on bottom two corners
				{
					actualCorner = reverseVecs.bl - glm::vec2(centerDiff.x, centerDiff.y); //subtracts opposite corner
				}
				else
				{
					actualCorner = reverseVecs.br - glm::vec2(-centerDiff.x, centerDiff.y);  //subtracts opposite corner
				}
			}
			else if (touchingSides.y) //down, so push up
			{
				moreBits = glm::vec2(0.0f, 1.0f);

				//concave shape, bottom side
				if (reverseCAreas.tl.x > reverseCAreas.tr.x) //down side touches, move is based on top two corners
				{
					actualCorner = reverseVecs.tl - glm::vec2(centerDiff.x, -centerDiff.y);  //subtracts opposite corner
				}
				else
				{
					actualCorner = reverseVecs.tr - glm::vec2(-centerDiff.x, -centerDiff.y);  //subtracts opposite corner
				}
			}
		}
		else //horizantal move
		{
			if (touchingSides.z) //left, so push right
			{
				moreBits = glm::vec2(1.0f, 0.0f);

				//concave shape, right side
				if (reverseCAreas.tr.x > reverseCAreas.br.x) //left side touches, move is based on right two corners
				{
					actualCorner = reverseVecs.tr - glm::vec2(-centerDiff.x, -centerDiff.y);  //subtracts opposite corner
				}
				else
				{
					actualCorner = reverseVecs.br - glm::vec2(-centerDiff.x, centerDiff.y);  //subtracts opposite corner
				}
			}
			else if (touchingSides.w) //right, so push left
			{
				moreBits = glm::vec2(-1.0f, 0.0f);

				//concave shape, left side
				if (reverseCAreas.tl.x > reverseCAreas.bl.x) //right side touches, move is based on left two corners
				{
					actualCorner = reverseVecs.tl - glm::vec2(centerDiff.x, -centerDiff.y);  //subtracts opposite corner
				}
				else
				{
					actualCorner = reverseVecs.bl - glm::vec2(centerDiff.x, centerDiff.y);  //subtracts opposite corner
				}
			}
		}
		if (reverseCAreas.tl.x > reverseCAreas.tr.x && reverseCAreas.tl.x > reverseCAreas.bl.x  && reverseCAreas.tl.x > reverseCAreas.br.x) //topleft largest
		{
			actualCorner = reverseVecs.tl - glm::vec2(centerDiff.x, centerDiff.y);  //subtracts opposite corner
		}
		else if (reverseCAreas.tr.x > reverseCAreas.bl.x  && reverseCAreas.tr.x > reverseCAreas.br.x) //topright largest
		{
			actualCorner = reverseVecs.tr - glm::vec2(-centerDiff.x, centerDiff.y);  //subtracts opposite corner
		}
		else if (reverseCAreas.bl.x > reverseCAreas.br.x) //bottomleft largest
		{
			actualCorner = reverseVecs.bl - glm::vec2(centerDiff.x, -centerDiff.y); //subtracts opposite corner
		}
		else //bottomright largest
		{
			actualCorner = reverseVecs.br - glm::vec2(-centerDiff.x, -centerDiff.y);  //subtracts opposite corner
		}
	}
	else if (sidesTouching == 0)
	{
		moreBits = glm::vec2(0.0f);
	}

	glm::vec2 magDir = glm::vec2(0.0f); //get concave collision to work
	if (moreBits.x > 0.0f)
	{
		magDir.x = centerDiff.x + positionalVecs.br.x;
	}
	else if (moreBits.x < 0.0f)
	{
		magDir.x = -centerDiff.x + positionalVecs.bl.x;
	}
	if (moreBits.y > 0.0f)
	{
		magDir.y = -centerDiff.y + positionalVecs.tr.y;
	}
	else if (moreBits.y < 0.0f)
	{
		magDir.y = centerDiff.y + positionalVecs.tl.y;
	}
	if (moreBits.x != 0.0f && moreBits.y != 0.0f) //diagonal
	{
		magDir *= moreBits;
		if (abs(magDir.x) < abs(magDir.y))
		{
			magDir = moreBits * magDir.x;
		}
		else
		{
			magDir = moreBits * magDir.y;
		}
	}
	float mag = sqrt(magDir.x * magDir.x + magDir.y * magDir.y);
	
	//for minimal concave based collision movement
	float cMag = sqrt(actualCorner.x * actualCorner.x + actualCorner.y * actualCorner.y);
	if ((sidesTouching == 3 || sidesTouching == 2) && cMag < mag)
	{
		mag = cMag;
		moreBits = glm::normalize(actualCorner);
	}
	/*
	if (mag > centerDiff.x && mag > centerDiff.y)
	{
		mag = 0; //catch errors
	}*/

	return Manifest{moreBits, mag, 0.0f}; //depth with offset
}

CollisionMap::~CollisionMap()
{
	if (this->m_map != nullptr) //deletes pointers to avoid memory leaks
	{
		for (int i = 0; i < this->m_width; i++)
		{
			if (this->m_map[i] != nullptr)
			{
				delete[] this->m_map[i];
				this->m_map[i] = nullptr;
			}
		}
		delete[] this->m_map;
		this->m_map = nullptr;
	}

	if (this->m_corners != nullptr)
	{
		delete this->m_corners;
		this->m_corners = nullptr;
	}
}

/*

end Collision Texture Object

*/