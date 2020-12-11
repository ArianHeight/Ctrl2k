#version 330 core

in vec2 vf_texcoord;

uniform sampler2D bloom; //5x5
uniform sampler2D bloomTwo; //11x11
uniform sampler2D bloomFour; //21x21
uniform sampler2D bloomSixteen; //42x42

layout(location = 0) out vec4 out_colour;

void main()
{
	out_colour.rgb = texture2D(bloom, vf_texcoord).rgb + texture2D(bloomTwo, vf_texcoord).rgb + texture2D(bloomFour, vf_texcoord).rgb + texture2D(bloomSixteen, vf_texcoord).rgb;
	out_colour.rgb *= 0.2;
}