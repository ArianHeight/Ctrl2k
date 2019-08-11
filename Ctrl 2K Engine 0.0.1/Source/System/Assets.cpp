#include <Ctrl 2kPCH.h>

/*

Assets in game

*/

Assets::Assets(CoreEngine &parent) :
	parentEngine(parent), textures(TextureManager()), animations(AnimationManager(textures)), sounds(AudioManager()), 
	maps(MapManager(textures, parentEngine.getRenderEngine().getMainWindow()->getPixelToWindowRef())), fonts(FontManager())
{
	//cstr
}

//accessors
TextureManager& Assets::getTM()
{
	return this->textures;
}

AnimationManager& Assets::getAM()
{
	return this->animations;
}

AudioManager& Assets::getAuM()
{
	return this->sounds;
}

MapManager& Assets::getMM()
{
	return this->maps;
}

FontManager& Assets::getFM()
{
	return this->fonts;
}

Assets::~Assets()
{
	//dstr
}

/*

end Assets

*/