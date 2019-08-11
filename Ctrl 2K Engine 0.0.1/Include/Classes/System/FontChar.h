#pragma once

/*

a character in a font

*/

class FontChar
{
public:
	FontChar(float x, float y, float width, float height, float xoffset, float yoffset, float advance, float atlasWidth, float atlasHeight, float fontSize);
	~FontChar();

	glm::vec2 m_dimensions;
	glm::vec2 m_offset;
	FourPoints m_texcoords;
	float m_advance;
	float m_size;
};

/*

end FontChar

*/