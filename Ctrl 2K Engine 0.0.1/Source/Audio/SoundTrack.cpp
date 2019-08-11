#include <Ctrl 2kPCH.h>

/*

for SoundTrack class

*/

SoundTrack::SoundTrack(std::string &name, std::string &filePath) :
	m_name(name), m_filePath(filePath)
{
	//cstr
}

bool SoundTrack::loadFile()
{
	return false;
}

bool SoundTrack::unloadFile()
{
	return false;
}

bool SoundTrack::trackPlaying()
{
	return false;
}

bool SoundTrack::trackPaused()
{
	return false;
}

bool SoundTrack::playTrack()
{
	return false;
}

bool SoundTrack::playTrack(int loop)
{
	return false;
}

bool SoundTrack::playTrack(int channel, int loop)
{
	return false;
}

bool SoundTrack::resumePlayingTrack()
{
	return false;
}

//accessors
std::string& SoundTrack::getName()
{
	return this->m_name;
}

std::string& SoundTrack::getFilePath()
{
	return this->m_filePath;
}

bool SoundTrack::getLoadState()
{
	return this->m_loadState;
}

bool SoundTrack::getIsMusic()
{
	return this->m_isMusic;
}

SoundTrack::~SoundTrack()
{
	//dstr
}

/*

end SoundTrack

*/