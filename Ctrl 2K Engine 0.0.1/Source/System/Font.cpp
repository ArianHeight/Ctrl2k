#include <Ctrl 2kPCH.h>

/*

manages Fonts

*/


Font::Font()
{
	//cstr
}

void Font::normalizeValues()
{
	this->m_baseLineN = this->m_baseLine / (float)this->m_pixelSize;
	this->m_lineHeightN = this->m_lineHeight / (float)this->m_pixelSize;
}

void Font::bindAtlas()
{
	glBindTexture(GL_TEXTURE_2D, this->m_atlas);
}

void Font::setAtlas(GLuint id)
{
	this->m_atlas = id;
}

void Font::setAtlasDim(int width, int height)
{
	this->atlasDimensions = glm::ivec2(width, height);
}

void Font::setLineHeight(float height)
{
	this->m_lineHeight = height;
}

void Font::setBase(float base)
{
	this->m_baseLine = base;
}

void Font::setSize(int pixelSize)
{
	this->m_pixelSize = pixelSize;
}

void Font::setFace(std::string &face)
{
	this->m_face = face;
}

GLuint Font::getAtlas()
{
	return this->m_atlas;
}

float Font::getBase()
{
	return this->m_baseLineN;
}

float Font::getLineHeight()
{
	return this->m_lineHeightN;
}

float Font::getSize()
{
	return this->m_pixelSize;
}

Font::~Font()
{
	//dstr
}

/*

end Font Class

*/