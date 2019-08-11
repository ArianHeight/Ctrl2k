#version 330 core

//Position, and Texcoord from vertex shader.
in vec2 vf_texcoord;

uniform sampler2D diffuseSampler;

layout(location = 0) out vec4 out_colour;

void main()
{	
	out_colour = texture(diffuseSampler, vf_texcoord);
}