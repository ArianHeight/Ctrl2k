#pragma once
#include "Textures.h"

//This is the internal namespace for texture-related stuff aka renderer use only
namespace Textures
{
	//Binds the texture to OGL for rendering
	//Please lock textures before calling bind to do rendering and unlock after you're done
	//Range-guarded not thread-safe
	void Bind(TexId id);

	//Unbinds the texture from OGL
	//Range-guarded, not thread-safe
	void Unbind(TexId id);

	//create a texturelock object to lock textures, which will be automatically unlocked when the object
	//is destroyed
	class TextureLock;
	//This is for thread safety, please lock before any binding to do rendering
	void LockTextures();
	//THis is for thread safety, please unlock after all rendering is done for the frame
	void UnlockTextures();
}