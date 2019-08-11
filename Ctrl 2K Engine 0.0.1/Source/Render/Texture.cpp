#include <Ctrl 2kPCH.h>

/*

Texture object

*/

Texture::Texture(std::string name, std::string path) :
	m_name(name), m_filePath(path)
{
	//cstr
}

GLuint Texture::activate(bool mips)
{
	if (this->m_state == DISABLED)
	{
		if (mips)
		{
			this->m_texture = SOIL_load_OGL_texture(this->m_filePath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y); //loads texture into OGL(openGL)
		}
		else
		{
			this->m_texture = SOIL_load_OGL_texture(this->m_filePath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y); //loads texture into OGL(openGL)
		}

		//sets texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);

		this->m_state = ACTIVE;
	}

	return this->m_texture;
}

void Texture::disable()
{
	if (this->m_state == ACTIVE)
	{
		glDeleteTextures(1, &this->m_texture);
		this->m_state = DISABLED;
	}
}

void Texture::bind()
{
	if (this->m_state == DISABLED)
	{
		this->activate();
	}

	glBindTexture(GL_TEXTURE_2D, this->m_texture);
}

ActiveState Texture::getState()
{
	return this->m_state;
}

Texture::~Texture()
{
	//dstr
	this->disable();
}

/*

end Texture object

*/