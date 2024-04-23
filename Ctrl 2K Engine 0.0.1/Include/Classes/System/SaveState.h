#pragma once

/*

SaveState Object is created by engine to store all necessary states of a game

*/

//for friending
class CoreEngine;

class SaveState
{
public:
	SaveState();
	~SaveState();

	friend class CoreEngine; //allows core engine to access private variables
private:
	float player_health{0.0f};
	glm::vec2 player_positionW{0.0f, 0.0f};
	float player_stamina{0.0f};
	float player_magic{0.0f};
	std::string current_map_name{""};

public:
	bool writeSave(std::string &filePath = std::string("../Resource/Data/0001.sav"));
	bool readSave(std::string &filePath = std::string("../Resource/Data/0001.sav"));
};

/*

end SaveState

*/