#pragma once

/*

value  Slider object which appears on screen

*/

class Slider : public Widget
{
public:
	Slider();
	virtual ~Slider();

private:
	int m_sliderValue; //current slider value
	int m_sliderMinValue;
	int m_sliderMaxValue;

	int m_textureBufferSliderIndex; //texture index of the slider

public:
	void drawWidget(); //draws widget onto screen
};

/*

end Slider object

*/