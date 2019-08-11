#include <Ctrl 2kPCH.h>

/*
Entity Buffer
*/
ScreenBuffer::ScreenBuffer(int width, int height) : m_width(width), m_height(height)
{
	this->initScreenBuffer();
}

void ScreenBuffer::initScreenBuffer()
{
	glGenFramebuffers(1, &this->m_screenBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, this->m_screenBuffer);

	glGenTextures(1, &this->m_sAlbedo);
	glBindTexture(GL_TEXTURE_2D, this->m_sAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->m_width, this->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_sAlbedo, 0);

	glGenTextures(1, &this->m_sDepth);
	glBindTexture(GL_TEXTURE_2D, this->m_sDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, this->m_width, this->m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->m_sDepth, 0);

	glDrawBuffers(1, attachments);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Frame Buffer Error! Status : 0x" << status << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

ScreenBuffer::~ScreenBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteTextures(1, &this->m_sAlbedo);
	glDeleteTextures(1, &this->m_sDepth);
	glDeleteFramebuffers(1, &this->m_screenBuffer);
}
/*
end Entity Buffer
*/