#pragma once

/*

the core engine that is being used

*/

class CoreEngine
{
public:
	CoreEngine();
	~CoreEngine();

private:
	//basic time stuff
	std::clock_t m_currentFrame; //time of current frame in milliseconds
	std::clock_t m_previousFrame; //time of previous frame in milliseconds
	double m_secondsElapsed; //seconds elapsed since last frame

	//for fps counter
	int m_framesSinceLastSecond{ 0 };
	int m_framesPerSecond{ 0 };
	double m_secondCount{ 0 };

	Game game;
	GameObject sceneGraph;
	Assets gameAssets;
	PhysEngine physicsEngine;
	RenderEngine renderingEngine;
	ParticleEngine particleEngine;
	AudioEngine soundEngine;
	Controller controlHandler;

public:
	//management methods
	void updateTime(); //updates m_secondsElapsed every frame to make it seconds elapsed since last frame
	void calcAvgFPS(); //updates the fps every frame

	//event managing
	void setMap(int index);
	void updateWindow();
	void setPlayer(std::vector<GameObject*>& target); //only uses the first one in list

	//saving
	bool loadSaveState(SaveState *state); //reads and loads a save state
	bool saveGame(); //saves the game

	//callbacks
	void displayUpdate(); //runs every frame to refresh the screen
	void idle(); //this functions runs every frame like the displayUpdate function
	void keyHandle(SDL_Event &event); //runs everytime to update key states
	void keyUpHandle(SDL_Event &event); //same as above
	void mouseHandle(SDL_Event &event); //runs everytime to update mouse events
	void mouseUpHandle(SDL_Event &event); //same as above
	void mouseMotionHandle(SDL_Event &event); //same as above

	//accessors
	Assets& getAssets();
	AudioEngine& getSoundEngine();
	RenderEngine& getRenderEngine();
	ParticleEngine& getParticleEngine();
	PhysEngine& getPhysicsEngine();
	Controller& getController();
	Game& getGame();
	double& getTimeRef();
	double* getTimePtr();
	double getTime();
	double getFPS();
};

/*

end core engine

*/