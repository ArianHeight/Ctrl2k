#pragma once

/*

Texture Manager class

*/

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	int m_bufferSize{0};

private:
	std::vector<Texture*> m_buffer;

	void updateSize();

public:
	int search(std::string &name);

	void bind(std::string &name);
	void bind(int index);

	void disable(std::string &name);
	void disable(int index);

	void activate(std::string &name);
	void activate(int index);

	int create(std::string &name, std::string &path);

	bool del(std::string &name);
	bool del(int index);

	std::string getPath(std::string &name);
	std::string getPath(int index);

	Texture* data(std::string &name);
	Texture* data(int index);

	void reserve(int size);
};

/*

end Texture Manager class

*/