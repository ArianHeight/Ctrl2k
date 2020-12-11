#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texcoord;

out vec2 vf_texcoord;

//Projection, View Matrix and Model Matrix.
uniform vec3 worldDisplace;
uniform vec2 viewDisplace;
uniform float worldSize;
uniform vec2 unitToScreen; //how many pixels a single unit is on screen
uniform vec2 aspectRatio;

void main()
{
	gl_Position = vec4((in_position.xy * worldSize * aspectRatio + (worldDisplace.xy - viewDisplace)) * unitToScreen, -worldDisplace.z, 1.0f);
	vf_texcoord = in_texcoord;
}