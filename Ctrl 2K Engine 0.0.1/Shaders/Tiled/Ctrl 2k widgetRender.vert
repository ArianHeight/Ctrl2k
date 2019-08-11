#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texcoord;

out vec2 vf_texcoord;

uniform vec2 dimensions;
uniform vec2 boxTopLeft;

void main()
{
	gl_Position = vec4(((in_position.xy + vec2(1.0f, -1.0f)) * 0.5f * dimensions) + boxTopLeft, 0.0f, 1.0f);

	vf_texcoord = in_texcoord;
}