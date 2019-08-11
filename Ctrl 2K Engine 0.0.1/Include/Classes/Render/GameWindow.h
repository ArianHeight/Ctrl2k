#pragma once

/*

SDL rendering context or window as it is known as to the majority of the population

*/

class Parser; //for initiating

class GameWindow
{
public:
	GameWindow(int width = 1280, int height = 720, std::string name = "");
	~GameWindow();

private:
	bool active = false;
	int m_vSync{ 0 }; //0 is false
	std::string m_name{""}; //name

	//sdl window setup
	SDL_Window* m_sdlWindow; //remember to delete
	SDL_GLContext m_glContext; //openGL context for sdl

	//dimensions
	int m_windowWidth{ 1280 }; //window dimensions
	int m_windowHeight{ 720 };
	int m_windowResX{ 1280 }; //window resolution
	int m_windowResY{ 720 }; 
	glm::vec2 m_resToWin; //res/win
	float m_windowAspectRatio;
	int m_screenWidth; //screen dimension values
	int m_screenHeight;
	float m_screenAspectRatio;

	//aspect ratio/pixel ratio
	glm::vec2 m_pixelToWindow;
	glm::vec2 m_singlePxToWindow;
	float m_parallaxToWindow{ 1.01f };

	//positions
	glm::ivec2 m_center = glm::ivec2(0); //center point of the window in world space
	//GLuint m_windowHandle; //window object
	glm::ivec2 m_mousePos;

	//private methods
	void initSDL(); //initializes sdl
	void initOGL(); //initializes OGL
	void initGLEW(); //initalizes GLEW
	void updateCenter(); //width & height / 2
	void updateRTW(); //updates res to window
public:
	void setConfig(std::string &name, std::vector<int> &configData);
	void init(); //initializes the game window
	void swapBuffers(); //updates the screen
	void changeWindowSizeTo(int winX, int winY); //changes the size of window but not resolution!!! This is a method only meant to be called from renderEngine!!! is meant to be followed with changeWindowRes()
	void changeWindowResTo(int resX, int resY); //changes the resolution of window but not size!!! This is a method only meant to be called from renderEngine!!!
	void changeWindowRes(); //changes resoluton according to ratio

	SDL_Window* getRenderingContext(); //returns the window context
	SDL_GLContext getRenderingContextGL(); //returns OGL context
	glm::vec2& getPixelToWindowRef();
	glm::vec2& getSPixelToWindowRef();
	int& getWindowWidthRef();
	int& getWindowHeightRef();
	int& getScreenWidthRef();
	int& getScreenHeightRef();
	int& getResolutionXRef();
	int& getResolutionYRef();
	float& getParallaxToWindowRef();
	float& getWindowArRef();
	float& getScreenArRef();
	bool isActive();
};

/*

end game window

*/