#include <Ctrl 2kPCH.h>

/*

BackGround Music class

*/

BGM::BGM(std::string &name, std::string &filePath) :
	SoundTrack(name, filePath)
{
	//cstr
	this->m_isMusic = true;
}

bool BGM::loadFile()
{
	if (!this->m_loadState)
	{
		this->m_music = Mix_LoadMUS(this->m_filePath.c_str());
		this->m_loadState = (this->m_music != nullptr);
	}

	return this->m_loadState;
}

bool BGM::unloadFile()
{
	if (this->m_loadState)
	{
		Mix_FreeMusic(this->m_music);
		this->m_music = nullptr;
		this->m_loadState = false;
	}

	return (!this->m_loadState);
}

bool BGM::trackPlaying()
{
	return Mix_PlayingMusic();
}

bool BGM::trackPaused()
{
	return Mix_PausedMusic();
}

bool BGM::playTrack()
{
	return this->playTrack(0);
}

bool BGM::playTrack(int loop)
{
	this->loadFile();
	return (Mix_PlayMusic(this->m_music, loop) != -1);
}

bool BGM::playTrack(int channel, int loop)
{
	return this->playTrack(loop);
}

bool BGM::resumePlayingTrack()
{
	Mix_ResumeMusic();
	return true;
}

BGM::~BGM()
{
	//dstr
	if (this->m_loadState) //frees the music memory if loaded
	{
		Mix_FreeMusic(this->m_music);
		this->m_music = nullptr;
		this->m_loadState = false;
	}
}

/*

end BGM class

*/