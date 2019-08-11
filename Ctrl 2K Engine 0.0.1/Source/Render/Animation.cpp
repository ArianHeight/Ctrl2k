#include <Ctrl 2kPCH.h>

/*

a single animation set(for every direction)

*/

Animation::Animation()
{
	//cstr
}

//internal method for class use only. Updates the animation buffer sizes
void Animation::updateBufferSize()
{
	if (this->m_bufferSize.size() != this->m_frames.size()) //updates the buffer holding the sizes
	{
		this->m_bufferSize.resize(this->m_frames.size());
	}

	for (int count = 0; count < this->m_frames.size(); count++) //updates the sizes
	{
		this->m_bufferSize[count] = this->m_frames[count].size();
	}
}

//returns the current frame of animation
AniFrame* Animation::playAnimation(int targetRow, double &timeIn)
{
	AniFrame *displayFrame = nullptr;
	if (targetRow < 0 || targetRow >= this->m_bufferSize.size()) //non-valid direction
	{
		switch (this->m_bufferSize.size())
		{
		case 2: //two directions only assumed up and down
			if (targetRow > 1 && targetRow <= 5) //left right up left up right
			{
				return this->playAnimation(0, timeIn); //uses up
			}
			else if (targetRow == 5 || targetRow == 7) //lower right lower left
			{
				return this->playAnimation(1, timeIn); //uses down
			}
			//break;
		case 4: //four directions, upper left and upper right uses up, lower right and lower left uses down
			if (targetRow == 4 || targetRow == 5) //upper left upper right
			{
				return this->playAnimation(0, timeIn); //uses up animation
			}
			else if (targetRow == 6 || targetRow == 7) //lower left lower right
			{
				return this->playAnimation(1, timeIn); //uses down animation
			}
			//break;
		}

		displayFrame = &this->m_frames[0][0]; //early return with frame 0
		return displayFrame;
	}

	//update for infinite animations
	if (this->m_repeatable == false)
	{
		if (timeIn > this->m_totalAnimationTime)
		{
			timeIn = this->m_totalAnimationTime; //this is a limit on how many frames a spritesheet can have
		}
	}
	else
	{
		//updates timeIn to the current position in range of the animation length
		while (timeIn >= this->m_totalAnimationTime)
		{
			timeIn -= this->m_totalAnimationTime;
		}
		while (timeIn < 0)
		{
			timeIn += this->m_totalAnimationTime;
		}
	}

	for (int count = 0; count < this->m_bufferSize[targetRow]; count++) //finds the right frame in accordance to timeIn
	{
		if (this->m_frames[targetRow][count].m_startTime <= timeIn && this->m_frames[targetRow][count].m_endTime >= timeIn)
		{
			displayFrame = &this->m_frames[targetRow][count];
			break;
		}
	}

	return displayFrame; //return
}

//wrapper method which uses an Orientation input instead of targetRow
AniFrame* Animation::playAnimation(Orientation direction, double &timeIn)
{
	int index = int(direction);
	return this->playAnimation(index, timeIn); //finds target row
}

//adds a single frame to the animation
void Animation::addFrame(int targetRow, glm::ivec2 &topLeft, glm::ivec2 &pixelDimensions, glm::ivec2 &atlasDimensions, int worldSpaceOne, double startTime, double duration)
{
	if (targetRow < 0 || targetRow >= this->m_bufferSize.size()) //non-valid targetRow
	{
		return;
	}

	this->m_frames[targetRow].push_back(AniFrame(topLeft, pixelDimensions, atlasDimensions, worldSpaceOne, startTime, duration));
	this->updateBufferSize();
}

//sets the amount of rows/orientations for the animation
void Animation::setRows(int rows)
{
	if (rows != this->m_bufferSize.size()) //resize not needed if size is equal
	{
		this->m_frames.resize(rows);
		this->m_orientations.resize(rows);
		this->updateBufferSize();
		
		for (int i = 0; i < this->m_frames.size() && i < int(LOWER_RIGHT) + 1; i++) //range check for Orientation -> int
		{
			this->m_orientations[i] = Orientation(i);
		}
	}
}

void Animation::setAtlas(int index)
{
	if (index >= 0)
	{
		this->m_atlasTextureIndex = index;
	}
}

int Animation::getAtlas()
{
	return this->m_atlasTextureIndex;
}

void Animation::setTotalTime(double time)
{
	this->m_totalAnimationTime = time;
}

double Animation::getTotalTime()
{
	return this->m_totalAnimationTime;
}

void Animation::makeRepeatable()
{
	this->m_repeatable = true;
}

void Animation::makeUnrepeatable()
{
	this->m_repeatable = false;
}

void Animation::setRepeatable(bool b)
{
	this->m_repeatable = b;
}

bool Animation::getRepeatable()
{
	return this->m_repeatable;
}

void Animation::setName(std::string &name)
{
	this->m_name = name;
}

Animation::~Animation()
{
	//dstr
	for (int i = 0; i < this->m_bufferSize.size(); i++)
	{
		this->m_frames[i].clear();
	}
	this->m_frames.clear();
	this->m_bufferSize.clear();
}

/*

end Animation class

*/