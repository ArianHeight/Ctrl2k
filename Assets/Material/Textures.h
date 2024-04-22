#pragma once
#include "../../../Core/GreatBath/FilePathProcessing.h"

#define LOAD_FAIL_VALUE 0
#define DEFAULT_TEX_HANDLE 0
typedef unsigned int OGLTexHandle; //OGL's texture handle
typedef int TexId;

//This is the global namespace for all texture-related stuff
namespace Textures
{
	//loads the texture into OGL if it's not already loaded
	//Increment the internal reference count for the texture
	//Only increments the ref count if texture has already been loaded
	//Range-guarded and thread-safe
	//Will SLEEP the current thread until the load is complete
	//PLEASE REFRAIN from using load on an important/non-loader thread
	TexId Load(const FilePath& path);
	//Decrements the internal reference count for the texture
	//Unloads and queues the texture to be deleted from OGL if ref count hits 0
	//Range-guarded and thread-safe
	void Unload(TexId id);
	//Returns the TexId for the given filename
	//Returns -1 if the texture doesn't exist
	//thread-safe
	TexId GetTexId(const FileName& name);
	
	//Returns the amount of textures currently loaded
	//thread-safe
	unsigned int CurrentTexturesLoaded();
}