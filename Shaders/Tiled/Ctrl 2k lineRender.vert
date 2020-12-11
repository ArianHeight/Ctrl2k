#version 330 core

layout(location = 0) in vec2 in_position;

//Projection, View Matrix and Model Matrix.
uniform vec2 worldDisplace;
uniform vec2 worldDisplaceTwo;
uniform vec2 viewDisplace;
uniform vec2 unitToScreen; //how many pixels a single unit is on screen

void main()
{
	vec2 pos = in_position;
	if (pos.x < 0.0)
	{
		pos = worldDisplace;
	}
	else
	{
		pos = worldDisplaceTwo;
	}
	gl_Position = vec4((pos - viewDisplace) * unitToScreen, 0.0, 1.0);
}