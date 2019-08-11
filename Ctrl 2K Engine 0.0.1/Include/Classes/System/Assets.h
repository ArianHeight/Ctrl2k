#pragma once

/*

inclusive class for all game assets
music/animation/texture/sound in general

*/

//pre-declarations
class CoreEngine;

class Assets
{
public:
	Assets(CoreEngine &parent);
	~Assets();

private:
	CoreEngine &parentEngine;

	TextureManager textures; //all textures in game
	AnimationManager animations; //all animations in game
	AudioManager sounds; //all audio in game
	MapManager maps; //all in game maps
	FontManager fonts; //all game fonts

public:
	TextureManager& getTM(); //returns a reference to texture manager
	AnimationManager& getAM(); //returns a reference to animation manager
	AudioManager& getAuM(); //returns a reference to audio manager
	MapManager& getMM(); //returns a reference to map manager
	FontManager& getFM(); //returns a refernce to font manager
};

/*

end Assets

*/