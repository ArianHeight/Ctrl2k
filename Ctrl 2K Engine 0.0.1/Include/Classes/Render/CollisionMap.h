#pragma once

/*

Collision Texture Object

*/

class Component; //pre-declaration

class CollisionMap
{
public:
	CollisionMap(GLuint map, float *mapBigness, glm::vec2 worldPos = glm::vec2(0.0f), float size = 1.0f);
	CollisionMap(const std::string &mapPath, float *mapBigness, glm::vec2 worldPos = glm::vec2(0.0f), float size = 1.0f);
	~CollisionMap();

private:
	float windowAR{16.0f / 9.0f}; //assumed
	float *mapSize;

	GLuint m_texture;
	int m_width{0};
	int m_height{0};
	FourPoints *m_corners = new FourPoints; //topleft, topright, bottomleft, bottomright all in world space
	glm::vec2 m_worldToPixel; //aspect ratio pixel length:world pos length
	glm::vec2 m_worldPos; //map origin's world position
	glm::vec2 m_tlWorldPos; //map topleft point's world pos
	float m_size; //size in world pos

	float **m_map = nullptr; //2D array for depth value

public:
	void updateWindowAR(float ar);

	glm::vec2 toPixelSpace(glm::vec2 worldPos); //change world space pos to pixel space pos
	glm::vec2 toWorldSpace(glm::vec2 pixelPos); //change pixel space pos to world space pos
	glm::vec2 toWorldSize(glm::vec2 pixelPos); //change pixel space value to world space size
	float getPixel(glm::ivec2 pos); //get pixel value at pos(normalized space)
	glm::ivec2 getNearestPixelPos(glm::vec2 pos); //get pixel position of nearest pixel to pos(normalized space)
	glm::ivec2 getNearestPixelPosPixelSpace(glm::vec2 pos); //get pixel value of nearest pixel to pos(pixel space)
	float getNearestPixel(glm::vec2 pos); //get pixel value of nearest pixel to pos(normalized space)
	float getLinearPixel(glm::vec2 pos); //get lerped pixel value of pos(normalized space)
	glm::vec2 getDimensions();

	bool mapCollisionCheck(Component *objTransform, Component *hitbox); //depth check against hitbox to this map. args(Transformation*, PhysComponent*)
	Manifest mapCollisionResponse(Component *objTransform, Component *hitbox); //collision response against hitbox to this map
};

/*

end Collision Texture Object

*/