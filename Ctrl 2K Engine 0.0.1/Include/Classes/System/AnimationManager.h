#pragma once

/*

a managing class for animations

*/

class AnimationManager
{
public:
	AnimationManager(TextureManager &source);
	~AnimationManager();

	int m_bufferSize;

private:
	TextureManager &textureSource;
	std::vector<Animation*> m_buffer;

	void updateSize();

public:
	int search(std::string &name);

	void bind(std::string &name);
	void bind(int index);

	Animation* getAnimation(std::string &name);
	Animation* getAnimation(int index);

	AniFrame* getAnimationFrame(std::string &name, double &time, Orientation direction = UP);
	AniFrame* getAnimationFrame(int index, double &time, Orientation direction = UP);

	void disable(std::string &name);
	void disable(int index);

	void activate(std::string &name);
	void activate(int index);

	int create(std::string &name);
	int addAnimation(Animation *newAnimation);

	bool del(std::string &name);
	bool del(int index);

	std::string getAtlasPath(std::string &name);
	std::string getAtlasPath(int index);

	int getAtlas(std::string &name);
	int getAtlas(int index);

	void reserve(int size);
};

/*

end AnimationManager class

*/