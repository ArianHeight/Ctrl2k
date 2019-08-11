#pragma once

/*
Texture Buffer
*/
class TBuffer
{
public:
	TBuffer(int width = 1280, int height = 720);
	~TBuffer();

public:
	int m_width, m_height;

	GLuint m_tBuffer;

	GLuint m_tAlbedo;
	GLuint m_tDepth;
	GLuint m_tNormal;

	GLuint attachments[2]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 }; //albedo, normal

	void initTBuffer();
};
/*
end Texture Buffer
*/