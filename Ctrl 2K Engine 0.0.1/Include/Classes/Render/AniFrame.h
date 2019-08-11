#pragma once

/*

a single frame of animation

*/

class AniFrame
{
public:
	AniFrame(glm::ivec2 topLeft, glm::ivec2 pixelDimensions, glm::ivec2 atlasDimensions,int worldSpaceOne, double startTime, double duration); //height in pixels that would be considered 1.0f in world space
	~AniFrame();

	double m_duration;
	double m_startTime;
	double m_endTime;

	glm::vec2 m_topLeftTexcoord;
	glm::vec2 m_bottomRightTexcoord;

	glm::vec2 m_worldSize;
};

/*

end AniFrame class

*/