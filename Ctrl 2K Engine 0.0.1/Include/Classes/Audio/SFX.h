#pragma once

/*

Sound Effect Class
used for holding sfx data
sub-class of SoundTrack

*/

//for friending
class AudioManager;

class SFX : public SoundTrack
{
public:
	SFX(const std::string &name, const std::string &filePath);
	~SFX();

	friend class AudioManager;

private:
	Mix_Chunk *m_chunk;

public:
	bool loadFile();
	bool unloadFile();
	bool trackPlaying(); //currently does not do anything
	bool trackPaused(); //currently does not do anything

	bool playTrack(); //plays on next availible channel for 1 loop on default
	bool playTrack(int loop); //plays on next availible channel for input loops on default
	bool playTrack(int channel, int loop);
	bool resumePlayingTrack(); //currently does not do anything
};

/*

end SFX

*/