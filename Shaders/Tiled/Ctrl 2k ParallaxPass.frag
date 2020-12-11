#version 330 core

in vec2 vf_texcoord;
in vec3 vf_vsPos;

uniform sampler2D albedoSampler; //albedo
uniform sampler2D normalSampler; //normals
uniform sampler2D depthSampler; //depth map

uniform float texelOffset;

uniform bool parallax;
uniform float parallaxWeight = 0.007;
uniform float maxLayers = 4.0;

layout(location = 0) out vec4 out_albedo;
layout(location = 1) out vec4 out_normals;

//simple parallax mapping
vec2 parallaxMap(vec3 vsPos, vec2 actualTexcoord) //*note vsPos may need to be inversed
{
	vec3 viewRay = vsPos * parallaxWeight; //sets z of ray to 1 and then multiplies it by intended weight
	
	vec2 currentTexcoord = actualTexcoord;
	float height = texture(depthSampler, currentTexcoord).r;
	vec2 parallaxCoord = currentTexcoord - 5.0 * viewRay.xy * vec2(1.0 / 1280, 1.0 / 720);
	
	return parallaxCoord;
}

//combo of steep parallax mapping and linear interpolation
vec2 parallaxOcclusionMap(vec3 vsPos, vec2 actualTexcoord)
{
	//finds layer info
	float numLayers = maxLayers;
	float layerDepth = 1.0 / numLayers;
	
	//get viewRay
	vec3 viewRay = vsPos / vsPos.z * parallaxWeight; //sets z of ray to 1 and then multiplies it by intended weight
	
	//get values needed for calculations
	vec2 texcoordOffset = viewRay.xy / numLayers * 0.48;
	vec2 currentTexcoord = actualTexcoord;
	float currentDepth = texture(depthSampler, currentTexcoord).r;
	float currentLayer = 0.0;
	
	//per layer iteration until after collision
	while (currentLayer < currentDepth)
	{	
		currentTexcoord -= texcoordOffset;
		currentDepth = texture(depthSampler, currentTexcoord).r;
		currentLayer += layerDepth;
	}
	
	vec2 previousTexcoord = currentTexcoord + texcoordOffset; //finds previous values
	float previousDepth = texture(depthSampler, previousTexcoord).r;
	
	//calculates before and after collision depths
	float firstDepth = currentDepth - currentLayer;
	float secondDepth = previousDepth - currentLayer + layerDepth; //previous depth - previous layer
	
	//depth interpolation
	float interpolationWeight = firstDepth / (firstDepth - secondDepth); //a / (a - b)
	vec2 parallaxCoord = previousTexcoord * interpolationWeight + currentTexcoord * (1.0 - interpolationWeight);
	
	return parallaxCoord;
}

void main()
{
	vec2 texcoord = vf_texcoord;
	if (parallax)
	{
		texcoord = parallaxOcclusionMap(vf_vsPos, vf_texcoord); //if parallax mapping is needed, map
	}
	
	out_albedo = texture(albedoSampler, texcoord);
	out_normals = texture(normalSampler, texcoord);
}