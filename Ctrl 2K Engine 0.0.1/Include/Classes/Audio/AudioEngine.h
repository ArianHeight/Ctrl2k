#pragma once

/*

The Engine that is responsible for running all sounds in game

*/

//for linking
class CoreEngine;

class AudioEngine
{
public:
	AudioEngine(CoreEngine& engine); //CANNOT ACCEPT SOUND FILES WITH THE NAME " "!!!!
	~AudioEngine();

private:
	CoreEngine &parentEngine;
	AudioManager &m_soundtracks;

	int m_channelNum; //number of channels
	std::vector<std::string> m_channels; //" " for empty
	std::string m_musicChannel{ " " }; //" " for empty

public:
	void update(); //keeps all the channel data up to date
	void setChannels(int num); //sets the amount of mixing channels

	//music will assume override!!
	void playSoundTrack(const std::string &name);
	void playSoundTrack(int index);
	void playSoundTrack(const std::string &name, int loop);
	void playSoundTrack(int index, int loop);
	void playSoundTrack(int channel, const std::string &name, int loop);
	void playSoundTrack(int channel, int index, int loop);
};

/*

end AudioEngine

*/