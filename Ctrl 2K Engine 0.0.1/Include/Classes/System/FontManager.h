#pragma once

/*

class for managing fonts

*/

class FontManager
{
public:
	FontManager();
	~FontManager();

	int m_bufferSize{0};

private:
	std::vector<Font*> m_buffer;

	void updateSize();

public:
	int search(std::string &name);

	int addFont(Font *newFont);

	int createFont();

	bool del(std::string &name);
	bool del(int index);

	std::string getName(std::string &name);
	std::string getName(int index);

	Font* data(std::string &name);
	Font* data(int index);

	void reserve(int size);
};

/*

end FontManager class

*/