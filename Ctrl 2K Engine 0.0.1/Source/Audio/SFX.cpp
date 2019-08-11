#include <Ctrl 2kPCH.h>

/*

Sound Effect class

*/

SFX::SFX(std::string &name, std::string &filePath) :
	SoundTrack(name, filePath)
{
	//cstr
}

bool SFX::loadFile()
{
	if (!this->m_loadState)
	{
		this->m_chunk = Mix_LoadWAV(this->m_filePath.c_str());
		this->m_loadState = (this->m_chunk != nullptr);
	}

	return this->m_loadState;
}

bool SFX::unloadFile()
{
	if (this->m_loadState)
	{
		Mix_FreeChunk(this->m_chunk);
		this->m_chunk = nullptr;
		this->m_loadState = false;
	}

	return (!this->m_loadState);
}

bool SFX::trackPlaying()
{
	return true;
}

bool SFX::trackPaused()
{
	return true;
}

bool SFX::playTrack()
{
	return this->playTrack(-1, 0);
}

bool SFX::playTrack(int loop)
{
	return this->playTrack(-1, loop);
}

bool SFX::playTrack(int channel, int loop)
{
	this->loadFile();
	return (Mix_PlayChannel(channel, this->m_chunk, loop) != -1);
}

bool SFX::resumePlayingTrack()
{
	return true;
}

SFX::~SFX()
{
	//dstr
	if (this->m_loadState) //frees the mix chunk memory if loaded
	{
		Mix_FreeChunk(this->m_chunk);
		this->m_chunk = nullptr;
		this->m_loadState = false;
	}
}

/*

end Sound Effect

*/