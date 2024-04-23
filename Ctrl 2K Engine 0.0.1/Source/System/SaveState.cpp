#define _CRT_SECURE_NO_WARNINGS
#include <Ctrl 2kPCH.h>

/*

code for SaveState

*/

SaveState::SaveState()
{
	//cstr
}

bool SaveState::readSave(const std::string &filePath)
{
	std::ifstream save(filePath.c_str()); //opens file for reading

	if (!save) //checks if file has been opened correctly
	{
		std::cerr << "Was Not Able To Open Save File " << filePath << " For Reading" << std::endl;
		return false;
	}

	//reads data from save file
	std::string dump;
	std::getline(save, dump); //dumps the date which is the first line
	save >> this->player_health;
	save >> this->player_stamina;
	save >> this->player_magic;
	save >> this->player_positionW.x;
	save >> this->player_positionW.y;
	save >> this->current_map_name;

	//closes the file
	save.close();
	return true;
}

bool SaveState::writeSave(const std::string &filePath)
{
	std::ofstream save(filePath.c_str(), std::ios::trunc); //opens file for writing while truncating the data

	if (!save) //checks if file has been opened correctly
	{
		std::cerr << "Was Not Able To Open Save File For Writing" << std::endl;
		return false;
	}

	//finds the current time of the save
	std::time_t temp = std::time(nullptr);
	std::string current_time = "";
	current_time += std::asctime(std::localtime(&temp));

	//writes to file
	save << current_time << '\n';
	save << this->player_health << '\n';
	save << this->player_stamina << '\n';
	save << this->player_magic << '\n';
	save << this->player_positionW.x << ' ' << this->player_positionW.y << '\n';
	save << this->current_map_name << '\n';

	//closes the file
	save.close();
	return true;
}

SaveState::~SaveState()
{
	//dstr
}

/*

end SaveState

*/