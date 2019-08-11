#pragma once

/*

inherited from GUI
Heads Up Display

*/

class HUD : public GUI
{
public:
	HUD();
	virtual ~HUD();

private:
	//these widgets are managed internally by the gameObject so they do not need to be deleted from this child class manually
	Text *fpsWidget; //no ownership, ownership is in m_children
	StatusBar *healthBar;
	StatusBar *staminaBar;
	StatusBar *magicBar;

	bool active{ false };

public:
	virtual void update(); //updates all the values and positions
	void activate(Text *fps, StatusBar *hp, StatusBar *sp, StatusBar *mp); //required to render stuff
};

/*

end HUD

*/