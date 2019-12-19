#include <Ctrl 2kPCH.h>

/*

Main part of game engine

*/

CoreEngine::CoreEngine() :
	game(Game(*this)), sceneGraph(), physicsEngine(&game, nullptr, this->m_secondsElapsed), gameAssets(Assets(*this)), 
	renderingEngine(RenderEngine(*this)), particleEngine(ParticleEngine(*this)), soundEngine(*this), 
	controlHandler(*this)
{
	//cstr
	this->gameAssets.getMM().changeLoadState(); //change to loadscreen

	std::string name; //for game configs
	std::vector<int> data;
	//this->gameParser->parseConfigs(std::string("Resource/System/Graphics.config"), name, data);
	//this->renderingEngine.getMainWindow()->setConfig(name, data);
}

/*

Management

*/

void CoreEngine::updateTime()
{
	this->m_currentFrame = std::clock(); //sets time to current frame time
	this->m_secondsElapsed = (double)(this->m_currentFrame - this->m_previousFrame) / 1000; //subtract time of last frame from this frame and turn into seconds
	this->m_previousFrame = this->m_currentFrame; //sets last frame to this frame
}

void CoreEngine::calcAvgFPS()
{
	//increments second count using seconds elapsed since last frame until second count reaches 1s, incrementing fss by 1 every frame
	this->m_framesSinceLastSecond++;
	this->m_secondCount += this->m_secondsElapsed;

	//when 1s is reached, sets fps to fss since the second has now passed, resets stuff
	if (this->m_secondCount >= 1.0)
	{
		this->m_secondCount = 0;
		this->m_framesPerSecond = this->m_framesSinceLastSecond;
		this->m_framesSinceLastSecond = 0;

		//updates fps counter text
		//fpsWidget.setString(std::to_string(framesPerSecond) + " fps");
	}
}

/*

Event Managing

*/

void CoreEngine::setMap(int index)
{
	this->game.setMapTo(index);
	MapManager& mm = this->gameAssets.getMM();
	this->physicsEngine.mapChange(mm.data(index)); //physengine's ptr to collision map
	mm.data(index)->getCollisionData()->updateWindowAR(this->renderingEngine.getMainWindow()->getWindowArRef()); //adjust the window ar values in the map
	this->renderingEngine.getActiveCam()->changeClampValues(mm.getMinCamWP(index), mm.getMaxCamWP(index)); //adjust camera clamps
}

void CoreEngine::updateWindow()
{
	//this->renderingEngine.changeResolution();
}

void CoreEngine::setPlayer(std::vector<GameObject*>& target)
{
	if (target.size() == 0)
	{
		return; //early return
	}

	this->game.setPlayer(target[0]);
}

/*

saving

*/

bool CoreEngine::loadSaveState(SaveState *state)
{
	GameObject *player = this->game.getPlayer(); //sets player states
	if (player != nullptr)
	{
		Component *stats = player->getComponent(4); //sets state of player statistics
		stats->useHealth(stats->getHealth() - state->player_health);
		stats->useStamina(stats->getStamina() - state->player_stamina);
		stats->useMagic(stats->getMagic() - state->player_magic);

		Component *pos = player->getComponent(0); //sets the player's position
		pos->setDisp(state->player_positionW);
	}

	int mpIndex = this->gameAssets.getMM().search(state->current_map_name);
	this->setMap(mpIndex); //sets the map

	return true;
}

bool CoreEngine::saveGame()
{
	SaveState s;
	s.current_map_name = this->gameAssets.getMM().getName(this->game.getMapIndex()); //saves current map

	GameObject *player = this->game.getPlayer(); //saves player states
	if (player != nullptr)
	{
		Component *stats = player->getComponent(4); //saves state of player statistics
		s.player_health = stats->getHealth();
		s.player_stamina = stats->getStamina();
		s.player_magic = stats->getMagic();

		Component *pos = player->getComponent(0); //saves the player's position
		s.player_positionW = pos->getDisplacement();
	}

	s.writeSave();
	return true;
}

/*

callbacks

*/

void CoreEngine::idle()
{
	this->updateTime();
	this->calcAvgFPS();
	
	//iterate through global events call
	std::vector<GameEvent>& queue = this->game.getEventQueue();
	for (GameEvent e : queue)
	{
		switch (e.getType())
		{
		case RESOLUTION_CHANGE:
			this->updateWindow();
			break;
		case PLAYGROUND_CHANGE:
			this->setMap(this->game.getMapIndex()); //some var
			break;
		case PLAYER_RELATIVE_TELEPORT:
			break;
		case PLAYER_CHANGE:
			setPlayer(e.getTargets());
			break;
		}
	}
	this->game.clearEventQueue();

	this->game.getPlayer()->inputCommand(this->controlHandler.queryOutput());
	/* ********************************************Please Move A Bunch Of Stuff**********************
	//temporary, will be moved to controller class
	currentPlayer->takeInput(inputController->queryOutput());
	//for debugging
	if (CTRL_Kf)
	{
		std::vector<std::string> *outputs = inputController->queryOutput();
		for (std::string str : *outputs)
		{
			if (str != "")
			{
				std::cout << str << '\n';
			}
		}
	}
	*/
	//physics stuff, temporary will be put into a single method soon
	bool changeMap = false;
	this->physicsEngine.buildCollisionList(changeMap, this->game.getMapIndexRef());
	this->physicsEngine.updateObjects();
	// need different method of calling the map change
	if (changeMap)
	{
		this->game.getEventQueue().push_back(GameEvent(PLAYGROUND_CHANGE));
		//queue.push_back(PLAYER_RELATIVE_TELEPORT);
	}

	//particle updates
	this->particleEngine.update();
	
	//entity stuff
	this->game.update();
	this->renderingEngine.getActiveCam()->mainPhrase(); //updates camera
}

void CoreEngine::displayUpdate()
{
	/*
	glm::vec2 cameraPos = this->renderingEngine.getActiveCam()->getPos();
	glm::vec2 mapPos = glm::vec2(0.0f);*/

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, this->renderingEngine.getMainWindow()->getWindowWidthRef(), this->renderingEngine.getMainWindow()->getWindowHeightRef());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//this->renderingEngine.drawMap(this->game.getMapIndex()); //draws the map ************** NEEDED**************!!!!

	//draws all entities
	//glm::vec3 pos = glm::vec3(currentPlayer->getPos(), currentPlayer->getDepth());
	this->game.render();
	//this->renderingEngine.renderScene(this->game.getScene());
	this->particleEngine.render();
	/*
	float hpPercent = glm::clamp(this->game.getPlayer()->getComponent(std::string("Statistics"))->getHealth() / 100.0f, 0.0f, 1.0f);
	healthBar->setPercentage(hpPercent);
	staminaBar->setPercentage(currentPlayer->getStamina() / 100.0);
	manaBar->setPercentage(currentPlayer->getMagic() / 100.0);*/

	this->renderingEngine.updateScreen();
}

void CoreEngine::keyHandle(SDL_Event &event)
{
	this->controlHandler.pressKey(event.key.keysym.sym);
	if (event.key.keysym.sym == 'f')
	{
		this->particleEngine.addEffect(new ExplosiveFX(glm::vec2(0.0f), 0.25, -0.00001, 0.1f, 40));
	}
}

void CoreEngine::keyUpHandle(SDL_Event &event)
{
	this->controlHandler.offKey(event.key.keysym.sym);
}

void CoreEngine::mouseHandle(SDL_Event &event)
{
	this->controlHandler.pressMouseButton(event.button.button);
}

void CoreEngine::mouseUpHandle(SDL_Event &event)
{
	this->controlHandler.offMouseButton(event.button.button);
}

void CoreEngine::mouseMotionHandle(SDL_Event &event)
{
	this->controlHandler.mousePosition(glm::vec2(event.motion.x, event.motion.y));
}

/*

accessors

*/

Assets& CoreEngine::getAssets()
{
	return this->gameAssets;
}

AudioEngine& CoreEngine::getSoundEngine()
{
	return this->soundEngine;
}

RenderEngine& CoreEngine::getRenderEngine()
{
	return this->renderingEngine;
}

ParticleEngine& CoreEngine::getParticleEngine()
{
	return this->particleEngine;
}

PhysEngine& CoreEngine::getPhysicsEngine()
{
	return this->physicsEngine;
}

Controller& CoreEngine::getController()
{
	return this->controlHandler;
}

Game& CoreEngine::getGame()
{
	return this->game;
}

double& CoreEngine::getTimeRef()
{
	return this->m_secondsElapsed;
}

double* CoreEngine::getTimePtr()
{
	return &(this->m_secondsElapsed);
}

double CoreEngine::getTime()
{
	return this->m_secondsElapsed;
}

double CoreEngine::getFPS()
{
	return this->m_framesPerSecond;
}

CoreEngine::~CoreEngine()
{
	//dstr
	this->sceneGraph.deleteGO(); //deletes all objects in game
	this->sceneGraph.update();
}

/*

end CoreEngine

*/