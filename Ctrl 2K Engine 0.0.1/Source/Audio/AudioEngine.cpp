#include <Ctrl 2kPCH.h>

/*

definitions for AudioEngine class

*/

AudioEngine::AudioEngine(CoreEngine &engine) :
	parentEngine(engine), m_soundtracks(engine.getAssets().getAuM())
{
	//cstr
}

void AudioEngine::update()
{
	this->m_channelNum = Mix_AllocateChannels(-1); //-1 tells how many channels are allocated
	
	if (this->m_channelNum != this->m_channels.size()) //need size adjusting
	{
		while (this->m_channelNum - this->m_channels.size() > 0) //more channels
		{
			this->m_channels.push_back(" ");
		}
		while (this->m_channelNum - this->m_channels.size() < 0) //less channels
		{
			this->m_channels.erase(this->m_channels.begin() + this->m_channels.size() - 1);
		}
	}

	for (int i = 0; i < this->m_channelNum; i++)
	{
		if (Mix_Playing(i) == 0) //if there is nothing playing on channel, update the status in m_channels
		{
			this->m_channels[i] = " ";
		}
	}

	//for music channel
	if (!Mix_PlayingMusic())
	{
		this->m_musicChannel = " ";
	}
}

void AudioEngine::setChannels(int num)
{
	if (num <= 128) //max is 128 allowed
	{
		Mix_AllocateChannels(num);
	}
	this->update(); //change the status of m_channels
}

void AudioEngine::playSoundTrack(std::string &name)
{
	this->playSoundTrack(-1, name, 0);
}

void AudioEngine::playSoundTrack(int index)
{
	this->playSoundTrack(-1, index, 0);
}

void AudioEngine::playSoundTrack(std::string &name, int loop)
{
	this->playSoundTrack(-1, name, loop);
}

void AudioEngine::playSoundTrack(int index, int loop)
{
	this->playSoundTrack(-1, index, loop);
}

void AudioEngine::playSoundTrack(int channel, std::string &name, int loop)
{
	if (this->m_soundtracks.getIsMusic(name)) //runs if track is music
	{
		this->m_soundtracks.play(channel, name, loop);
		this->m_musicChannel = name;
		return;
	}

	if (channel < 0) //negative number will go to next availible channel
	{
		for (int i = 0; i < this->m_channelNum; i++)
		{
			if (this->m_channels[i] == " ")
			{
				this->m_soundtracks.play(channel, name, loop);
				this->m_channels[i] = name;
				return;
			}
		}
		return;
	}

	if (this->m_channels[channel] == " ") //channel is availible and can be played on
	{
		this->m_soundtracks.play(channel, name, loop);
		this->m_channels[channel] = name;
		return;
	}
}

void AudioEngine::playSoundTrack(int channel, int index, int loop)
{
	if (this->m_soundtracks.getIsMusic(index)) //runs if track is music
	{
		this->m_soundtracks.play(channel, index, loop);
		this->m_musicChannel = "Random_Song";
		return;
	}

	std::string temp = "track_" + index;
	if (channel < 0) //negative number will go to next availible channel
	{
		for (int i = 0; i < this->m_channelNum; i++)
		{
			if (this->m_channels[i] == " ")
			{
				this->m_soundtracks.play(channel, index, loop);
				this->m_channels[i] = temp;
				return;
			}
		}
		return;
	}

	if (this->m_channels[channel] == " ")
	{
		this->m_soundtracks.play(channel, index, loop);
		this->m_channels[index] = temp;
	}
}

AudioEngine::~AudioEngine()
{
	//dstr
}

/*

end AudioEngine class

*/