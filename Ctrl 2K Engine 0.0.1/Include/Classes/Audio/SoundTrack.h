#pragma once

/*

Main Audio Data Structure
Also base class for SFX and BGM

*/

//for friending
class AudioManager;

class SoundTrack
{
public:
	SoundTrack(const std::string &name, const std::string &filePath);
	~SoundTrack();

	friend class AudioManager;

protected:
	std::string m_name; //name of the track
	std::string m_filePath; //file path

	bool m_loadState{ false }; //whether or not the file is loaded
	bool m_isMusic{ false }; //tells whether or not the current track is background music

public:
	virtual bool loadFile();
	virtual bool unloadFile();
	virtual bool trackPlaying();
	virtual bool trackPaused();
	
	virtual bool playTrack(); //standard loops track once
	virtual bool playTrack(int loop); //inputs the amount of loops
	virtual bool playTrack(int channel, int loop); //inputs the target channel and amount of loops
	virtual bool resumePlayingTrack();

	std::string getName();
	std::string getFilePath();
	bool getLoadState();
	bool getIsMusic();
};

/*

end SoundTrack Object

*/