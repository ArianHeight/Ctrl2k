#include <Ctrl 2kPCH.h>

/*

for AudioManager

*/

AudioManager::AudioManager()
{
	//cstr
}

//internal method used by manager class
void AudioManager::updateSize()
{
	this->m_bufferSize = this->m_buffer.size();
}

int AudioManager::search(std::string &name)
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

void AudioManager::load(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		this->m_buffer[index]->loadFile();
		return;
	}
}

void AudioManager::load(std::string &name)
{
	this->load(this->search(name));
}

void AudioManager::unload(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		this->m_buffer[index]->unloadFile();
	}
}

void AudioManager::unload(std::string &name)
{
	this->unload(this->search(name));
}

void AudioManager::play(int index)
{
	this->play(-1, index, 0);
}

void AudioManager::play(std::string &name)
{
	this->play(this->search(name));
}
void AudioManager::play(std::string &name, int loop)
{
	this->play(-1, this->search(name), loop);
}

void AudioManager::play(int index, int loop)
{
	this->play(-1, index, loop);
}

void AudioManager::play(int channel, std::string &name, int loop)
{
	this->play(channel, this->search(name), loop);
}

void AudioManager::play(int channel, int index, int loop)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		this->m_buffer[index]->playTrack(channel, loop);
	}
}

int AudioManager::addMusic(std::string &name, std::string &path)
{
	int index = this->search(name);

	if (index != -1) //texture exists or name overlap
	{
		return index;
	}

	this->m_buffer.push_back(new BGM(name, path));
	this->updateSize();

	index = this->search(name);
	return index;
}

int AudioManager::addSFX(std::string &name, std::string &path)
{
	int index = this->search(name);

	if (index != -1) //texture exists or name overlap
	{
		return index;
	}

	this->m_buffer.push_back(new SFX(name, path));
	this->updateSize();

	index = this->search(name);
	return index;
}

bool AudioManager::del(int index)
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

bool AudioManager::del(std::string &name)
{
	return this->del(this->search(name));
}

std::string AudioManager::getPath(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		return this->m_buffer[index]->m_filePath;
	}
}

std::string AudioManager::getPath(std::string &name)
{
	return this->getPath(this->search(name));
}

//returns a pointer to the Audio object
SoundTrack* AudioManager::data(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		return this->m_buffer[index];
	}
	return this->m_buffer[0];
}

//returns a pointer to the Audio object
SoundTrack* AudioManager::data(std::string &name)
{
	return this->data(this->search(name));
}

bool AudioManager::getIsMusic(std::string &name)
{
	return this->getIsMusic(this->search(name));
}

bool AudioManager::getIsMusic(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		return this->m_buffer[index]->getIsMusic();
	}
	return this->m_buffer[0]->getIsMusic();
}

void AudioManager::reserve(int size)
{
	if (size > this->m_bufferSize)
	{
		this->m_buffer.reserve(size);
	}
}

AudioManager::~AudioManager()
{
	for (SoundTrack *track : this->m_buffer)
	{
		track->unloadFile();
	}
	this->m_buffer.clear();
}

/*

end AudioManager

*/