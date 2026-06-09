#include <Ctrl 2kPCH.h>
#include "Core/GreatBath/Logger.h"

/*

a game window

*/

GameWindow::GameWindow(int width, int height, std::string name) :
	m_windowWidth(width), m_windowHeight(height), m_name(name), m_windowResX(width), m_windowResY(height)
{
	//cstr
}

void GameWindow::setConfig(std::string &name, std::vector<int> &configData)
{
	//cstr
	this->m_name = name;
	if (configData.size() >= 6)
	{
		this->m_vSync = configData[1];
		this->m_windowWidth = configData[2];
		this->m_windowHeight = configData[3];
		this->m_windowResX = configData[4];
		this->m_windowResY = configData[5];
	}
}

//intializes the game window
void GameWindow::init()
{
	this->active = true;

	this->initSDL(); //window
	this->initOGL(); //ogl render context
	this->initGLEW(); //gl extension wrangler
}

void GameWindow::swapBuffers()
{
	SDL_GL_SwapWindow(this->m_sdlWindow);
}

void GameWindow::initSDL()
{
	LOG_MSG_QUEUE("Initializing SDL...");

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) //inits sdl
	{
		//did not work
		LOG_FATAL_PUSH("Falied to initialize SDL 2, Exiting...");
		exit(-1);
	}

	this->m_sdlWindow = SDL_CreateWindow(this->m_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->m_windowWidth, this->m_windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (this->m_sdlWindow == nullptr) //check if sucessful
	{
		LOG_FATAL_PUSH("Failed to create SDL2 window, Exiting...");
		exit(-1);
	}
	
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) //check if sdl_mixer has been initialized successfully
	{
		LOG_FATAL_PUSH("Failed to initialize SDL_mixer, Exiting...");
		exit(-1);
	}
	Mix_Volume(-1, MIX_MAX_VOLUME / 4);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 4);

	//sets gl render attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); //context 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //double buffering

	this->m_glContext = SDL_GL_CreateContext(this->m_sdlWindow);

	//disables v-sync
	SDL_GL_SetSwapInterval(this->m_vSync);

	LOG_MSG_PUSH("SDL Has Been Initalized Successfully");
}

void GameWindow::initOGL()
{
	LOG_MSG_QUEUE("Initalizing OpenGL...");

	//Window Stuffs
	SDL_DisplayMode sdlDeskTop; //for dimensions
	SDL_GetDesktopDisplayMode(0, &sdlDeskTop);

	//screen dimensions + ar
	this->m_screenWidth = sdlDeskTop.w;
	this->m_screenHeight = sdlDeskTop.h;
	this->m_screenAspectRatio = (float)this->m_screenWidth / (float)this->m_screenHeight;
	//window ar
	this->m_windowAspectRatio = (float)this->m_windowWidth / (float)this->m_windowHeight;
	this->m_pixelToWindow = glm::vec2(64.0f / (float)this->m_windowResX, 64.0f / (float)this->m_windowResY); //64 pixels on screen
	this->m_singlePxToWindow = this->m_pixelToWindow / 64.0f;

	this->updateCenter();

	//Other Setups
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	LOG_MSG_PUSH("OpenGL Has Initialized Successfully");
}

void GameWindow::initGLEW() //for vertex array and buffer objects
{
	LOG_MSG_QUEUE("Initalizing GLEW...");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) //not initialized properly
	{
		LOG_FATAL_PUSH("There was a problem initializing GL Extension Wrangler, Exiting...");
		exit(-1);
	}
	if (!GLEW_VERSION_3_3) //wrong opengl vesion support
	{
		LOG_FATAL_PUSH("OpenGL 3.3 Version Support Not Present For Engine Use...");
		exit(-1);
	}
#ifdef _WIN32
	if (WGLEW_EXT_swap_control) //v sync on windows 32
	{
		wglSwapIntervalEXT(this->m_vSync); //disables Vertical-Sync
	}
#endif

	LOG_MSG_PUSH("GLEW Has Been Initalized Successfully");
}

void GameWindow::updateCenter()
{
	this->m_center.x = this->m_windowWidth * 0.5;
	this->m_center.y = this->m_windowHeight * 0.5;
}

void GameWindow::updateRTW()
{
	this->m_resToWin.x = (float)this->m_windowResX / (float)this->m_windowWidth;
	this->m_resToWin.y = (float)this->m_windowResY / (float)this->m_windowHeight;
}

void GameWindow::changeWindowSizeTo(int winx, int winy)
{
	this->m_windowWidth = winx;
	this->m_windowHeight = winy;
	this->m_windowAspectRatio = (float)this->m_windowWidth / (float)this->m_windowHeight;

	//NOTE: need to check if dis works
	SDL_SetWindowSize(m_sdlWindow, m_windowWidth, m_windowHeight);
	swapBuffers();
}

void GameWindow::changeWindowResTo(int resx, int resy)
{
	this->m_windowResX = resx;
	this->m_windowResY = resy;

	this->updateRTW();
}

void GameWindow::changeWindowRes()
{
	this->m_windowResX = this->m_windowWidth * this->m_resToWin.x;
	this->m_windowResY = this->m_windowHeight * this->m_resToWin.y;
}

//accessors
SDL_Window* GameWindow::getRenderingContext()
{
	return this->m_sdlWindow;
}

SDL_GLContext GameWindow::getRenderingContextGL()
{
	return this->m_glContext;
}

glm::vec2& GameWindow::getPixelToWindowRef()
{
	return this->m_pixelToWindow;
}

glm::vec2& GameWindow::getSPixelToWindowRef()
{
	return this->m_singlePxToWindow;
}

int& GameWindow::getWindowWidthRef()
{
	return this->m_windowWidth;
}

int& GameWindow::getWindowHeightRef()
{
	return this->m_windowHeight;
}

int& GameWindow::getScreenWidthRef()
{
	return this->m_screenWidth;
}

int& GameWindow::getScreenHeightRef()
{
	return this->m_screenHeight;
}

int& GameWindow::getResolutionXRef()
{
	return this->m_windowResX;
}

int& GameWindow::getResolutionYRef()
{
	return this->m_windowResY;
}

float& GameWindow::getParallaxToWindowRef()
{
	return this->m_parallaxToWindow;
}

float& GameWindow::getWindowArRef()
{
	return this->m_windowAspectRatio;
}

float& GameWindow::getScreenArRef()
{
	return this->m_screenAspectRatio;
}

bool GameWindow::isActive()
{
	return this->active;
}

GameWindow::~GameWindow()
{
	//clean up
	if (this->m_sdlWindow != nullptr)
	{
		SDL_GL_DeleteContext(this->m_glContext);
		SDL_DestroyWindow(this->m_sdlWindow);
		SDL_Quit();
	}

	this->m_sdlWindow = nullptr;
}

/*

a game window

*/