#include <queue>
#include <vector>
#include <unordered_map>
#include <mutex>

// TODO Make this use dependencies outside ofthe main project folder(ie put deps in sln folder instead?)

#define GLEW_STATIC
#include "../../Ctrl 2K Engine 0.0.1/Include/GL/glew.h"
#ifdef _WIN32
#include "../../Ctrl 2K Engine 0.0.1/Include/GL/wglew.h"
#endif

#include "../../Ctrl 2K Engine 0.0.1/Dependencies/SOIL for opengl 3.0 context and above/src/SOIL.h"

#include "../../Core/GreatBath/Logger.h"
#include "TexturesInternal.h"
#include "../AssetsCore/AssetsManager.h"

static_assert(std::is_same<AssetId, TexId>::value, "TexId is not the same type as AssetId");

static struct ImageMetaData : public AssetMetaDataTemplate
{
	int width;
	int height;
	int channels;
};

static struct OGLTexture : public AssetTemplate
{
	OGLTexHandle handle;

	OGLTexture() : handle(DEFAULT_TEX_HANDLE) {}
};

//Internal texture manager
//only meant to be used in one single thread
//Is a singleton please refrain from making multiple OGLTexManagers
static class OGLTexManager : public InternalAsyncAssetsManager<OGLTexture, unsigned char*, ImageMetaData>
{
private:
	TexId bindedTexture; //-1 for none binded

public:
	OGLTexManager() : bindedTexture(-1) {}

	//Loads the texture into RAM and queues it to be delivered to GPU
	//Return the TexId of the texture
	//Thread-safe
	TexId load(const gbt::FilePath& path)
	{
		return loadAssetInternal(
			path, 
			[=](unsigned char*& img, ImageMetaData& meta)->void //loadFunc
			{
				int width, height, channels;

				img = SOIL_load_image(path.path().c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);
				meta.width = width;
				meta.height = height;
				meta.channels = channels;
			});
	}

	//Decrements the refcount of the texture
	//If the refcount hits zero, queue the texture id for delete
	//Thread-safe
	void unload(TexId id)
	{
		unloadAssetInternal(id);
	}

	//Locks the internal texture lock mutex
	//DO THIS BEFORE YOU START BINDING!
	void lockTextures()
	{
		lockAssetsInternal();
	}

	//Unlocks the internal texture lock mutex
	//DO THIS AFTER YOU ARE DONE RENDERING
	//AT THIS POINT THERE SHOULD BE NO TEXTURES BINDED
	void unlockTextures()
	{
#ifdef _DEBUG
		unbind(); //just to be sure
#endif
		unlockAssetsInternal();
	}


	//Binds the texture for rendering if the TexId exists
	//Not thread-safe
	void bind(TexId id)
	{
		bindedTexture = id;
		glBindTexture(GL_TEXTURE_2D, assets[id].handle);
	}

	//Unbinds any textures from OGL
	//aka binding the texture to 0
	//Not thread-safe
	void unbind()
	{
		bindedTexture = -1;
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//returns the ogl handle of the current binded texture
	//NOT thread-safe, requires lock() call before calling
	//Use getSafeBindedTextureHandle for thread-safe implementation
	OGLTexHandle getBindedTextureHandle()
	{
		return bindedTexture > -1 ? assets[bindedTexture].handle : -1;
	}

	//returns the ogl handle of the current binded texture
	//thread-safe
	OGLTexHandle getSafeBindedTextureHandle()
	{
		lockAssetsInternal();
		OGLTexHandle handle = bindedTexture > -1 ? assets[bindedTexture].handle : -1;
		unlockAssetsInternal();
		return handle;
	}

	//clears loadData, unloadQueue, and updates texLoadCount
	//does internal updates
	void update()
	{
		GLuint handle;
		updateAssetsInternal(
			[=](unsigned char*& img, ImageMetaData& meta)->void //unloadFunc
			{
				SOIL_free_image_data(loadData.front());
			},
			[&](unsigned char*& img, ImageMetaData& meta)->void //internalLoadFunc
			{
				glGenTextures(1, &handle);
				glBindTexture(GL_TEXTURE_2D, handle);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, meta.width, meta.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, loadData.front());
				glGenerateMipmap(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);

				assets[meta.id].handle = handle;
			},
			[&](TexId id)->void //internalUnloadFunc
			{
				handle = assets[id].handle;
				glDeleteTextures(1, &handle);
				assets[id].handle = LOAD_FAIL_VALUE;
			});
	}
};
