#version 330 core

layout(location = 0) in vec3 in_ndcPosition;
layout(location = 1) in vec2 in_texcoord;

out vec2 vf_texcoord;
out vec3 vf_vsPos;

uniform float scale;

void main()
{
	gl_Position = vec4(in_ndcPosition.xy * scale, in_ndcPosition.z, 1.0);
	vf_vsPos.xy = -in_ndcPosition.xy * scale;
	vf_vsPos.z = 1.0 * scale;
	vf_texcoord = in_texcoord;
}