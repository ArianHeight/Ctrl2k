#version 330 core

in vec2 vf_texcoord;

uniform sampler2D targetTexture;
uniform sampler2D bloomTexture;

uniform vec2 texelOffset;
uniform float exposure;

layout(location = 0) out vec4 out_colour;

void fxaa()
{
	//fxaa params
	float maxBlurSpan = 7.0;
	float reduceBlurMin = 1.0 / 128.0; //lowest value for blurring directional multiplier
	float reduceBlurMul = 1.0 / 6.0; //luma reduce bias

	//finds texel colours in x-shape
	vec3 rgbTL = texture2D(targetTexture, vf_texcoord + (vec2(-1.0, -1.0) * texelOffset)).rgb;
	vec3 rgbTR = texture2D(targetTexture, vf_texcoord + (vec2(1.0, -1.0) * texelOffset)).rgb;
	vec3 rgbBL = texture2D(targetTexture, vf_texcoord + (vec2(-1.0, 1.0) * texelOffset)).rgb;
	vec3 rgbBR = texture2D(targetTexture, vf_texcoord + (vec2(1.0, 1.0) * texelOffset)).rgb;
	vec3 rgbM = texture2D(targetTexture, vf_texcoord).rgb;
	
	//calculates individual texel luminosity
	vec3 luma = vec3(0.299, 0.587, 0.114);
	float lumaTL = dot(luma, rgbTL);
	float lumaTR = dot(luma, rgbTR);
	float lumaBL = dot(luma, rgbBL);
	float lumaBR = dot(luma, rgbBR);
	float lumaM = dot(luma, rgbM);
	float minLuma = min(min(lumaTL, lumaTR), min(min(lumaM, lumaBL), lumaBR));
	float maxLuma = max(max(lumaTL, lumaTR), max(max(lumaM, lumaBL), lumaBR));

	vec2 blurDirection;
	blurDirection.x = -((lumaTL + lumaTR) - (lumaBL + lumaBR));
	blurDirection.y = ((lumaTL + lumaBL) - (lumaTR + lumaBR));

	//scales smallest value in blurDirection to 1 and clamps largest values to maxBlurSpan
	float reduceDirectionMul = max((lumaTL + lumaTR + lumaBL + lumaBR) * (0.25 * reduceBlurMul), reduceBlurMin); //biases directional reduction multiplier by lumaniscense
	float directionMultiplier = 1.0 / (min(abs(blurDirection.x), abs(blurDirection.y)) + reduceDirectionMul);
	blurDirection = min(vec2(maxBlurSpan, maxBlurSpan), max(vec2(-maxBlurSpan, -maxBlurSpan), blurDirection * directionMultiplier));
	
	//transforms blurDirection into texel space
	blurDirection = blurDirection * texelOffset;

	vec3 firstRGB; //blurred to half the sample
	firstRGB = 0.5 * (texture2D(targetTexture, vf_texcoord + (blurDirection * vec2(1.0 / 3.0 - 0.5))).rgb + texture2D(targetTexture, vf_texcoord + (blurDirection * vec2(2.0 / 3.0 - 0.5))).rgb);

	vec3 secondRGB; //blurred to the whole sample
	secondRGB = firstRGB * 0.5 + 0.25 * (texture2D(targetTexture, vf_texcoord + (blurDirection * vec2(0.0 / 3.0 - 0.5))).rgb + texture2D(targetTexture, vf_texcoord + (blurDirection * vec2(3.0 / 3.0 - 0.5))).rgb);

	float secondSampleLuma = dot(luma, secondRGB); //luma of second sample
	
	if (secondSampleLuma < minLuma || secondSampleLuma > maxLuma) //test second sample's luma's range against overall range in the 5 tested texels
	{
		out_colour = vec4(firstRGB, 1.0);
	}
	else
	{
		out_colour = vec4(secondRGB, 1.0);
	}
}

void reinhardToneMap()
{
	//out_colour is hdr value
	out_colour.rgb += texture2D(bloomTexture, vf_texcoord).rgb; //blending
	out_colour.rgb = vec3(1.0) - exp(-out_colour.rgb * 3.2); //reinhard tone mapping, * by exposure
	out_colour.rgb = pow(out_colour.rgb, vec3(1.0 / 0.5)); //1.0 divded by gamma
}

void main()
{
	fxaa();
	reinhardToneMap();
}