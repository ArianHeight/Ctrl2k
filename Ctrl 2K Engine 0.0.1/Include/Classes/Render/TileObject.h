#pragma once

/*
Tile Class
*/

class TileObject
{
public:
	TileObject(TileType format, std::string name, GLuint texture, GLuint depth, GLuint normal, float unitsWidth = 1.0f, float aspectRatio = 1.0f);
	~TileObject();

private:
	void updateAR();
	void createCollisionBitMap();

public:
	TileType m_type;

	GLuint m_texture;
	GLuint m_depthMap;
	GLuint m_normalMap;

	CollisionMap *m_collisionMap = nullptr; //collisionMap object
	
	float m_unitsWidth;
	float m_ar; //aspect ratio, width : height
	std::string m_name;
};

/*
end Tile Class
*/