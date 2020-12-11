#version 330 core

layout(location = 0) out vec4 out_colour;

uniform vec3 in_colour;

void main()
{
	out_colour.rgb = in_colour;
	out_colour.a = 1.0;
}