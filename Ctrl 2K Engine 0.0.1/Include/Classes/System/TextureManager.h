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
	int search(const std::string &name);

	void bind(const std::string &name);
	void bind(int index);

	void disable(const std::string &name);
	void disable(int index);

	void activate(const std::string &name);
	void activate(int index);

	int create(const std::string &name, const std::string &path);

	bool del(const std::string &name);
	bool del(int index);

	std::string getPath(const std::string &name);
	std::string getPath(int index);

	Texture* data(const std::string &name);
	Texture* data(int index);

	void reserve(int size);
};

/*

end Texture Manager class

*/