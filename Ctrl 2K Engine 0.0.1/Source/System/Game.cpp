#include <Ctrl 2kPCH.h>

/*

Game

*/

Game::Game(CoreEngine &engine) :
	parentEngine(engine)
{
	//cstr
	this->globalActiveObjects = new GameObject();
	this->globalActiveObjects->linkGame(this);
	//this->globalActiveObjects->setLinkCam(this->parentEngine.getRenderEngine().getActiveCam());
	this->entities = new GameObject;
	this->entities->linkGame(this);
	this->playArea = new GameObject;
	this->entities->linkGame(this);
	this->widgets = new GameObject;
	this->widgets->linkGame(this);

	this->globalActiveObjects->addChild(this->entities); //passes ownership here
	this->globalActiveObjects->addChild(this->playArea); //passes ownership here
	this->globalActiveObjects->addChild(this->widgets); //passes ownership here
}

void Game::addObj(const std::string &category, GameObject *obj)
{
	obj->linkGame(this);
	if (category == "entity")
	{
		this->entities->addChild(obj);
	}
	else if (category == "playArea")
	{
		this->playArea->addChild(obj);
	}
	else if (category == "widgets")
	{
		this->widgets->addChild(obj);
	}
}

void Game::clearEventQueue()
{
	this->globalEventsQueue.clear();
}

void Game::render()
{
	this->playArea->render();
	this->entities->render();
	this->widgets->render();
	/*
	for (auto it = this->globalActiveObjects.begin(); it != this->globalActiveObjects.end(); it++)
	{
		it->second->render();
	}*/
}

void Game::update()
{
	this->globalActiveObjects->update();
	/*
	for (auto it = this->globalActiveObjects.begin(); it != this->globalActiveObjects.end(); it++)
	{
		it->second->update();
	}*/
}

//mutators
void Game::setMapTo(int index)
{
	this->playArea->clearChildrenPtr(1); //clears the ptr to map

	this->currentMapIndex = index;
	MapManager& mm = this->parentEngine.getAssets().getMM();
	mm.useMap(index);
	Map *mPtr = mm.data(index);
	mPtr->linkGame(this);

	this->playArea->addChild(mPtr);
}

void Game::setPlayer(GameObject* newPlayer)
{
	this->player = newPlayer;
}

//for debugging
void Game::debugSetup()
{
	//sets up refs
	RenderEngine &rEngine = this->parentEngine.getRenderEngine();
	PhysEngine &pEngine = this->parentEngine.getPhysicsEngine();
	AudioEngine &sEngine = this->parentEngine.getSoundEngine();
	Game &game = this->parentEngine.getGame();
	Controller &inputController = this->parentEngine.getController();

	TextureManager &textures = this->parentEngine.getAssets().getTM(); //all textures in game
	AnimationManager &animations = this->parentEngine.getAssets().getAM(); //all animations in game
	AudioManager &sounds = this->parentEngine.getAssets().getAuM(); //all audio in game
	MapManager &maps = this->parentEngine.getAssets().getMM(); //all in game maps
	FontManager &fonts = this->parentEngine.getAssets().getFM(); //all game fonts
	ThreadManager gameThreads; //all in game **not including sdl and framework stuff**

	TBuffer *gl_tBuffer;
	Camera *activeCam; //the active camera doing the rendering
	Player *currentPlayer; //current player ***temporary***
	Player *newTargetPlayer; //the player entity which the game is trying to switch player control to
	Map *activePlayground = nullptr; //current active map
	int activePlaygroundIndex = 0;

	GLint texture = -1;
	GLint victor = -1;
	GLint ballTexture = -1;
	int gameFont = -1;
	DamageArea *testArea = nullptr;
	DamageArea *testAreaTwo = nullptr;
	GUI *headsUpDisplay;
	Text *loremIpsum;
	Text *fpsWidget;
	StatusBar *healthBar = nullptr;
	StatusBar *staminaBar = nullptr;
	StatusBar *manaBar = nullptr;

	rEngine.init();
	pEngine.mapChange(activePlayground);

	//activates maps and textures
	maps.reserve(1);
	textures.reserve(3);

	//creates buffers
	gl_tBuffer = new TBuffer(rEngine.getMainWindow()->getWindowWidthRef() * rEngine.getMainWindow()->getParallaxToWindowRef(), rEngine.getMainWindow()->getWindowHeightRef() * rEngine.getMainWindow()->getParallaxToWindowRef());

	//activate camera
	activeCam = rEngine.getActiveCam();

	//debug all temporary code....
	sounds.addMusic(std::string("Running in The 90s"), std::string("Resource/Audio/Initial D - Running in The 90s.wav"));
	sounds.addSFX(std::string("beat"), std::string("Resource/Audio/beat.wav"));
	Parser temp(maps, textures, fonts, animations);
	victor = temp.parseDotAnim(std::string("../../Resource/Animations/victor_walk.anim"));
	texture = temp.parseDotAnim(std::string("../../Resource/Animations/turret_rest.anim"));
	ballTexture = temp.parseDotAnim(std::string("../../Resource/Animations/ball.anim"));
	gameFont = temp.parseDotFNT(std::string("../../Resource/System/Fonts/visitor/visitor.fnt"));
	//temp.parseDotFNT(std::string("Resource/System/Fonts/quantum/quantum.fnt"));
	//temp.parseDotFNT(std::string("Resource/System/Fonts/perfect_dark/perfect_dark.fnt"));
	activePlaygroundIndex = temp.parseDotMP(std::string("../../Resource/Map/test.mp"));
	temp.parseDotMP(std::string("../../Resource/Map/test1.mp"));
	healthBar = new StatusBar(std::string("health bar"), glm::dvec2(-1.0, 0.99), glm::dvec2(-0.25, 0.9011111), textures.create(std::string("woodenbar"), std::string("Resource/Texture/Widgets/woodenBarBackground.png")),
		glm::dvec2(-0.95703125, 0.6875), glm::dvec2(0.95703125, -0.6875), textures.create(std::string("barFiller"), std::string("Resource/Texture/Widgets/barFiller.png")));
	healthBar->setColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	staminaBar = new StatusBar(std::string("stamina bar"), glm::dvec2(-1.0, 0.901111), glm::dvec2(-0.25, 0.8122222), textures.search(std::string("woodenbar")),
		glm::dvec2(-0.95703125, 0.6875), glm::dvec2(0.95703125, -0.6875), textures.search(std::string("barFiller")));
	staminaBar->setColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	manaBar = new StatusBar(std::string("mana bar"), glm::dvec2(-1.0, 0.8122222), glm::dvec2(-0.25, 0.7233333), textures.search(std::string("woodenbar")),
		glm::dvec2(-0.95703125, 0.6875), glm::dvec2(0.95703125, -0.6875), textures.search(std::string("barFiller")));
	manaBar->setColour(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	//widgets
	headsUpDisplay = new GUI(std::string("HUD"), glm::dvec2(-1.0, 1.0), glm::dvec2(1.0, -1.0));
	fpsWidget = new Text(std::string("fps counter"), fonts, rEngine.getMainWindow()->getSPixelToWindowRef(), 0, std::string("0 fps"), glm::dvec2(0.5, 1.0), glm::dvec2(1.0, 0.5), 2, 64, glm::vec4(0.2f, 0.75f, 0.3f, 0.9f));
	loremIpsum = new Text(std::string("lorem ipsum"), fonts, rEngine.getMainWindow()->getSPixelToWindowRef(), 0, std::string("Lorem ipsum dolor sit amet, consecteur adipiscing elit. Praesent imperdiet nibh ac lorem commodo, in ornare mi accumsan.Cras pharetra, justo eu porta condimentum, nunc dui viverra arcu, sed posuere."),
		glm::dvec2(-0.5, 1.0), glm::dvec2(0.5, 0.0), 1, 64, glm::vec4(1.0f, 1.0f, 1.0f, 0.75f));
	this->addObj(std::string("widgets"), headsUpDisplay);
	headsUpDisplay->addChild(healthBar);
	headsUpDisplay->addChild(staminaBar);
	headsUpDisplay->addChild(manaBar);
	headsUpDisplay->addChild(fpsWidget);
	//headsUpDisplay->addChild(&loremIpsum);

	inputController.changeKeyBinding('w', std::string("MOVE_UP"));
	inputController.changeKeyBinding('a', std::string("MOVE_LEFT"));
	inputController.changeKeyBinding('s', std::string("MOVE_DOWN"));
	inputController.changeKeyBinding('d', std::string("MOVE_RIGHT"));

	//adds map connectors
	FourPoints rectangleOne{ glm::vec2(-30.0f, 3.0f), glm::vec2(-29.5f, 3.0f), glm::vec2(-30.0f, -2.0f), glm::vec2(-29.5f, -2.0) };
	FourPoints rectangleTwo{ glm::vec2(-30.5f, 3.0f), glm::vec2(-30.0f, 3.0f), glm::vec2(-30.5f, -2.0f), glm::vec2(-30.0f, -2.0) };
	//maps.addConnector(std::string("test"), std::string("test1"), rectangleTwo, rectangleOne);
	//maps.data(std::string("test"))->addComponent(std::string("tpOne"), new Connector(rectangleTwo, std::string("test1"), rectangleOne));
	//maps.data(std::string("test1"))->addComponent(std::string("tpZero"), new Connector(rectangleOne, std::string("test"), rectangleTwo));
	maps.updateAllAdjMapIndices();
	rectangleOne.tl = glm::vec2(0.0f);
	rectangleOne.tr = glm::vec2(0.0f);
	rectangleOne.bl = glm::vec2(0.0f);
	rectangleOne.br = glm::vec2(0.0f);
	rectangleTwo.tl = glm::vec2(0.0f);
	rectangleTwo.tr = glm::vec2(0.0f);
	rectangleTwo.bl = glm::vec2(0.0f);
	rectangleTwo.br = glm::vec2(0.0f);

	this->parentEngine.setMap(activePlaygroundIndex); //sets the map

	//player
	Statistics playerStats{ 100, 2.0, 3.5, 6.0, 100, 0.05, 0.5, 100, 0.75, 100, 20, 0.5, 4.0, 8.0, 14.0, 0.01, 15, 1.5, 1.0, 1.0, 1.1, 2, 1, 0.8f, 0.15f, 0.3f };
	currentPlayer = new Player(this->parentEngine.getTimeRef(), std::string("Victor"), new RectangleHB(glm::vec2(0.0f), glm::vec2(1.0f), 15.0f));
	/*currentPlayer = new Player(dmgAreas, gl_dSecondsElapsed, std::string("Victor"), playerStats, std::vector<int>{victor, victor, victor}, glm::vec2(0.0),
	FourPoints{glm::vec2(-0.5f, 0.5f), glm::vec2(0.5f, 0.5f), glm::vec2(-0.5f, -0.5f), glm::vec2(0.5f, -0.5f)}, 10.0f, 0.96f, 0.5f, 1.0f);*/
	currentPlayer->usePlayer();
	newTargetPlayer = currentPlayer; /*
	Entity* shootingThing = new NonMovingShooter(ballTexture, this->parentEngine.getTimeRef(), std::string("Turret3"),
		new RectangleHB(glm::vec2(0.0f), glm::vec2(1.6f), 45.0f), glm::vec2(0.0f, 10.0f), AGGRESSIVE, 0.0f, 0.0f, 0.0f, 15.0f, LEFT);
	shootingThing->addComponent(std::string("Turret3"), new VisualData(TEXTURE, texture, glm::vec2(1.0f, 1.0f)));
	this->addObj(std::string("entity"), shootingThing);
	/*entities.push_back(new NonMovingShooter(ballTexture, dmgAreas, gl_dSecondsElapsed, std::string("Turret3"), playerStats, std::vector<int>{texture, texture, texture}, glm::vec2(0.0f, 10.0f),
	FourPoints{glm::vec2(-0.8f, 0.8f), glm::vec2(0.8f, 0.8f), glm::vec2(-0.8f, -0.8f), glm::vec2(0.8f, -0.8f) }, 9999.99f, 0.96f, 1.0f, 1.0f, 15.0f, LEFT));*/
	//currentPlayer->addArmour(100); //gives player 100 armour
	activeCam->setTarget(currentPlayer);
	//currentPlayer->addComponent(std::string("victor_walk"), new VisualData(ANIMATION, victor, glm::vec2(0.5f, 1.0f)));
	this->addObj(std::string("entity"), currentPlayer);
	this->setPlayer(currentPlayer);
}

//accessors
CoreEngine& Game::getEngine()
{
	return this->parentEngine;
}

void Game::getAllPOs(std::vector<Component*> &output)
{
	this->globalActiveObjects->getAllActiveComponentsC(output, PHYSOBJECT);
	/*
	for (auto it = this->globalActiveObjects.begin(); it != this->globalActiveObjects.end(); it++)
	{
		it->second->getAllComponentByTypeB(output, PHYSOBJECT);
	}*/
}

std::vector<GameEvent>& Game::getEventQueue()
{
	return this->globalEventsQueue;
}

int Game::getMapIndex()
{
	return this->currentMapIndex;
}

int& Game::getMapIndexRef()
{
	return this->currentMapIndex;
}

GameObject* Game::getPlayer()
{
	return this->player;
}

GameObject* Game::getScene() {
	return this->globalActiveObjects;
}

Game::~Game()
{
	//dstr for cleanup
	this->entities = nullptr; //prevents hanging ptrs
	this->playArea = nullptr;

	delete this->globalActiveObjects; //deletes all objects in game
	this->globalActiveObjects = nullptr;
	/*
	for (auto it = this->globalActiveObjects.begin(); it != this->globalActiveObjects.end(); it++)
	{
		if (it->second != nullptr)
		{
			delete it->second;
			it->second = nullptr;
		}
	}
	this->globalActiveObjects.clear();*/
}

/*

end Game

*/