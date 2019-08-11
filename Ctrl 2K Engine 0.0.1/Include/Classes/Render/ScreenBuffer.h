#pragma once

/*
Entity Buffer
*/
class ScreenBuffer
{
public:
	ScreenBuffer(int width = 1280, int height = 720);
	~ScreenBuffer();

private:
	int m_width, m_height;

	GLuint m_screenBuffer;

	GLuint m_sAlbedo;
	GLuint m_sDepth;

	GLuint attachments[1]{ GL_COLOR_ATTACHMENT0 }; //albedo, depth

public:
	void initScreenBuffer();
};
/*
end Entity Buffer
*/