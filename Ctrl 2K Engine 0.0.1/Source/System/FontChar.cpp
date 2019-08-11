#include <Ctrl 2kPCH.h>

/*

a character in a font

*/

FontChar::FontChar(float x, float y, float width, float height, float xoffset, float yoffset, float advance, float atlasWidth, float atlasHeight, float fontSize) :
	m_dimensions(glm::vec2(width / fontSize, height / fontSize)), m_offset(glm::vec2(xoffset / fontSize, yoffset / fontSize)), m_advance(advance / fontSize), m_size(fontSize)
{
	//cstr
	//this->m_texcoords.tl = glm::vec2(x / atlasWidth, (atlasHeight - y )/ atlasHeight);
	this->m_texcoords.tl = glm::vec2(x / atlasWidth, (atlasHeight - y) / atlasHeight);
	this->m_texcoords.br = glm::vec2((x + width) / atlasWidth, ((atlasHeight - y) - height) / atlasHeight);
	this->m_texcoords.tr.x = this->m_texcoords.br.x;
	this->m_texcoords.tr.y = this->m_texcoords.tl.y;
	this->m_texcoords.bl.x = this->m_texcoords.tl.x;
	this->m_texcoords.bl.y = this->m_texcoords.br.y;
}

FontChar::~FontChar()
{
	//dstr
}

/*

end FontChar

*/