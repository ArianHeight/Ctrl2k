#version 330 core

in vec2 vf_texcoord;

uniform sampler2D targetTexture;

uniform vec2 direction;
uniform vec2 texelOffset;

uniform float contribution[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
//uniform float contribution[7] = float[] (0.2186, 0.18813, 0.11991, 0.05661, 0.01979, 0.00513, 0.001);

layout(location = 0) out vec4 out_colour;

void main()
{
	out_colour = texture2D(targetTexture, vf_texcoord) * contribution[0];
	vec2 oneTex = direction * texelOffset;
	for (int count = 1; count < 5; count++)
	{
		out_colour += texture2D(targetTexture, vf_texcoord + oneTex * count) * contribution[count];
		out_colour += texture2D(targetTexture, vf_texcoord - oneTex * count) * contribution[count];
	}
	
	out_colour.a = 1.0;
}