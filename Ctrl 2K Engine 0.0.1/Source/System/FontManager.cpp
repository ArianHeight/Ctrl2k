#include <Ctrl 2kPCH.h>

/*

class for managing fonts

*/

FontManager::FontManager()
{
	//cstr
}

void FontManager::updateSize()
{
	this->m_bufferSize = this->m_buffer.size();
}

int FontManager::search(std::string &name)
{
	for (int count = 0; count < this->m_bufferSize; count++)
	{
		if (this->m_buffer[count]->m_face == name)
		{
			return count;
		}
	}

	return -1;
}

int FontManager::addFont(Font *newFont)
{
	this->m_buffer.push_back(newFont);
	this->updateSize();

	return this->search(newFont->m_face);
}

int FontManager::createFont()
{
	this->m_buffer.push_back(new Font());
	this->updateSize();

	return this->m_bufferSize - 1;
}

bool FontManager::del(int index)
{
	if (index < 0 || index >= this->m_bufferSize) //out of range
	{
		return false;
	}

	delete this->m_buffer[index];
	this->m_buffer[index] = nullptr;
	this->m_buffer.erase(this->m_buffer.begin() + index);
	this->updateSize();

	return true;
}

bool FontManager::del(std::string &name)
{
	return this->del(this->search(name));
}

std::string FontManager::getName(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		return this->m_buffer[index]->m_face;
	}

	return std::string("");
}

std::string FontManager::getName(std::string &name)
{
	return this->getName(this->search(name));
}

Font* FontManager::data(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		return this->m_buffer[index];
	}

	return this->m_buffer[0];
}

Font* FontManager::data(std::string &name)
{
	return this->data(this->search(name));
}

void FontManager::reserve(int size)
{
	this->m_buffer.reserve(size);
}

FontManager::~FontManager()
{
	//dstr
	for (int i = 0; i < this->m_buffer.size(); i++)
	{
		delete this->m_buffer[i];
		this->m_buffer[i] = nullptr;
	}
	this->m_buffer.clear();
}

/*

end FontManager class

*/