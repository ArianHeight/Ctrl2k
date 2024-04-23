#pragma once

/*

BackGround Music class
sub-class of SoundTrack
used to hold music data

*/

//for friending
class AudioManager;

class BGM : public SoundTrack
{
public:
	BGM(const std::string &name, const std::string &filePath);
	~BGM();

	friend class AudioManager;

private:
	Mix_Music *m_music;

public:
	bool loadFile();
	bool unloadFile();
	bool trackPlaying();
	bool trackPaused();

	bool playTrack();
	bool playTrack(int loop);
	bool playTrack(int channel, int loop); //will ignore channel
	bool resumePlayingTrack();
};

/*

end BGM class

*/