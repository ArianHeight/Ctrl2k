#version 330 core

in vec2 vf_texcoord;

uniform sampler2D source;

layout(location = 0) out vec4 out_colour;

void main()
{
	out_colour = texture2D(source, vf_texcoord);
}