#version 330 core

//Position, and Texcoord from vertex shader.
in vec2 vf_texcoord;

uniform sampler2D bitSampler;
uniform vec4 tintColour = vec4(1.0f);

layout(location = 0) out vec4 out_colour;

void main()
{	
	out_colour = texture(bitSampler, vf_texcoord) * tintColour;
}