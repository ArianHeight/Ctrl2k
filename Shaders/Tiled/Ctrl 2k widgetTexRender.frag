#version 330 core

//Position, and Texcoord from vertex shader.
in vec2 vf_texcoord;

uniform sampler2D bitSampler;
uniform vec4 textColour;

layout(location = 0) out vec4 out_colour;

void main()
{	
	out_colour.rgb = textColour.rgb;
	out_colour.a = texture(bitSampler, vf_texcoord).a * textColour.a;
}