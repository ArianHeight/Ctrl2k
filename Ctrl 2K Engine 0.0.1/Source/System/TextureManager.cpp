#include <Ctrl 2kPCH.h>

/*

Texture Manager class

*/

TextureManager::TextureManager()
{
	//cstr
}

//internal method used by manager class
void TextureManager::updateSize()
{
	this->m_bufferSize = this->m_buffer.size();
}

int TextureManager::search(std::string &name)
{
	for (int count = 0; count < this->m_bufferSize; count++)
	{
		if (this->m_buffer[count]->m_name == name)
		{
			return count;
		}
	}

	return -1;
}

void TextureManager::bind(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		this->m_buffer[index]->bind();
		return;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureManager::bind(std::string &name)
{
	this->bind(this->search(name));
}

void TextureManager::disable(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		this->m_buffer[index]->disable();
	}
}

void TextureManager::disable(std::string &name)
{
	this->disable(this->search(name));
}

void TextureManager::activate(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		this->m_buffer[index]->activate();
	}
}

void TextureManager::activate(std::string &name)
{
	this->activate(this->search(name));
}

int TextureManager::create(std::string &name, std::string &path)
{
	int index = this->search(name);

	if (index != -1) //texture exists or name overlap
	{
		return index;
	}

	this->m_buffer.push_back(new Texture(name, path));
	this->updateSize();

	index = this->search(name);
	return index;
}

bool TextureManager::del(int index)
{
	if (index < 0 || index >= this->m_bufferSize) //out of range
	{
		return false;
	}

	delete this->m_buffer[index];
	this->m_buffer.erase(this->m_buffer.begin() + index);
	this->updateSize();

	return true;
}

bool TextureManager::del(std::string &name)
{
	return this->del(this->search(name));
}

std::string TextureManager::getPath(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		return this->m_buffer[index]->m_filePath;
	}
}

std::string TextureManager::getPath(std::string &name)
{
	return this->getPath(this->search(name));
}

//returns a pointer to the Texture object
Texture* TextureManager::data(int index)
{
	if (index >= 0 && index < this->m_bufferSize)
	{
		return this->m_buffer[index];
	}
	return this->m_buffer[0];
}

//returns a pointer to the Texture object
Texture* TextureManager::data(std::string &name)
{
	return this->data(this->search(name));
}

void TextureManager::reserve(int size)
{
	if (size > this->m_bufferSize)
	{
		this->m_buffer.reserve(size);
	}
}

TextureManager::~TextureManager()
{
	for (Texture *i : this->m_buffer)
	{
		i->disable(); //disable all textures in buffer
	}
	this->m_buffer.clear();
}

/*

end Texture Manager class

*/