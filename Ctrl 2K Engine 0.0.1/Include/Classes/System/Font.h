#pragma once

/*

manages Fonts

*/

//friends
class FontManager;

class Font
{
public:
	Font();
	~Font();

	friend class FontManager;

private:
	GLuint m_atlas = 0; //font texture atlas
	float m_lineHeight;
	float m_baseLine;
	glm::ivec2 atlasDimensions;

	float m_lineHeightN;
	float m_baseLineN;

	std::string m_face;
	int m_pixelSize;

public:
	std::map<char, FontChar> m_characters;

	void normalizeValues();
	void bindAtlas();
	void setAtlas(GLuint id);
	void setAtlasDim(int width, int height);
	void setLineHeight(float height);
	void setBase(float base);
	void setSize(int pixelSize);
	void setFace(std::string &face);
	GLuint getAtlas();
	float getBase();
	float getLineHeight();
	float getSize();
};

/*

end Font Class

*/