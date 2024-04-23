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
	int search(const std::string &name);

	void load(const std::string &name);
	void load(int index);

	void unload(const std::string &name);
	void unload(int index);

	void play(const std::string &name);
	void play(int index);
	void play(const std::string &name, int loop);
	void play(int index, int loop);
	void play(int channel, const std::string &name, int loop);
	void play(int channel, int index, int loop);

	int addMusic(const std::string &name, const std::string &path);
	int addSFX(const std::string &name, const std::string &path);

	bool del(const std::string &name);
	bool del(int index);

	std::string getPath(const std::string &name);
	std::string getPath(int index);

	SoundTrack* data(const std::string &name);
	SoundTrack* data(int index);

	bool getIsMusic(const std::string &name);
	bool getIsMusic(int index);

	void reserve(int size);
};

/*

end AudioManager

*/