#include <Ctrl 2kPCH.h>

/*

a single frame of animation

*/

AniFrame::AniFrame(glm::ivec2 topLeft, glm::ivec2 pixelDimensions, glm::ivec2 atlasDimensions, int worldSpaceOne, double startTime, double duration) :
	m_startTime(startTime), m_duration(duration), m_endTime(startTime + duration)
{
	//cstr
	this->m_topLeftTexcoord = glm::vec2(topLeft) / glm::vec2(atlasDimensions);
	this->m_bottomRightTexcoord = glm::vec2(topLeft.x + pixelDimensions.x, topLeft.y - pixelDimensions.y) / glm::vec2(atlasDimensions);

	this->m_worldSize = glm::vec2(pixelDimensions) / (float)worldSpaceOne;
}

AniFrame::~AniFrame()
{
	//dstr
}

/*

end AniFrame class

*/