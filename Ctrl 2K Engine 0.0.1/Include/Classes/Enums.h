#pragma once

/*

various enumerated classes

*/

//for anything
struct FourPoints
{
	glm::vec2 tl;
	glm::vec2 tr;
	glm::vec2 bl;
	glm::vec2 br;
};

struct Manifest
{
	glm::vec2 normal;
	float magnitude;
	float depthChange;
};

//for tile object
enum TileType
{
	BACKGROUND_TILE, //layer 0
	PLAYGROUND_TILE, //layer 1
	FOREGROUND_TILE //layer 2
};

enum EntityType
{
	PLAYABLE, //0
	FRIENDLY, //1
	PASSIVE, //2
	FRIENDLY_PASSIVE, //3
	NEUTRAL, //4
	NEUTRAL_PASSIVE, //5
	NEUTRAL_AGGRESSIVE, //6
	AGGRESSIVE, //7
	MINI_BOSS, //8
	BOSS, //9
	MECHANISM, //10
	PLATFORM, //11
	DOOR, //12
	CURRENT_PLAYER //13
};

//for active effects
enum Effects
{
	POISON, //0
	RAGE, //1
	NUMBNESS, //2
	SENSITVE, //3
	CRAZED, //4
	DISEASED, //5
	HUNGRY, //6
	TIRED, //7
	SICK, //8
	LIGHT_FOOTED, //9
	HEAVY_FOOTED, //10
	BRONZE_SKIN, //11
	PAPER_SKIN, //12
	LOG_MUSCLES, //13
	STICK_MUSCLES, //14
	BLURRED_VISION, //15
	EAGLE_SIGHT, //16
	STUNNED //17
};

//for effects
struct ActiveEffect
{
	int severity; //starts from 1
	Effects fx;
	double timeLeft;
};

//for component
enum ComponentType {
	TRANSFORMATION, //0
	RENDERER, //1
	PHYSOBJECT, //2
	VISUAL_DATA, //3
	ENTITY_DATA, //4
	MOVE_DATA, //5
	CONNECTOR, //6
	DMG_VALUE //7
};

//for physobject
enum PhysType
{
	RECTANGLE, //0
	CIRCLE, //1
	FAN //2
};

enum RenderType
{
	TEXTURE, //0
	ANIMATION, //1
	TEXT, //2
	MAP, //3
	WIDGET, //4
	NONE //5
};

//for entities
struct Statistics
{
	int max_health;
	double health_fastRegen_seconds;
	double health_midRegen_seconds;
	double health_slowRegen_seconds;
	int max_stamina;
	double stamina_fastRegen_seconds;
	double stamina_slowRegen_seconds;
	int max_magic;
	double magic_regen_seconds;
	int max_special;
	int pain_tolerance;
	double pain_fade_seconds;
	float speed_one;
	float speed_two;
	float speed_three;
	float critical_hit_chance;
	int strength;
	float critical_damage_multiplier;
	float dodge_duration;
	float max_dodge_distance; //dodge_duration - max_dodge_distance / dodge_speed = dodge cooldown
	float dodge_speed; //per second
	int defense;
	int criticalHitDefense;
	float m_healthThresholdMid;
	float m_healthThresholdLow;
	float m_healthThresholdMidLow;
};

//for texture object
enum ActiveState
{
	ACTIVE, //0
	DISABLED //1
};

//for map object
enum LoadState
{
	LOADED, //0
	UNLOADED //1
};

//for map manager object
enum PreLoadState
{
	LOAD_SCREEN_ENABLED, //0
	PRELOAD_SURROUNDINGS_1, //1 preload immediate surroundings(preload all maps that are 1 link apart from current), default value
	PRELOAD_SURROUNDINGS_2 //2 preload all maps that are two links apart to current map
};

//for events queue
enum EventType
{
	RESOLUTION_CHANGE, //0
	PLAYGROUND_CHANGE, //1
	PLAYER_CHANGE, //2
	PLAYER_RELATIVE_TELEPORT //3
};

//for animation
enum Orientation
{
	UP, //0
	DOWN, //1
	LEFT, //2
	RIGHT, //3
	UPPER_LEFT, //4
	UPPER_RIGHT, //5
	LOWER_LEFT, //6
	LOWER_RIGHT //7
};

//for parser
enum TypeOfToken
{
	WHITESPACE, //0
	EOL, //1 End of Line
	STRING_LITERAL, //2 string literal
	INT_CONSTANT, //3 integer
	FLOATING_CONSTANT, //4 float
	IDENTIFIER, //5 identifier
	KEYWORD, //6 keywords
	PUNCTUATOR, //7 (), {} and other stuff like that as well as all operators
	INVALID //8 invalid tokens
};