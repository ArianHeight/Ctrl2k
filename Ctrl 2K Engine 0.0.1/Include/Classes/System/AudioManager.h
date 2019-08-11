#pragma once

/*

Manager class for Audio data

*/

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	int m_bufferSize; //size of soundTracks

private:
	std::vector<SoundTrack*> m_buffer;

	void updateSize();

public:
	int search(std::string &name);

	void load(std::string &name);
	void load(int index);

	void unload(std::string &name);
	void unload(int index);

	void play(std::string &name);
	void play(int index);
	void play(std::string &name, int loop);
	void play(int index, int loop);
	void play(int channel, std::string &name, int loop);
	void play(int channel, int index, int loop);

	int addMusic(std::string &name, std::string &path);
	int addSFX(std::string &name, std::string &path);

	bool del(std::string &name);
	bool del(int index);

	std::string getPath(std::string &name);
	std::string getPath(int index);

	SoundTrack* data(std::string &name);
	SoundTrack* data(int index);

	bool getIsMusic(std::string &name);
	bool getIsMusic(int index);

	void reserve(int size);
};

/*

end AudioManager

*/