#version 330 core

layout(location = 0) in vec3 in_ndcPosition;
layout(location = 1) in vec2 in_texcoord;

out vec2 vf_texcoord;

void main()
{
	gl_Position = vec4(in_ndcPosition, 1.0);
	vf_texcoord = in_texcoord;
}