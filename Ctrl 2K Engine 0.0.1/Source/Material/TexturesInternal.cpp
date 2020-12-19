#include <queue>
#include <vector>
#include <unordered_map>
#include <mutex>

#define GLEW_STATIC
#include "../Include/GL/glew.h"
#ifdef _WIN32
#include "../Include/GL/wglew.h"
#endif

#include "SOIL.h"

#include "Logger.h"
#include "TexturesInternal.h"

static enum OGLTexState
{
	LOADING_QUEUED,		//When the texture is loading in the background
	LOADED,				//Texture has been loaded
	DELETE_QUEUED,		//Texture is queued for delete
	DELETED,			//Texture has been deleted
	TEXSTATE_SIZE				//TEXSTATE_SIZE must remain the final member here
};

static struct ImageMetaData
{
	TexId id;
	int width;
	int height;
	int channels;
};

static struct OGLTexture
{
	OGLTexState state;
	FilePath path;
	OGLTexHandle handle;
	unsigned int refcount;

	OGLTexture() : state(OGLTexState::LOADING_QUEUED), path(""), handle(DEFAULT_TEX_HANDLE), refcount(1) {}
};

static std::mutex texCountMutex;
static unsigned int loadedTexCount = 0;

#define NAMEMAP_SYNC_ISSUE_MSG "nameMap and textures out of sync"
#define DELTEX_SYNC_ISSUE_MSG "deletedTextures and textures out of sync"

//Internal texture manager
//only meant to be used in one single thread
//Is a singleton please refrain from making multiple OGLTexManagers
static class OGLTexManager
{
private:
	std::mutex textureMutex;
	std::vector<OGLTexture> textures;
	std::unordered_map<FileNameNoExt, TexId> nameMap;
	std::queue<TexId> deletedTextures;
	TexId bindedTexture; //-1 for none binded

	std::mutex loadMutex;
	std::queue<unsigned char*> loadData;
	std::queue<ImageMetaData> loadMetaData;

	std::mutex unloadMutex;
	std::queue<TexId> unloadQueue;

	//returns true if id exists
	//not thread-safe
	bool exists(TexId id)
	{
		return (id >= 0 && id < textures.size());
	}

public:
	//Loads the texture into RAM and queues it to be delivered to GPU
	//Return the TexId of the texture
	//Thread-safe
	TexId load(const FilePath& path)
	{
		TexId id = -1;
		//initial load request
		{
			std::lock_guard<std::mutex> lock(textureMutex);
			std::string name = FilePath_GetFileNameNoExt(path);
			auto it = nameMap.find(name);
			if(it != nameMap.end()) //texture exists already
			{
				id = it->second;
				if(!exists(id))
				{
					LOG_FATAL_PUSH(NAMEMAP_SYNC_ISSUE_MSG);
					return LOAD_FAIL_VALUE;
				}
#ifdef _DEBUG
				if(textures[id].path != path)
				{
					LOG_WARNING_PUSH("name conflict with texture file");
					return LOAD_FAIL_VALUE;
				}
				if(textures[id].state == OGLTexState::DELETED)
				{
					LOG_FATAL_PUSH(NAMEMAP_SYNC_ISSUE_MSG);
					return LOAD_FAIL_VALUE;
				}
#endif
				textures[id].refcount++; //inc ref count when texture exists
				
				if(textures[id].state == OGLTexState::DELETE_QUEUED)
					textures[id].state = OGLTexState::LOADED;
				return id; //exit here because texture is already loaded
			}
			else //texture does not exist
			{
				if(!deletedTextures.empty())
				{
					id = deletedTextures.front();
					deletedTextures.pop();

					if(!exists(id))
					{
						LOG_FATAL_PUSH(DELTEX_SYNC_ISSUE_MSG);
						return LOAD_FAIL_VALUE;
					}
#ifdef _DEBUG
					if(textures[id].state != OGLTexState::DELETED)
					{
						LOG_FATAL_PUSH(DELTEX_SYNC_ISSUE_MSG);
						return LOAD_FAIL_VALUE;
					}
#endif
					textures[id].state = OGLTexState::LOADING_QUEUED;
					textures[id].refcount = 1;
					textures[id].handle = DEFAULT_TEX_HANDLE;
				}
				else
				{
					id = textures.size();
					textures.push_back(OGLTexture());
				}
				textures[id].path = path;
				nameMap[name] = id;
			}
		} //textureMutex scope
		//At this point, the texture does not exist and needs loading

		//TODO finish writing this
		unsigned char* img;
		int width, height, channels;

		img = SOIL_load_image(path.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);

		{
			std::lock_guard<std::mutex> lock(loadMutex);
			loadMetaData.push(ImageMetaData{ id, width, height, channels });
			loadData.push(img); //ownership of the img data is passed onto loadData, which will delete it when it gets loaded to OGL in update()
		}

		return id;
	}

	//Decrements the refcount of the texture
	//If the refcount hits zero, queue the texture id for delete
	//Thread-safe
	void unload(TexId id)
	{
		bool addToUnloadQueue = false;
		{
			std::lock_guard<std::mutex> lock(textureMutex);

			if(!exists(id))
				return;
#ifdef _DEBUG
			if(textures[id].state != OGLTexState::LOADED)
			{
				LOG_WARNING_PUSH("Trying to unload a non-loaded texture");
				return;
			}
			else if(textures[id].refcount == 0)
			{
				LOG_FATAL_PUSH("texture with id " + std::to_string(id) + " has refcount 0 but is being requested to unload");
				return;
			}
#endif
			textures[id].refcount--;
			if(textures[id].refcount == 0) //signal queue for delete
			{
				textures[id].state = OGLTexState::DELETE_QUEUED;
				addToUnloadQueue = true;
			}
		}

		if(addToUnloadQueue)
		{
			std::lock_guard<std::mutex> lock(unloadMutex);
			unloadQueue.push(id);
		}
	}

	//Locks the internal texture lock mutex
	//DO THIS BEFORE YOU START BINDING!
	void lockTextures()
	{
		textureMutex.lock();
	}

	//Unlocks the internal texture lock mutex
	//DO THIS AFTER YOU ARE DONE RENDERING
	void unlockTextures()
	{
		textureMutex.unlock();
	}


	//Binds the texture for rendering if the TexId exists
	//Not thread-safe
	void bind(TexId id)
	{

	}

	//Unbinds any textures from OGL
	//aka binding the texture to 0
	//Not thread-safe
	void unbind()
	{

	}

	//clears loadData, unloadQueue, and updates texLoadCount
	//does internal updates
	void update()
	{
		int loadCount = 0;
		int unloadCount = 0;
		{
			std::lock_guard<std::mutex> texturelock(textureMutex);

			//loads data
			{
				std::lock_guard<std::mutex> lock(loadMutex);
				GLuint handle;
				ImageMetaData meta;
				while(!loadData.empty() && loadCount < 2) //2 is arbitrary TODO change to a timer based thing
				{
					glGenTextures(1, &handle);
					glBindTexture(GL_TEXTURE_2D, handle);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

					meta = loadMetaData.front();

					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, meta.width, meta.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, loadData.front());
					glGenerateMipmap(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, 0);

					textures[meta.id].handle = handle;
					textures[meta.id].state = OGLTexState::LOADED;
					SOIL_free_image_data(loadData.front());
					loadData.pop();
					loadMetaData.pop();

					loadCount++;
				}
			}

			//unloads data
			{
				std::lock_guard<std::mutex> lock(unloadMutex);
				TexId id;
				GLuint handle;
				while(!unloadQueue.empty() && unloadCount < 2) //TODO replace with some sort of timer thing
				{
					id = unloadQueue.front();
					if(textures[id].state == OGLTexState::DELETE_QUEUED)
					{
						handle = textures[id].handle;
						glDeleteTextures(1, &handle);
						textures[id].handle = LOAD_FAIL_VALUE;
						textures[id].state = OGLTexState::DELETED;

						deletedTextures.push(id);
						unloadCount++;
					}
					unloadQueue.pop();
				}
			}
		} //end of texturelock

		//updates loadedTexCount
		{
			std::lock_guard<std::mutex> lock(texCountMutex);
			loadedTexCount = loadedTexCount + loadCount - unloadCount;
		}
	}
};
