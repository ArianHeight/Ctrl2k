#include <Ctrl 2kPCH.h>

/*

HUD class

*/

HUD::HUD() :
	GUI("Heads-Up Display", glm::vec2(-1.0f, 1.0f), glm::vec2(1.0f, -1.0f))
{
	//cstr
}

void HUD::activate(Text *fps, StatusBar *hp, StatusBar *sp, StatusBar *mp)
{
	if (this->game != nullptr)
	{
		this->active = true;

		//links to self
		this->fpsWidget = fps;
		this->healthBar = hp;
		this->staminaBar = sp;
		this->magicBar = mp;

		//adds to child for rendering and updating
		this->addChild(fps);
		this->addChild(hp);
		this->addChild(sp);
		this->addChild(mp);
	}
}

void HUD::update()
{
	GUI::update();
	if (this->active)
	{
		this->fpsWidget->setString(std::to_string(int(this->game->getEngine().getFPS())));

		if (this->targetObj != nullptr) //check if camera is centered on an object
		{
			Component *eData = this->targetObj->getComponent(std::string("Statistics"));
			if (eData != nullptr) //is and entity and has statistics to display
			{
				this->healthBar->setPercentage(eData->getHealth() / 100.0);
				this->staminaBar->setPercentage(eData->getStamina() / 100.0);
				this->magicBar->setPercentage(eData->getMagic() / 100.0);
			}
		}
	}
}

HUD::~HUD()
{
	//dstr
	this->fpsWidget = nullptr;
	this->healthBar = nullptr;
	this->staminaBar = nullptr;
	this->magicBar = nullptr;
}

/*

end HUD

*/