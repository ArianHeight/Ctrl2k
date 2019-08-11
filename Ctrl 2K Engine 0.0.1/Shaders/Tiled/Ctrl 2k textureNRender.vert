#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texcoord;

out vec2 vf_texcoord;

uniform vec2 boxBottomRight;
uniform vec2 boxTopLeft;
uniform float depth;

void main()
{
	gl_Position = vec4(0.0f, 0.0f, -depth, 1.0f);

	gl_Position.x = (in_position.x < 0.0f ? boxTopLeft.x : boxBottomRight.x); //smaller than 0 is left, greater than 0 is right
	gl_Position.y = (in_position.y < 0.0f ? boxBottomRight.y : boxTopLeft.y); //smaller than 0 is bottom, greater is top

	vf_texcoord = in_texcoord;
}