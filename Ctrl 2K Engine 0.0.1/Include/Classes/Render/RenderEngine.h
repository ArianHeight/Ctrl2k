#pragma once

/*

rendering engine

*/

class RenderEngine
{
public:
	RenderEngine(CoreEngine &engine);
	RenderEngine(CoreEngine &engine, int w, int h, std::string &s);
	~RenderEngine();

private:
	//rendering vaos
	//for drawing squares, Vertex Array Objects and Vertex Buffer Objects
	GLuint quadVao = 0;
	GLuint quadVbo;
	//for drawing lines
	GLuint lineVao = 0;
	GLuint lineVbo;

	CoreEngine &parentEngine;
	GameWindow mainWindow;
	ScreenBuffer *sBuffer; //needs to be recreated if res is changed
	Camera activeCam;
	std::vector<Shader> shaderPrograms;
	std::vector<Component*> rendererBuffer;
	//Camera activeCam;

	void activateWindow(); //these must be run before any rendering can be done!!
	void createQuadVao();
	void createLineVao();
	void createBaseShaders();

	//these are the base renders used by the engine
	void drawQuad();
	void drawLine();

	void recreateScreenBuffer(); //called only when needed to update the resolution

public:
	void init(); //does all the rendering engine initialization

	void addShader(Shader &shaderobj); //adds shader to render engine
	void useShader(std::string& name);
	void render(Component *renderComponent); //renders by reading a passed component obj
	void renderScene(GameObject* scene); //render everything in the scenegraph
	void updateScreen(); //switches the double buffer

	void resizeWindow(int newX, int newY); //updates the window size
	void changeResolution(int newX, int newY); //change the resolution
	//void resetCameraClamp(); //called when clamp values need to be updated - either after map change or a res change

	void drawTexture(int textureIndex, const glm::vec2& pos, const glm::vec2& aspectRatio, float size, float depth = 0.0f); //draws a texture into world space
	void drawTextureN(int textureIndex, glm::vec2& topLeft, glm::vec2& bottomRight, float depth = 0.0f); //draw texture with normalized coordinates in screen space(0 - 1)
	void drawAnimation(int animationIndex, double &animTime, const glm::vec2& pos, float depth = 0.0f, Orientation dir = UP); //draws an animation into world space
	void drawLine(glm::vec2& wp, glm::vec2& wpt, glm::vec3& colour); //draws a line into world space
	void drawString(int fontIndex, std::string &string, glm::vec2 cursor, float size = 64.0f, glm::vec4 colour = glm::vec4(1.0f)); //draws a string onto the screen, left-aligned
	void drawStringInBox(int fontIndex, std::string& string, glm::vec2 cursor, float width, float size = 64.0f, glm::vec4 colour = glm::vec4(1.0f)); //draws a string in a box, left-aligned 

	void drawWidget(Widget *drawTarget); //draws widgets to screen
	void drawGUI(GUI *gui); //draws a GUI to screen
	void drawMap(int mapIndex); //draws the map/background
	void drawAnimation(Entity *drawTarget); //draws animation to window of entity class
	void drawAnimation(DamageArea *drawTarget); //draws animation to window off damagearea class
	void drawLine(DamageArea* a); //draws line to window off damage area class
	void drawText(Text *t); //draws a Text obj to screen
	void drawParticle(Particle *p); //draws a single particle
	void drawParticleFX(ParticleEffect *pfx); //draws a particle effect

	GameWindow* getMainWindow();
	Shader* getShader(std::string& name);
	Camera* getActiveCam();
};

/*

rendering engine

*/