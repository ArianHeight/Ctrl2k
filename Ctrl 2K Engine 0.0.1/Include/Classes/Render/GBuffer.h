#pragma once

/*

G-Buffer Class

*/
class GBuffer
{
public:
	GBuffer(int *windowWidth, int *windowHeight);
	~GBuffer();

	void init();

	GLuint gBuffer; //96 bit g-buffer object

	GLuint gDepth; //depth 24, stencil 8
	GLuint gAlbedo; //r 8, g 8, b 8, ambient occlusion 8
	GLuint gNormal; //normal x 10, normal y 10, matID 10, normal z general direction 2

	GLuint attachments[2]{GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};

	int *wwPointer, *whPointer;
};
/*

end G-Buffer Class

*/