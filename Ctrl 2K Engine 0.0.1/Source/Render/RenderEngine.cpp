#include <Ctrl 2kPCH.h>

/*

rendering engine

*/

RenderEngine::RenderEngine(CoreEngine &engine) :
	parentEngine(engine), mainWindow(GameWindow()), 
	activeCam(*this, "Camera", glm::vec2(0.0f), engine.getTimeRef(), this->mainWindow.getWindowWidthRef(), this->mainWindow.getWindowHeightRef())
{
	//cstr
}

RenderEngine::RenderEngine(CoreEngine &engine, int w, int h, std::string &s) :
	parentEngine(engine), mainWindow(GameWindow(w, h, s)),  
	activeCam(*this, "Camera", glm::vec2(0.0f), engine.getTimeRef(), this->mainWindow.getWindowWidthRef(), this->mainWindow.getWindowHeightRef())
{
	//cstr
}

void RenderEngine::init()
{
	std::cout << "Initializing Rendering Engine..." << std::endl;
	this->activateWindow();
	this->createQuadVao();
	this->createLineVao();
	this->createBaseShaders();

	if (!this->mainWindow.isActive() || this->quadVao == 0 || this->lineVao == 0)
	{
		std::cerr << "The Render Engine Was Not Initialized Properly..." << std::endl;
		exit(-1);
	}

	std::cout << "Rendering Engine Has Been Initialized Successfully" << std::endl;

	this->recreateScreenBuffer();
}

/*

setup stuff

*/

void RenderEngine::activateWindow()
{
	this->mainWindow.init();
}

void RenderEngine::createQuadVao()
{
	if (this->quadVao == 0) //check if created
	{
		GLfloat quadVertices[] = {
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f
		}; //temporary location of all four vertices to range (0, 1)
		glGenVertexArrays(1, &this->quadVao);
		glBindVertexArray(this->quadVao);
		glGenBuffers(1, &this->quadVbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->quadVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	}
}

void RenderEngine::createLineVao()
{
	if (this->lineVao == 0) //check if created
	{
		GLfloat lineVertices[] = {
			-1.0f, 1.0f,
			1.0f, 1.0f
		}; //temporary location of all two vertices to range (0, 1)
		glGenVertexArrays(1, &this->lineVao);
		glBindVertexArray(this->lineVao);
		glGenBuffers(1, &this->lineVbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->lineVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), &lineVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (GLvoid*)0);
	}
}

void RenderEngine::createBaseShaders()
{
	std::string tempName;
	Shader *tempShaderPtr;

	/*

	---Texture---

	*/
	//loads texture shaders
	tempName = "textureRendering";
	this->addShader(Shader("textureRendering", "Shaders/Tiled/Ctrl 2k textureRender.vert", "Shaders/Tiled/Ctrl 2k textureRender.frag"));
	tempShaderPtr = this->getShader(tempName);
	
	//sets the vars for texture shader
	tempShaderPtr->addShaderVariable("diffuseSampler"); //0
	tempShaderPtr->addShaderVariable("worldDisplace"); //1
	tempShaderPtr->addShaderVariable("viewDisplace"); //2
	tempShaderPtr->addShaderVariable("worldSize"); //3
	tempShaderPtr->addShaderVariable("aspectRatio"); //4
	tempShaderPtr->addShaderVariable("unitToScreen"); //5

	/*

	---Animation---

	*/
	//loads animation shaders
	tempName = "animationRendering";
	this->addShader(Shader("animationRendering", "Shaders/Tiled/Ctrl 2k animationRender.vert", "Shaders/Tiled/Ctrl 2k textureRender.frag"));
	tempShaderPtr = this->getShader(tempName);

	//sets the vars for animation shader
	tempShaderPtr->addShaderVariable("diffuseSampler"); //0
	tempShaderPtr->addShaderVariable("worldDisplace"); //1
	tempShaderPtr->addShaderVariable("viewDisplace"); //2
	tempShaderPtr->addShaderVariable("worldSize"); //3
	tempShaderPtr->addShaderVariable("aspectRatio"); //4
	tempShaderPtr->addShaderVariable("unitToScreen"); //5
	tempShaderPtr->addShaderVariable("tlbr"); //6

	/*

	---Line---

	*/
	//loads line render shaders
	tempName = "lineRendering";
	this->addShader(Shader("lineRendering", "Shaders/Tiled/Ctrl 2k lineRender.vert", "Shaders/Tiled/Ctrl 2k lineRender.frag"));
	tempShaderPtr = this->getShader(tempName);

	//sets the vars for line render shader
	tempShaderPtr->addShaderVariable("worldDisplace"); //0
	tempShaderPtr->addShaderVariable("worldDisplaceTwo"); //1
	tempShaderPtr->addShaderVariable("viewDisplace"); //2
	tempShaderPtr->addShaderVariable("unitToScreen"); //3
	tempShaderPtr->addShaderVariable("in_colour"); //4

	/*

	---Text---

	*/
	//loads text render shaders
	tempName = "textRendering";
	this->addShader(Shader("textRendering", "Shaders/Tiled/Ctrl 2k textRender.vert", "Shaders/Tiled/Ctrl 2k textRender.frag"));
	tempShaderPtr = this->getShader(tempName);

	//loads vars for text render
	tempShaderPtr->addShaderVariable("bitSampler"); //0
	tempShaderPtr->addShaderVariable("worldSize"); //1
	tempShaderPtr->addShaderVariable("textColour"); //2
	tempShaderPtr->addShaderVariable("cursor"); //3
	tempShaderPtr->addShaderVariable("offsets"); //4
	tempShaderPtr->addShaderVariable("aspectRatio"); //5
	tempShaderPtr->addShaderVariable("unitToScreen"); //6
	tempShaderPtr->addShaderVariable("tlbr"); //7

	/*

	---Normalizied Texture---

	*/
	//loads texture normalized render shaders
	tempName = "textureNRendering";
	this->addShader(Shader("textureNRendering", "Shaders/Tiled/Ctrl 2k textureNRender.vert", "Shaders/Tiled/Ctrl 2k textureNRender.frag"));
	tempShaderPtr = this->getShader(tempName);

	//sets the vars for normalized texture render shaders
	tempShaderPtr->addShaderVariable("diffuseSampler"); //0
	tempShaderPtr->addShaderVariable("boxTopLeft"); //1
	tempShaderPtr->addShaderVariable("boxBottomRight"); //2
	tempShaderPtr->addShaderVariable("depth"); //3
}

/*

private draw methods

*/

void RenderEngine::drawQuad()
{
	glBindVertexArray(quadVao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void RenderEngine::drawLine()
{
	glBindVertexArray(lineVao);
	glLineWidth(3.0);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

/*

basic draw methods

*/

void RenderEngine::drawTexture(int textureIndex, glm::vec2& pos, glm::vec2& aspectRatio, float size, float depth)
{
	Shader* s = this->getShader(std::string("textureRendering")); //temp ptr to shader
	TextureManager &tm = this->parentEngine.getAssets().getTM(); //temp ref to texture manager
	glm::vec2 &ptw = this->getMainWindow()->getPixelToWindowRef(); //temp ref to pixel to window
	glm::vec2 cameraPos = this->activeCam.getPos(); //camera position

	//sets up
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	s->useShader();
	glActiveTexture(GL_TEXTURE0);
	tm.bind(textureIndex);
	glUniform1i(s->getShaderVar(0), 0);
	glUniform3f(s->getShaderVar(1), pos.x, pos.y, depth);
	glUniform2f(s->getShaderVar(2), cameraPos.x, cameraPos.y);
	glUniform1f(s->getShaderVar(3), size);
	glUniform2f(s->getShaderVar(4), aspectRatio.x, aspectRatio.y);
	glUniform2f(s->getShaderVar(5), ptw.x, ptw.y); //standard map zoom

	//draws
	this->drawQuad();
}

void RenderEngine::drawTextureN(int textureIndex, glm::vec2& topLeft, glm::vec2& bottomRight, float depth)
{
	Shader* s = this->getShader(std::string("textureNRendering")); //temp ptr to shader
	TextureManager &tm = this->parentEngine.getAssets().getTM(); //temp ref to texture manager
	glm::vec2 &ptw = this->getMainWindow()->getPixelToWindowRef(); //temp ref to pixel to window

	//sets up
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	s->useShader();
	glActiveTexture(GL_TEXTURE0);
	tm.bind(textureIndex);
	glUniform1i(s->getShaderVar(0), 0);
	glUniform2f(s->getShaderVar(1), topLeft.x, topLeft.y);
	glUniform2f(s->getShaderVar(2), bottomRight.x, bottomRight.y);
	glUniform1f(s->getShaderVar(3), depth);

	//draws
	this->drawQuad();
}

void RenderEngine::drawAnimation(int animationIndex, double &animTime, glm::vec2& pos, float depth, Orientation dir)
{
	Shader* s = this->getShader(std::string("animationRendering")); //temporary ptr
	AnimationManager &am = this->parentEngine.getAssets().getAM(); //temporary reference to animation manager
	AniFrame *currentFrame = nullptr;
	currentFrame = am.getAnimationFrame(animationIndex, animTime, dir);
	glm::vec4 tlbr = glm::vec4(currentFrame->m_topLeftTexcoord.x, currentFrame->m_topLeftTexcoord.y, currentFrame->m_bottomRightTexcoord.x, currentFrame->m_bottomRightTexcoord.y);
	glm::vec2 &ptw = this->getMainWindow()->getPixelToWindowRef(); //temp ref to pixel to window
	glm::vec2 cameraPos = this->activeCam.getPos(); //camera position

	//sets up
	glEnable(GL_BLEND);
	s->useShader();
	glActiveTexture(GL_TEXTURE0);
	this->parentEngine.getAssets().getTM().bind(am.getAtlas(animationIndex));
	glUniform1i(s->getShaderVar(0), 0);
	glUniform3f(s->getShaderVar(1), pos.x, pos.y, depth);
	glUniform2f(s->getShaderVar(2), cameraPos.x, cameraPos.y);
	glUniform1f(s->getShaderVar(3), 1.0);
	glUniform2f(s->getShaderVar(4), currentFrame->m_worldSize.x, currentFrame->m_worldSize.y);
	glUniform2f(s->getShaderVar(5), ptw.x, ptw.y); //standard map zoom
	glUniform4fv(s->getShaderVar(6), 1, glm::value_ptr(tlbr));

	//draws
	this->drawQuad();
}

void RenderEngine::drawLine(glm::vec2& wp, glm::vec2& wpt, glm::vec3& colour)
{
	Shader* s = this->getShader(std::string("lineRendering")); //temp ptr
	glm::vec2 &ptw = this->getMainWindow()->getPixelToWindowRef(); //temp ref to pixel to window
	glm::vec2 camPos = this->activeCam.getPos(); //camera position

	//sets up
	s->useShader();
	glUniform2f(s->getShaderVar(0), wp.x, wp.y);
	glUniform2f(s->getShaderVar(1), wpt.x, wpt.y);
	glUniform2f(s->getShaderVar(2), camPos.x, camPos.y);
	glUniform2f(s->getShaderVar(3), ptw.x, ptw.y); //standard map zoom
	glUniform3f(s->getShaderVar(4), colour.x, colour.y, colour.z);

	//draws
	this->drawLine();
}

void RenderEngine::drawString(int fontIndex, std::string &string, glm::vec2 cursor, float size, glm::vec4 colour)
{
	Shader* s = this->getShader(std::string("textRendering")); //temporary ptr
	Font *selectFont = this->parentEngine.getAssets().getFM().data(fontIndex); //ptr to the font we are printing in
	glm::vec2 &sptw = this->getMainWindow()->getSPixelToWindowRef(); //temp ref to pixel to window

	//change cursor to topleft corner
	cursor.y -= selectFont->getLineHeight() * size * sptw.y;

	//set up stuff that is same across all characters
	glEnable(GL_BLEND);
	s->useShader();
	glActiveTexture(GL_TEXTURE0);
	selectFont->bindAtlas();
	glUniform1i(s->getShaderVar(0), 0);
	glUniform1f(s->getShaderVar(1), size);
	glUniform4fv(s->getShaderVar(2), 1, glm::value_ptr(colour));

	for (char c : string) //draws every character
	{
		FontChar &character = selectFont->m_characters.at(c); //sets to each character in string

		//binds for drawing
		glUniform2f(s->getShaderVar(3), cursor.x, cursor.y);
		glUniform2f(s->getShaderVar(4), character.m_offset.x, character.m_offset.y);
		glUniform2f(s->getShaderVar(5), character.m_dimensions.x, character.m_dimensions.y);
		glUniform2f(s->getShaderVar(6), sptw.x, sptw.y);
		glUniform4f(s->getShaderVar(7), character.m_texcoords.tl.x, character.m_texcoords.tl.y, character.m_texcoords.br.x, character.m_texcoords.br.y);

		//draws
		this->drawQuad();

		cursor.x += character.m_advance * size * sptw.x; //moves cursor to next character
	}
}

void RenderEngine::drawStringInBox(int fontIndex, std::string& string, glm::vec2 cursor, float width, float size, glm::vec4 colour)
{
	Shader* s = this->getShader(std::string("textRendering")); //temporary ptr
	Font *selectFont = this->parentEngine.getAssets().getFM().data(fontIndex); //ptr to the font we are printing in
	glm::vec2 &sptw = this->getMainWindow()->getSPixelToWindowRef(); //temp ref to pixel to window

	glm::vec2 lineT = cursor; //the left line

	//change  to left = 1, right = -1
	//change cursor to topleft corner
	cursor.y -= selectFont->getLineHeight() * size * sptw.y;

	float lineR = lineT.x + width; //rightmost line's x
	float height = size * sptw.y; //word height/how much to go down when right line is hit

	//set up stuff that is same across all characters
	glEnable(GL_BLEND);
	s->useShader();
	glActiveTexture(GL_TEXTURE0);
	selectFont->bindAtlas();
	glUniform1i(s->getShaderVar(0), 0);
	glUniform1f(s->getShaderVar(1), size);
	glUniform4fv(s->getShaderVar(2), 1, glm::value_ptr(colour));

	for (char c : string) //draws every character
	{
		FontChar &character = selectFont->m_characters.at(c); //sets to each character in string

		//binds for drawing
		glUniform2f(s->getShaderVar(3), cursor.x, cursor.y);
		glUniform2f(s->getShaderVar(4), character.m_offset.x, character.m_offset.y);
		glUniform2f(s->getShaderVar(5), character.m_dimensions.x, character.m_dimensions.y);
		glUniform2f(s->getShaderVar(6), sptw.x, sptw.y);
		glUniform4f(s->getShaderVar(7), character.m_texcoords.tl.x, character.m_texcoords.tl.y, character.m_texcoords.br.x, character.m_texcoords.br.y);

		//draws
		this->drawQuad();

		cursor.x += character.m_advance * size * sptw.x; //movse cursor to next character
		if (cursor.x > lineR) //nextline if the cursor is over the right line
		{
			if (c == ' ')
			{
				cursor.x = lineT.x;
				cursor.y -= height;
			}
		}
	}
}

/*

updaters

*/

void RenderEngine::addShader(Shader &shaderobj)
{
	this->shaderPrograms.push_back(shaderobj);
}

void RenderEngine::useShader(std::string& name)
{
	for (int i = 0; i < this->shaderPrograms.size(); i++) //iterate and look for name
	{
		if (this->shaderPrograms[i].getNameRef() == name)
		{
			this->shaderPrograms[i].useShader(); //match
			return;
		}
	}

	this->shaderPrograms.at(0).useShader(); //none found
}

void RenderEngine::render(Component *renderComponent)
{
	if (renderComponent->getType() != RENDERER)
	{
		return; //early return if not renderer
	}
	
	renderComponent->renderBind(this); //draws to screen
}

void RenderEngine::updateScreen()
{
	this->mainWindow.swapBuffers();
}

/*

complex draw methods

*/
/**
void RenderEngine::drawWidget(Widget *drawTarget)
{
	this->drawTextureN(drawTarget->getTextureIndex(), glm::vec2(drawTarget->getTopLeftPos()), glm::vec2(drawTarget->getBottomRightPos()));
}

void RenderEngine::drawGUI(GUI *gui)
{
	std::vector<GameObject*> &children = gui->getChildren(); //reference to vector of children widgets that will be drawn

	this->drawWidget(gui);
	for (int i = 0; i < children.size(); i++)
	{
		this->drawWidget(children[i]); //draws the children
	}
}*/

void RenderEngine::drawMap(int mapIndex)
{
	MapManager& mm = this->parentEngine.getAssets().getMM(); //temp ref to map manager
	this->drawTexture(mm.getTextureIndexR(mapIndex), mm.getWP(mapIndex), glm::vec2(mm.getAR(mapIndex), 1.0f), mm.getSize(mapIndex));
}

void RenderEngine::drawAnimation(Entity *drawTarget)
{
	int animationIndex = drawTarget->getActiveAnimation();
	double &animTime = drawTarget->getAnimationTimerRef();
	glm::vec2 pos = drawTarget->getPos();
	float depth = drawTarget->getDepth();
	Orientation dir = drawTarget->getDirection();

	drawAnimation(drawTarget->getActiveAnimation(), drawTarget->getAnimationTimerRef(),
		drawTarget->getPos(), drawTarget->getDepth(), drawTarget->getDirection()); //same var names because I was lazy
}

void RenderEngine::drawAnimation(DamageArea *drawTarget)
{
	if (drawTarget->getDraw()) //checks whether or not drawTarget has an animation
	{
		//drawAnimation(drawTarget->getAnimation(), drawTarget->getAnimationTimerRef(), drawTarget->getOrigin(), 0.01f, UP);
	}
}

void RenderEngine::drawLine(DamageArea* a)
{
	//this->drawLine(a->getOrigin(), a->getEndPoint(), glm::vec3(1.0f, 1.0f, 0.0f));
}

void RenderEngine::drawText(Text *t)
{
	int alignment = t->getOrientation();

	if (alignment == 0) //left
	{
		this->drawStringInBox(t->getFontIndex(), t->getString(), t->getTopLeftPos(), t->getBottomRightPos().x - t->getTopLeftPos().x, t->getTextSize(), t->getColour()); //single draw since it's left-aligned
	}
	else if (alignment == 1) //center
	{
		//runs a test run through that splits strings into groups which can then be drawn one by one
		int fontIndex = t->getFontIndex();
		Font *selectFont = this->parentEngine.getAssets().getFM().data(fontIndex); //ptr to the font we are printing in
		glm::vec2 &sptw = this->getMainWindow()->getSPixelToWindowRef(); //temp ref to pixel to window

		glm::vec2 cursor(t->getTopLeftPos()); //cursor
		float lineR = t->getBottomRightPos().x; //rightmost boundary
		std::string temp = ""; //temporary string used to hold line being printed
		std::string text = t->getString();
		float tSize = t->getTextSize();
		glm::vec2 cp(t->getCenteredPos()); //centered position of string
		glm::vec4 colour = t->getColour();
		glm::vec2 ptl = t->getTopLeftPos(); //top left coordinates of the screen

		for (char c : text)
		{
			FontChar &character = selectFont->m_characters.at(c);

			temp += c;
			cursor.x += character.m_advance * tSize * sptw.x;

			if (cursor.x > lineR)
			{
				if (c == ' ')
				{
					cursor.x -= character.m_advance * tSize * sptw.x; //undo the cursor move of the ' ' character
					//gets width of the line offset (lineR - cursor.x) and adds it to the left most boundary for drawing
					this->drawString(fontIndex, temp, glm::vec2(cp.x - ((cursor.x - ptl.x) / 2.0f), cursor.y), tSize, colour);

					//resets some values
					cursor.x = ptl.x;
					cursor.y -= tSize * sptw.y;
					temp.clear();
				}
			}
		}

		if (temp != "") //draws what is left of the string if lineR has not been touched
		{
			this->drawString(fontIndex, temp, glm::vec2(cp.x - ((cursor.x - ptl.x) / 2.0), cursor.y), tSize, colour);
		}
	}
	else //2 which is right
	{
		//runs a test run through that splits strings into groups which can then be drawn one by one
		int fontIndex = t->getFontIndex();
		Font *selectFont = this->parentEngine.getAssets().getFM().data(fontIndex); //ptr to the font we are printing in
		glm::vec2 &sptw = this->getMainWindow()->getSPixelToWindowRef(); //temp ref to pixel to window

		glm::vec2 cursor(t->getTopLeftPos()); //cursor
		float lineR = t->getBottomRightPos().x; //rightmost boundary
		std::string temp = ""; //temporary string used to hold line being printed
		std::string text = t->getString();
		float tSize = t->getTextSize();
		glm::vec4 colour = t->getColour();
		glm::vec2 ptl = t->getTopLeftPos(); //top left coordinates of the screen

		for (char c : text)
		{
			FontChar &character = selectFont->m_characters.at(c);

			temp += c;
			cursor.x += character.m_advance * tSize * sptw.x;

			if (cursor.x > lineR)
			{
				if (c == ' ')
				{
					cursor.x -= character.m_advance * tSize * sptw.x; //undo the cursor move of the ' ' character
					//gets width of the line offset (lineR - cursor.x) and adds it to the left most boundary for drawing
					this->drawString(fontIndex, temp, glm::vec2(lineR - cursor.x + ptl.x, cursor.y), tSize, colour);

					//resets some values
					cursor.x = ptl.x;
					cursor.y -= tSize * sptw.y;
					temp.clear();
				}
			}
		}

		if (temp != "")
		{
			this->drawString(fontIndex, temp, glm::vec2(lineR - cursor.x + ptl.x, cursor.y), tSize, colour); //draws what is left of the string if lineR has not been touched
		}
	}
}

void RenderEngine::drawParticle(Particle *p)
{
	AnimationManager& am = this->parentEngine.getAssets().getAM();
	double age = p->getAge(); //does not update the time of animation on particle because that is updated in the update() for particle
	this->drawAnimation(am.search(*(p->getAnimationName())), age, p->getPos());
}

void RenderEngine::drawParticleFX(ParticleEffect * pfx)
{
	std::vector<Particle*> &particles = pfx->getBufferRef();
	for (int i = 0; i < particles.size(); i++) //iterates all particles and draws each
	{
		this->drawParticle(particles[i]);
	}
}

/*

window managing

*/

void RenderEngine::recreateScreenBuffer()
{
	delete this->sBuffer;
	this->sBuffer = new ScreenBuffer(this->mainWindow.getResolutionXRef(), this->mainWindow.getResolutionYRef());
	this->sBuffer->initScreenBuffer();
}

void RenderEngine::resizeWindow(int newX, int newY)
{
	this->mainWindow.changeWindowSizeTo(newX, newY); //changes the window's size
	this->mainWindow.changeWindowRes(); //changes the res to match the window's size with same aspect ratio

	this->recreateScreenBuffer(); //res is updated, render target must be updated
}

void RenderEngine::changeResolution(int newX, int newY)
{
	this->mainWindow.changeWindowResTo(newX, newY);

	this->recreateScreenBuffer(); //res is updated, render target must be updated
}
/*
void RenderEngine::resetCameraClamp()
{

}*/

/*

accessors

*/

GameWindow* RenderEngine::getMainWindow()
{
	return &(this->mainWindow);
}

Shader* RenderEngine::getShader(std::string& name)
{
	for (int i = 0; i < this->shaderPrograms.size(); i++) //iterate and look for name
	{
		if (this->shaderPrograms[i].getNameRef() == name)
		{
			return &(this->shaderPrograms[i]); //match
		}
	}

	return &(this->shaderPrograms.at(0)); //no match found
}

Camera* RenderEngine::getActiveCam()
{
	return &(this->activeCam);
}

RenderEngine::~RenderEngine()
{
	//dstr
}

/*

end rendering engine

*/