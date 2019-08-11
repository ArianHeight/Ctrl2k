#include <Ctrl 2kPCH.h>

/*

a managing class for animations

*/

AnimationManager::AnimationManager(TextureManager &source) : 
	textureSource(source)
{
	//cstr
}

//internal method used by the manager class
void AnimationManager::updateSize()
{
	this->m_bufferSize = this->m_buffer.size();
}

int AnimationManager::search(std::string &name)
{
	for (int count = 0; count < this->m_bufferSize; count++)
	{
		if (this->m_buffer[count]->m_name == name)
		{
			return count;
		}
	}

	return -1;
}

void AnimationManager::bind(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		this->textureSource.bind(this->m_buffer[index]->getAtlas());
		return;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void AnimationManager::bind(std::string &name)
{
	this->bind(this->search(name));
}

Animation* AnimationManager::getAnimation(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		return this->m_buffer[index];
	}
	return this->m_buffer[0];
}

Animation* AnimationManager::getAnimation(std::string &name)
{
	return this->getAnimation(this->search(name));
}

//returns the current frame of animation by index and time elapsed
AniFrame* AnimationManager::getAnimationFrame(int index, double &time, Orientation direction)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		return this->m_buffer[index]->playAnimation(direction, time);
	}
	return this->m_buffer[0]->playAnimation(direction, time);
}

//returns the current frame of animation by name and time elapsed
AniFrame* AnimationManager::getAnimationFrame(std::string &name, double &time, Orientation direction)
{
	return this->getAnimationFrame(this->search(name), time, direction);
}

void AnimationManager::disable(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		this->textureSource.disable(this->m_buffer[index]->getAtlas());
	}
}

void AnimationManager::disable(std::string &name)
{
	this->disable(this->search(name));
}

void AnimationManager::activate(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		this->textureSource.activate(this->m_buffer[index]->getAtlas());
	}
}

void AnimationManager::activate(std::string &name)
{
	this->activate(this->search(name));
}

int AnimationManager::create(std::string &name)
{
	int index = this->search(name);

	if (index != -1) //animation exists or name overlap
	{
		return index;
	}
	
	index = this->m_bufferSize;

	this->m_buffer.push_back(new Animation);
	this->updateSize();
	this->m_buffer[index]->setName(name);

	return index;
}

int AnimationManager::addAnimation(Animation *newAnimation)
{
	this->m_buffer.push_back(newAnimation);
	this->updateSize();

	return this->m_bufferSize - 1;
}

bool AnimationManager::del(int index)
{
	if (index < 0 || index >= this->m_bufferSize) //out of range
	{
		return false;
	}

	delete this->m_buffer[index];
	this->m_buffer.erase(this->m_buffer.begin() + index);
	this->updateSize();

	return true;
}

bool AnimationManager::del(std::string &name)
{
	return this->del(this->search(name));
}

std::string AnimationManager::getAtlasPath(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		return this->textureSource.getPath(this->m_buffer[index]->getAtlas());
	}

	return std::string("");
}

std::string AnimationManager::getAtlasPath(std::string &name)
{
	return this->getAtlasPath(this->search(name));
}

int AnimationManager::getAtlas(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		return this->m_buffer[index]->getAtlas();
	}

	return 0;
}

int AnimationManager::getAtlas(std::string &name)
{
	return this->getAtlas(this->search(name));
}

void AnimationManager::reserve(int size)
{
	if (size > this->m_bufferSize)
	{
		this->m_buffer.reserve(size);
	}
}


AnimationManager::~AnimationManager()
{
	//dstr
	for (int i = 0; i < this->m_bufferSize; i++)
	{
		if (this->m_buffer[i] != nullptr)
		{
			delete this->m_buffer[i];
			this->m_buffer[i] = nullptr;
		}
	}
	this->m_buffer.clear();
	this->m_bufferSize = 0;
}

/*

end AnimationManager class

*/