#pragma once

/*

Texture object

*/

//friends
class TextureManager;

class Texture
{
public:
	Texture(std::string name, std::string path);
	~Texture();

	friend class TextureManager;

private:
	std::string m_name;
	std::string m_filePath;
	ActiveState m_state {DISABLED}; //created as disabled
	GLuint m_texture{0};

public:
	//methods
	GLuint activate(bool mips = false);
	void disable();
	void bind();

	ActiveState getState();
};

/*

end Texture object

*/