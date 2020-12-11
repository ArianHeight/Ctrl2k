/*

Ctrl 2K 2D Game Engine

*/

//#include "stdafx.h"
//#include <Ctrl 2kPCH.h>
#include "../Include/Ctrl 2kPCH.h"

/*

Engine

*/

CoreEngine gameEngine = CoreEngine();
RenderEngine &rEngine = gameEngine.getRenderEngine();
PhysEngine &pEngine = gameEngine.getPhysicsEngine();
AudioEngine &sEngine = gameEngine.getSoundEngine();
Game &game = gameEngine.getGame();
Controller &inputController = gameEngine.getController();

/*

end Engine

*/

/*

buffers

*/

std::vector<bool> key(128, 0); //bools of which keys are pressed
//std::vector<Entity*> entities; //list of all active entities
TextureManager &textures = gameEngine.getAssets().getTM(); //all textures in game
AnimationManager &animations = gameEngine.getAssets().getAM(); //all animations in game
AudioManager &sounds = gameEngine.getAssets().getAuM(); //all audio in game
MapManager &maps = gameEngine.getAssets().getMM(); //all in game maps
FontManager &fonts = gameEngine.getAssets().getFM(); //all game fonts
ThreadManager gameThreads; //all in game **not including sdl and framework stuff**

/*

end buffers

*/

/*

system stuffs

*/

bool pause = false;

//sdl window setup
//SDL_Window* sdlWindow;
//SDL_GLContext glContext; //openGL context for sdl

//int gl_iCenter[2] = { 0, 0 }; //center point of the window
//GLuint gl_iWindowHandle; //window object
//for drawing squares, Vertex Array Objects and Vertex Buffer Objects
GLuint quadVao = 0;
GLuint quadVbo;
//for drawing lines
GLuint lineVao = 0;
GLuint lineVbo;

glm::ivec2 gl_iMousePos;
TBuffer *gl_tBuffer;
Camera *activeCam; //the active camera doing the rendering
Player *currentPlayer; //current player ***temporary***
Player *newTargetPlayer; //the player entity which the game is trying to switch player control to
Map *activePlayground; //current active map
int activePlaygroundIndex = 0;
std::vector<EventType> globalEventsQueue;

std::clock_t gl_cCurrentFrame; //time of current frame
std::clock_t gl_cPreviousFrame; //time of previous frame
double gl_dSecondsElapsed; //seconds elapsed since last frame

int framesSinceLastSecond = 0; //for fps counter
int framesPerSecond = 0;
double secondCount = 0;

//PhysEngine physicsEngine(entities, dmgAreas, activePlayground, gl_dSecondsElapsed); //physics engine

/*

end system stuffs

*/

/*

shader stuffs

*/

GLint parallaxBool = -1;
GLint parallaxWeight = -1;
GLint parallaxScale = -1;
GLint parallaxNormal = -1;
GLint parallaxDepth = -1;
GLint parallaxAlbedo = -1;

GLint txtCursor = -1;
GLint txtOffsets = -1;
GLint txtWS = -1;
GLint txtUnitScreenRatio = -1;
GLint txtAR = -1;
GLint txtTLBR = -1;
GLint txtBit = -1;
GLint txtColour = -1;

GLint wgtDimensions = -1;
GLint wgtBoxTopLeft = -1;
GLint wgtTintColour = -1;
GLint wgtBit = -1;

/*

end shader stuffs

*/

//testing values
GLint texture = -1;
GLint victor = -1;
GLint ballTexture = -1;
GLint turretDeathTexture = -1;
int gameFont = -1;
DamageArea *testArea = nullptr;
DamageArea *testAreaTwo = nullptr;
HUD *headsUpDisplay;
Text *loremIpsum;
Text *fpsWidget;
StatusBar *healthBar = nullptr;
StatusBar *staminaBar = nullptr;
StatusBar *manaBar = nullptr;

//callbacks
void resizeWindow(int w, int h);

//updates gl_dSecondsElapsed every frame to make it seconds elapsed since last frame
void updateTime()
{
	gl_cCurrentFrame = std::clock();
	gl_dSecondsElapsed = (double)(gl_cCurrentFrame - gl_cPreviousFrame) / 1000;
	gl_cPreviousFrame = gl_cCurrentFrame;
}

//updates fps counter
void calcAvgFPS()
{
	framesSinceLastSecond++;
	secondCount += gl_dSecondsElapsed;

	if (secondCount >= 1.0)
	{
		secondCount = 0;
		framesPerSecond = framesSinceLastSecond;
		framesSinceLastSecond = 0;

		//updates fps counter text
		fpsWidget->setString(std::to_string(framesPerSecond) + " fps");
	}
}

/*/updates the center values of the window
void updateCenter()
{
	gl_iCenter[0] = rEngine.getMainWindow()->getWindowWidthRef() * 0.5;
	gl_iCenter[1] = rEngine.getMainWindow()->getWindowHeightRef() * 0.5;
}*/

//drawing square/rectangle to window
void drawScreenQuad()
{
	if (quadVao == 0)
	{
		GLfloat quadVertices[] = {
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f
		};
		glGenVertexArrays(1, &quadVao);
		glBindVertexArray(quadVao);
		glGenBuffers(1, &quadVbo);
		glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	}
	glBindVertexArray(quadVao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
/*
//draws a line on screen
void drawScreenLine()
{
	if (lineVao == 0)
	{
		GLfloat lineVertices[] = {
			-1.0f, 1.0f, 
			1.0f, 1.0f
		};
		glGenVertexArrays(1, &lineVao);
		glBindVertexArray(lineVao);
		glGenBuffers(1, &lineVbo);
		glBindBuffer(GL_ARRAY_BUFFER, lineVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), &lineVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (GLvoid*)0);
	}
	glBindVertexArray(lineVao);
	glLineWidth(3.0);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}*/

//draws string to window, left-aligned. function used for text class
void drawString(int fontIndex, std::string &string, glm::dvec2 cursor, float size = 64.0f, glm::vec4 colour = glm::vec4(1.0f))
{
	Font *selectFont = fonts.data(fontIndex);

	//change cursor to topleft corner
	cursor.y -= selectFont->getLineHeight() * size * rEngine.getMainWindow()->getSPixelToWindowRef().y;

	glEnable(GL_BLEND);
	rEngine.useShader(std::string("textRendering"));
	glActiveTexture(GL_TEXTURE0);
	selectFont->bindAtlas();
	glUniform1i(txtBit, 0);
	glUniform1f(txtWS, size);
	glUniform4fv(txtColour, 1, glm::value_ptr(colour));

	for (char c : string) //draws every character
	{
		FontChar character = selectFont->m_characters.at(c);

		glUniform2f(txtCursor, cursor.x, cursor.y);
		glUniform2f(txtOffsets, character.m_offset.x, character.m_offset.y);
		glUniform2f(txtAR, character.m_dimensions.x, character.m_dimensions.y);
		glUniform2f(txtUnitScreenRatio, rEngine.getMainWindow()->getSPixelToWindowRef().x, rEngine.getMainWindow()->getSPixelToWindowRef().y);
		glUniform4f(txtTLBR, character.m_texcoords.tl.x, character.m_texcoords.tl.y, character.m_texcoords.br.x, character.m_texcoords.br.y);

		drawScreenQuad();

		cursor.x += character.m_advance * size * rEngine.getMainWindow()->getSPixelToWindowRef().x;
	}

	glDisable(GL_BLEND);
}

//draws string in a box left-aligned, once right edge of line is hit, next character will be drawn on a new line
void drawStringInBox(int fontIndex, std::string& string, glm::dvec2 cursor, double width, float size = 64.0f, glm::vec4 colour = glm::vec4(1.0f))
{
	Font *selectFont = fonts.data(fontIndex);

	glm::vec2 lineT = cursor;

	//change  to left = 1, right = -1
	//change cursor to topleft corner
	cursor.y -= selectFont->getLineHeight() * size * rEngine.getMainWindow()->getSPixelToWindowRef().y;

	double lineR = lineT.x + width;
	double height = size * rEngine.getMainWindow()->getSPixelToWindowRef().y;

	glEnable(GL_BLEND);
	rEngine.useShader(std::string("textRendering"));
	glActiveTexture(GL_TEXTURE0);
	selectFont->bindAtlas();
	glUniform1i(txtBit, 0);
	glUniform1f(txtWS, size);
	glUniform4fv(txtColour, 1, glm::value_ptr(colour));

	for (char c : string) //draws every character
	{
		FontChar character = selectFont->m_characters.at(c);

		glUniform2f(txtCursor, cursor.x, cursor.y);
		glUniform2f(txtOffsets, character.m_offset.x, character.m_offset.y);
		glUniform2f(txtAR, character.m_dimensions.x, character.m_dimensions.y);
		glUniform2f(txtUnitScreenRatio, rEngine.getMainWindow()->getSPixelToWindowRef().x, rEngine.getMainWindow()->getSPixelToWindowRef().y);
		glUniform4f(txtTLBR, character.m_texcoords.tl.x, character.m_texcoords.tl.y, character.m_texcoords.br.x, character.m_texcoords.br.y);

		drawScreenQuad();

		cursor.x += character.m_advance * size * rEngine.getMainWindow()->getSPixelToWindowRef().x;
		if (cursor.x > lineR)
		{
			if (c == ' ')
			{
				cursor.x = lineT.x;
				cursor.y -= height;
			}
		}
	}

	glDisable(GL_BLEND);
}

//draws image to screen
void drawTexture(int textureIndex, glm::dvec2 topLeft, glm::dvec2 bottomRight, glm::vec4 tintColour = glm::vec4(1.0f))
{
	glm::vec2 dimensions = glm::vec2(bottomRight.x - topLeft.x, topLeft.y - bottomRight.y);

	glEnable(GL_BLEND);
	rEngine.useShader(std::string("widgetRenderPass"));
	glActiveTexture(GL_TEXTURE0);
	textures.bind(textureIndex);
	glUniform1i(wgtBit, 0);
	glUniform2f(wgtDimensions, dimensions.x, dimensions.y);
	glUniform2f(wgtBoxTopLeft, topLeft.x, topLeft.y);
	glUniform4f(wgtTintColour, tintColour.x, tintColour.y, tintColour.z, tintColour.w);

	drawScreenQuad();

	glDisable(GL_BLEND);
}

//updates everything when the active map changes
void playerAreaChange(int index)
{
	glm::vec2 playerPos = currentPlayer->getPos();
	glm::vec2 camRelative = activeCam->getPos() - playerPos;
	FourPoints currentArea = maps.getTPArea(activePlayground->getName(), maps.getName(index));
	FourPoints targetArea = maps.getTPArea(maps.getName(index), activePlayground->getName());

	//player position relative to "teleport box"
	float magCurrent = sqrt(pow(currentArea.tr.x - currentArea.bl.x, 2) + pow(currentArea.tr.y - currentArea.bl.y, 2));
	float magTarget = sqrt(pow(targetArea.tr.x - targetArea.bl.x, 2) + pow(targetArea.tr.y - targetArea.bl.y, 2));
	float ratio = 1.0f;
	if (magCurrent != 0)
	{
		ratio = magTarget / magCurrent;
	}
	glm::vec2 blDiff = playerPos - currentArea.bl;
	glm::vec2 trDiff = currentArea.tr - playerPos;

	//puts player in a relative position in the connected teleport box in the new active map
	if (trDiff.x <= blDiff.x)
	{
		if (trDiff.x <= trDiff.y && trDiff.x <= blDiff.y) //right closer, exit left
		{
			currentPlayer->setPos(glm::vec2(targetArea.bl.x, targetArea.bl.y + (blDiff.y * ratio)));
		}
		else if (trDiff.y <= blDiff.y) //top closer, exit bottom
		{
			currentPlayer->setPos(glm::vec2(targetArea.bl.x + (blDiff.x * ratio), targetArea.bl.y));
		}
		else //bottom closer, exit top
		{
			currentPlayer->setPos(glm::vec2(targetArea.bl.x + (blDiff.x * ratio), targetArea.tr.y));
		}
	}
	else
	{
		if (blDiff.x <= trDiff.y && blDiff.x <= blDiff.y) //left closer, exit right
		{
			currentPlayer->setPos(glm::vec2(targetArea.tr.x, targetArea.bl.y + (blDiff.y * ratio)));
		}
		else if (trDiff.y <= blDiff.y) //top closer, exit bottom
		{
			currentPlayer->setPos(glm::vec2(targetArea.bl.x + (blDiff.x * ratio), targetArea.bl.y));
		}
		else //bottom closer, exit top
		{
			currentPlayer->setPos(glm::vec2(targetArea.bl.x + (blDiff.x * ratio), targetArea.tr.y));
		}
	}

	//puts camera in the correct relative position to the player in the new map
	activeCam->setPos(currentPlayer->getPos() + camRelative);
}

//activates a map change
void setActivePlayground(int index)
{
	//change player position
	if (activePlayground != nullptr && currentPlayer != nullptr && activeCam != nullptr)
	{
		//playerAreaChange(index);
	}
	maps.useMap(index);

	activePlayground = maps.data(index); //global playground pointer
	pEngine.mapChange(maps.data(index)); //physics engine playground pointer
	maps.data(index)->getCollisionData()->updateWindowAR(rEngine.getMainWindow()->getWindowArRef()); //update playground collision map's window aspect ratio
	activeCam->changeClampValues(maps.getMinCamWP(index), maps.getMaxCamWP(index)); //updates camera's clamping vlaues;
}

//player change
void setPlayer()
{
	currentPlayer->stopPlayer();
	currentPlayer = newTargetPlayer;
	currentPlayer->usePlayer();
}

//window aspect ratio change
void windowAspectChange()
{
	activePlayground->getCollisionData()->updateWindowAR(rEngine.getMainWindow()->getWindowArRef()); //update playground collision map's window aspect ratio
	activePlayground->calcCamClampValues(rEngine.getMainWindow()->getPixelToWindowRef()); //updates camera clamp values
	activeCam->updateWindowDimensions(); //updates camera's window dimensions
	activeCam->changeClampValues(maps.getMinCamWP(activePlaygroundIndex), maps.getMaxCamWP(activePlaygroundIndex)); //updates clamp values in camera class

	rEngine.getMainWindow()->getScreenArRef() = (float)rEngine.getMainWindow()->getScreenWidthRef() / (float)rEngine.getMainWindow()->getScreenHeightRef();
	rEngine.getMainWindow()->getWindowArRef() = (float)rEngine.getMainWindow()->getWindowWidthRef() / (float)rEngine.getMainWindow()->getWindowHeightRef();
	rEngine.getMainWindow()->getPixelToWindowRef().x = 64.0f / (float)rEngine.getMainWindow()->getWindowWidthRef();
	rEngine.getMainWindow()->getPixelToWindowRef().y = 64.0f / (float)rEngine.getMainWindow()->getWindowHeightRef();
	rEngine.getMainWindow()->getSPixelToWindowRef() = rEngine.getMainWindow()->getPixelToWindowRef() / 64.0f;

	//recreates the framebuffer with new window dimensions
	delete gl_tBuffer;
	gl_tBuffer = new TBuffer(rEngine.getMainWindow()->getWindowWidthRef() * rEngine.getMainWindow()->getParallaxToWindowRef(), rEngine.getMainWindow()->getWindowHeightRef() * rEngine.getMainWindow()->getParallaxToWindowRef());
}


//resizes the window, will add that in someday
void resizeWindow(int w, int h)
{

}

//sets up the shaders required for the game
void shaderSetup()
{
	std::string tempName;
	/*
	
	---Parallax---
	
	*/
	//loads parallax shaders
	tempName = "parallaxPass";
	rEngine.addShader(Shader("parallaxPass", "../../Shaders/Tiled/Ctrl 2k parallaxPass.vert", "../../Shaders/Tiled/Ctrl 2k parallaxPass.frag"));

	parallaxAlbedo = glGetUniformLocation(rEngine.getShader(tempName)->getShaderProgram(), "albedoSampler");
	parallaxDepth = glGetUniformLocation(rEngine.getShader(tempName)->getShaderProgram(), "depthSampler");
	parallaxNormal = glGetUniformLocation(rEngine.getShader(tempName)->getShaderProgram(), "normalSampler");
	parallaxBool = glGetUniformLocation(rEngine.getShader(tempName)->getShaderProgram(), "parallax");
	parallaxScale = glGetUniformLocation(rEngine.getShader(tempName)->getShaderProgram(), "scale");
	parallaxWeight = glGetUniformLocation(rEngine.getShader(tempName)->getShaderProgram(), "parallaxWeight");

	/*
	
	---Text---
	
	*/
	//loads text render shaders
	tempName = "textRendering";

	txtCursor = glGetUniformLocation(rEngine.getShader(tempName)->getShaderProgram(), "cursor");
	txtOffsets = glGetUniformLocation(rEngine.getShader(tempName)->getShaderProgram(), "offsets");
	txtWS = glGetUniformLocation(rEngine.getShader(tempName)->getShaderProgram(), "worldSize");
	txtUnitScreenRatio = glGetUniformLocation(rEngine.getShader(tempName)->getShaderProgram(), "unitToScreen");
	txtAR = glGetUniformLocation(rEngine.getShader(tempName)->getShaderProgram(), "aspectRatio");
	txtTLBR = glGetUniformLocation(rEngine.getShader(tempName)->getShaderProgram(), "tlbr");
	txtBit = glGetUniformLocation(rEngine.getShader(tempName)->getShaderProgram(), "bitSampler");
	txtColour = glGetUniformLocation(rEngine.getShader(tempName)->getShaderProgram(), "textColour");

	/*

	---Widget---

	*/
	//loads widget render shaders
	tempName = "widgetRenderPass";
	rEngine.addShader(Shader("widgetRenderPass", "../../Shaders/Tiled/Ctrl 2k widgetRender.vert", "../../Shaders/Tiled/Ctrl 2k widgetRender.frag"));

	wgtDimensions = glGetUniformLocation(rEngine.getShader(tempName)->getShaderProgram(), "dimensions");
	wgtBoxTopLeft = glGetUniformLocation(rEngine.getShader(tempName)->getShaderProgram(), "boxTopLeft");
	wgtBit = glGetUniformLocation(rEngine.getShader(tempName)->getShaderProgram(), "bitSampler");
	wgtTintColour = glGetUniformLocation(rEngine.getShader(tempName)->getShaderProgram(), "tintColour");
}

//the loop which runs all loops
void gameLoop()
{
	bool game = true;
	//music->loadFile();
	sEngine.playSoundTrack(std::string("Running in The 90s"));
	while (game)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event)) //input
		{
			switch (event.type)
			{
			case SDL_QUIT: //close window
				game = false;
				break;
			case SDL_KEYDOWN: //call keyhandler
				//keyboardHandle(event);
				gameEngine.keyHandle(event);
				break;
			case SDL_KEYUP: //call keyuphandler
				//keyboardUp(event);
				gameEngine.keyUpHandle(event);
				break;
			case SDL_MOUSEMOTION: //call mousemotionhandler
				gameEngine.mouseMotionHandle(event);
				break;
			case SDL_MOUSEBUTTONDOWN: //call mousehandler
				gameEngine.mouseHandle(event);
				break;
			case SDL_MOUSEBUTTONUP: //call mouseuphandler
				gameEngine.mouseUpHandle(event);
				break;
			}
		}

		gameEngine.idle();
		gameEngine.displayUpdate();
	}
}
 
//clean up function at end of main()
void cleanUp()
{
	delete gl_tBuffer;
	gl_tBuffer = nullptr;
}

int main(int argc, char *argv[])
{
	Parser temp(maps, textures, fonts, animations);
	std::string name; //for game configs
	std::vector<int> data;
	temp.parseConfigs(std::string("../../Resource/System/Graphics.config"), name, data);
	rEngine.getMainWindow()->setConfig(name, data);

	rEngine.init();
	pEngine.mapChange(activePlayground);

	//activates maps and textures
	maps.reserve(1);
	textures.reserve(3);

	//creates buffers
	gl_tBuffer = new TBuffer(rEngine.getMainWindow()->getWindowWidthRef() * rEngine.getMainWindow()->getParallaxToWindowRef(), rEngine.getMainWindow()->getWindowHeightRef() * rEngine.getMainWindow()->getParallaxToWindowRef());

	//activate camera
	activeCam = rEngine.getActiveCam();

	shaderSetup();

	//debug all temporary code....
	sounds.addMusic(std::string("Running in The 90s"), std::string("../../Resource/Audio/Initial D - Running in The 90s.wav"));
	sounds.addSFX(std::string("beat"), std::string("../../Resource/Audio/beat.wav"));
	victor = temp.parseDotAnim(std::string("../../Resource/Animations/victor_walk.anim"));
	texture = temp.parseDotAnim(std::string("../../Resource/Animations/turret_rest.anim"));
	turretDeathTexture = temp.parseDotAnim(std::string("../../Resource/Animations/turret_death.anim"));
	int fireFX = temp.parseDotAnim(std::string("../../Resource/Animations/FireEffect.anim"));
	int swordFX = temp.parseDotAnim(std::string("../../Resource/Animations/sword_flash.anim"));
	ballTexture = temp.parseDotAnim(std::string("../../Resource/Animations/ball.anim"));
	int vicIdle = temp.parseDotAnim(std::string("../../Resource/Animations/victor_idle.anim"));
	gameFont = temp.parseDotFNT(std::string("../../Resource/System/Fonts/visitor/visitor.fnt"));
	//temp.parseDotFNT(std::string("Resource/System/Fonts/quantum/quantum.fnt"));
	//temp.parseDotFNT(std::string("Resource/System/Fonts/perfect_dark/perfect_dark.fnt"));
	activePlaygroundIndex = temp.parseDotMP(std::string("../../Resource/Map/test.mp"));
	temp.parseDotMP(std::string("../../Resource/Map/test1.mp"));
	healthBar = new StatusBar(std::string("health bar"), glm::dvec2(-1.0, 0.99), glm::dvec2(-0.25, 0.9011111), textures.create(std::string("woodenbar"), std::string("../../Resource/Texture/Widgets/woodenBarBackground.png")),
							  glm::dvec2(-0.95703125, 0.6875), glm::dvec2(0.95703125, -0.6875), textures.create(std::string("barFiller"), std::string("../../Resource/Texture/Widgets/barFiller.png")));
	healthBar->setColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	staminaBar = new StatusBar(std::string("stamina bar"), glm::dvec2(-1.0, 0.901111), glm::dvec2(-0.25, 0.8122222), textures.search(std::string("woodenbar")),
		glm::dvec2(-0.95703125, 0.6875), glm::dvec2(0.95703125, -0.6875), textures.search(std::string("barFiller")));
	staminaBar->setColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	manaBar = new StatusBar(std::string("mana bar"), glm::dvec2(-1.0, 0.8122222), glm::dvec2(-0.25, 0.7233333), textures.search(std::string("woodenbar")),
		glm::dvec2(-0.95703125, 0.6875), glm::dvec2(0.95703125, -0.6875), textures.search(std::string("barFiller")));
	manaBar->setColour(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	//widgets
	headsUpDisplay = new HUD();
	fpsWidget = new Text(std::string("fps counter"), fonts, rEngine.getMainWindow()->getSPixelToWindowRef(), 0, std::string("0 fps"), glm::dvec2(0.5, 1.0), glm::dvec2(1.0, 0.5), 2, 64, glm::vec4(0.2f, 0.75f, 0.3f, 0.9f));
	loremIpsum = new Text(std::string("lorem ipsum"), fonts, rEngine.getMainWindow()->getSPixelToWindowRef(), 0, std::string("Lorem ipsum dolor sit amet, consecteur adipiscing elit. Praesent imperdiet nibh ac lorem commodo, in ornare mi accumsan.Cras pharetra, justo eu porta condimentum, nunc dui viverra arcu, sed posuere."),
		glm::dvec2(-0.5, 1.0), glm::dvec2(0.5, 0.0), 1, 64, glm::vec4(1.0f, 1.0f, 1.0f, 0.75f));
	game.addObj(std::string("widgets"), headsUpDisplay); //game linked
	//headsUpDisplay->addChild(&loremIpsum);
	headsUpDisplay->activate(fpsWidget, healthBar, staminaBar, manaBar);

	inputController.changeKeyBinding('w', std::string("MOVE_UP"), true);
	inputController.changeKeyBinding('a', std::string("MOVE_LEFT"), true);
	inputController.changeKeyBinding('s', std::string("MOVE_DOWN"), true);
	inputController.changeKeyBinding('d', std::string("MOVE_RIGHT"), true);
	inputController.changeKeyBinding(SDL_BUTTON_LEFT, std::string("SWING"));

	//adds map connectors
	FourPoints rectangleOne {glm::vec2(-30.0f, 3.0f), glm::vec2(-29.5f, 3.0f), glm::vec2(-30.0f, -2.0f), glm::vec2(-29.5f, -2.0)};
	FourPoints rectangleTwo {glm::vec2(-30.5f, 3.0f), glm::vec2(-30.0f, 3.0f), glm::vec2(-30.5f, -2.0f), glm::vec2(-30.0f, -2.0)};
	//maps.addConnector(std::string("test"), std::string("test1"), rectangleTwo, rectangleOne);
	maps.data(std::string("test"))->addComponent(new Connector(rectangleTwo, std::string("test1"), rectangleOne), true); //tpOne 3
	maps.data(std::string("test1"))->addComponent(new Connector(rectangleOne, std::string("test"), rectangleTwo), true); //tpZero 3
	maps.updateAllAdjMapIndices();
	rectangleOne.tl = glm::vec2(0.0f);
	rectangleOne.tr = glm::vec2(0.0f);
	rectangleOne.bl = glm::vec2(0.0f);
	rectangleOne.br = glm::vec2(0.0f);
	rectangleTwo.tl = glm::vec2(0.0f);
	rectangleTwo.tr = glm::vec2(0.0f);
	rectangleTwo.bl = glm::vec2(0.0f);
	rectangleTwo.br = glm::vec2(0.0f);

	gameEngine.setMap(activePlaygroundIndex); //sets the map

	//player
	Statistics playerStats{ 100, 2.0, 3.5, 6.0, 100, 0.05, 0.5, 100, 0.75, 100, 20, 0.5, 4.0, 8.0, 14.0, 0.01, 15, 1.5, 1.0, 1.0, 1.1, 2, 1, 0.8f, 0.15f, 0.3f };
	currentPlayer = new Player(gameEngine.getTimeRef(), std::string("Victor"), new RectangleHB(glm::vec2(0.0f), glm::vec2(1.0f), 15.0f));
	/*currentPlayer = new Player(dmgAreas, gl_dSecondsElapsed, std::string("Victor"), playerStats, std::vector<int>{victor, victor, victor}, glm::vec2(0.0), 
		FourPoints{glm::vec2(-0.5f, 0.5f), glm::vec2(0.5f, 0.5f), glm::vec2(-0.5f, -0.5f), glm::vec2(0.5f, -0.5f)}, 10.0f, 0.96f, 0.5f, 1.0f);*/
	currentPlayer->usePlayer();
	newTargetPlayer = currentPlayer;
	Entity* shootingThing = new NonMovingShooter(ballTexture, gameEngine.getTimeRef(), std::string("Turret3"),
		new RectangleHB(glm::vec2(0.0f), glm::vec2(1.6f), 45.0f), glm::vec2(0.0f, 10.0f), AGGRESSIVE, 0.0f, 0.0f, 0.0f, 15.0f, LEFT);
	shootingThing->addComponent(new VisualData(TEXTURE, texture, glm::vec2(1.0f, 1.0f)), true); //idle 6
	shootingThing->addComponent(new VisualData(ANIMATION, turretDeathTexture, glm::vec2(1.0f))); //death 7
	game.addObj(std::string("entity"), shootingThing);
	/*entities.push_back(new NonMovingShooter(ballTexture, dmgAreas, gl_dSecondsElapsed, std::string("Turret3"), playerStats, std::vector<int>{texture, texture, texture}, glm::vec2(0.0f, 10.0f), 
		FourPoints{glm::vec2(-0.8f, 0.8f), glm::vec2(0.8f, 0.8f), glm::vec2(-0.8f, -0.8f), glm::vec2(0.8f, -0.8f) }, 9999.99f, 0.96f, 1.0f, 1.0f, 15.0f, LEFT));*/
	//currentPlayer->addArmour(100); //gives player 100 armour
	activeCam->setTarget(currentPlayer);
	headsUpDisplay->setTarget(currentPlayer);
	currentPlayer->addComponent(new VisualData(ANIMATION, vicIdle, glm::vec2(0.5f, 1.0f)), true); //idle 6
	currentPlayer->addComponent(new VisualData(ANIMATION, vicIdle, glm::vec2(0.5f, 1.0f))); //death 7
	currentPlayer->addComponent(new VisualData(ANIMATION, victor, glm::vec2(0.5f, 1.0f))); //walking 8
	game.addObj(std::string("entity"), currentPlayer);
	game.setPlayer(currentPlayer);
	/*
	Spawner spTest;
	glm::vec2 printTest;
	for (int i = 0; i < 20; i++)
	{
		printTest = spTest.randomDirection();
		std::cout << printTest.x << ' ' << printTest.y << '\n';
	}*/

	SaveState save;
	save.readSave();
	gameEngine.loadSaveState(&save);
	gameLoop();
	gameEngine.saveGame();
	cleanUp();
	//game.debugSetup();
	//gameLoop();
	return 0;
}