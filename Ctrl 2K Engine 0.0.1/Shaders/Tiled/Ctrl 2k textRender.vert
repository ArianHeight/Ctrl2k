#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texcoord;

out vec2 vf_texcoord;

uniform vec2 cursor;
uniform vec2 offsets;
uniform float worldSize;
uniform vec2 unitToScreen; //how many pixels a single unit is on screen
uniform vec2 aspectRatio; //ar, char height

uniform vec4 tlbr; //texcoords of the two corners

void main()
{
	gl_Position = vec4(((in_position.xy + vec2(1.0f)) * 0.5f * worldSize * aspectRatio - offsets) * unitToScreen + cursor, 0.0f, 1.0f);

	vf_texcoord = in_texcoord;
	vf_texcoord.x = (vf_texcoord.x == 0) ? tlbr.x : tlbr.z;
	vf_texcoord.y = (vf_texcoord.y == 0) ? tlbr.w : tlbr.y;
}