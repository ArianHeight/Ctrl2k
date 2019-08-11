#pragma once

/*

a single animation set(for every direction)

*/

class Animation
{
public:
	Animation();
	~Animation();

	double m_totalAnimationTime;
	std::vector<int> m_bufferSize;
	std::string m_name;
	bool m_repeatable{true}; //if true, playAnimation() will reset timeIn and effectively repeat forever as long as it is still called. Default:true

private:
	std::vector<std::vector<AniFrame>> m_frames;
	std::vector<Orientation> m_orientations;
	int m_atlasTextureIndex = 0;

	void updateBufferSize();

public:
	AniFrame* playAnimation(int targetRow, double &timeIn); //returns time - m_totalAnimationTime if timeIn exceeds total time, else returns timeIn, displayFrame is set to frame that should be drawn
	AniFrame* playAnimation(Orientation direction, double &timeIn);

	void addFrame(int targetRow, glm::ivec2 &topLeft, glm::ivec2 &pixelDimensions, glm::ivec2 &atlasDimensions, int worldSpaceOne, double startTime, double duration);
	void setRows(int rows);

	void setAtlas(int index);
	int getAtlas();

	void setTotalTime(double time);
	double getTotalTime();

	void makeRepeatable();
	void makeUnrepeatable();
	void setRepeatable(bool b);
	bool getRepeatable();

	void setName(std::string &name);
};

/*

end Animation class

*/