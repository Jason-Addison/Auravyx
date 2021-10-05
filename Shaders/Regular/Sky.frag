#version 140
#extension GL_ARB_explicit_attrib_location : enable

in vec3 normalPass;
in vec2 uvPass;

layout(location = 0) out vec4 frag_colour;
layout(location = 1) out vec4 normalOut;
layout(location = 2) out vec4 specularOut;
layout(location = 3) out vec4 glowOut;
layout(location = 4) out vec4 positionOut;

uniform sampler2D stars;
uniform vec3 sun;
uniform vec2 screenResolution;
uniform float time;

vec3 skyColour = vec3(0, 0.2, 0.8) * 1.0;
vec3 horizonColour = vec3(1, 1, 1.3333) * 0.75;;//vec3(1, 1, 1.2) * 0.6;
vec3 sunRiseColour = vec3(1, 0.27, 0);
vec3 sunSetColour = vec3(1, 0.4, 0);

float distanceBetweenPoints(vec3 a, vec3 b)
{
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
}

float range(float min, float max, float newMin, float newMax)
{
	return (newMax - newMin) / (max - min);
}
const float coeiff = 0.25;
const vec3 totalSkyLight = vec3(0.3, 0.5, 1.0);

vec3 mie(float dist, vec3 sunL)
{
    return max(exp(-pow(dist, 0.25)) * sunL - 0.4, 0.0);
}


void main()
{
	frag_colour = texture2D(stars, vec2(uvPass.x, uvPass.y) * 0.01);

	float mod = 1 - (normalPass.y+0.333)/1.5;
	frag_colour = vec4(mod, mod, 1, 1); 
	if(normalPass.y < 0)
	{
		//frag_colour = vec4(0.7777, 0.7777, 1, 1); 
	}

	float sunSkyNormal = (dot(normalize(normalPass), normalize(sun)));

	
	float n = 1 - pow((1-pow(sin(time * 3.14), 8)), 8);
	float skyInterp = ((n + 0.09) / 1.09);

	float skyHorizonMix = pow(abs(normalPass.y), 0.4) - 0.2;
	
	if(skyHorizonMix < 0)
	{
		skyHorizonMix = 0;
	}

	vec3 skyHorizon = (skyColour * skyInterp) * skyHorizonMix + (1 - skyHorizonMix) * (horizonColour * skyInterp);

	float sunSkyMix = (1 - (pow(abs(normalPass.y), 0.4) * 1)) + 0.2;
	if(sunSkyMix > 1)
	{
		sunSkyMix = 1;
	}
	sunSkyMix = clamp(sunSkyMix, 0, 1);

	vec3 sunColour = sunRiseColour;
	float riseSet = (normalPass.x + 1)/2;
	if(time < 0.5)
	{
		riseSet = (-normalPass.x + 1)/2;
		sunColour = sunSetColour;
	}
	riseSet = clamp(riseSet, 0, 1);

	riseSet *= 1 - pow(sin((time + 0.27) * 3.14 * 2), 2);

	riseSet = clamp(riseSet, 0, 1);

	vec4 star = texture2D(stars, vec2(uvPass.x, uvPass.y) * 0.8);
	vec3 sunSky = (sunSkyMix * riseSet) * sunColour + (1 - (sunSkyMix * riseSet)) * (skyHorizon);
	
	vec3 starT = vec3(0, 0, 0);

	frag_colour = vec4(sunSky, 1);
	//float starFilter = normalPass.y + 0.3;

	float sunGlareMax = 0.001 + (sunSkyMix * riseSet) / 400;
	if(frag_colour.b > 1)
	{
		frag_colour.b = 1;
	}
	if(1 - sunSkyNormal < sunGlareMax * 150)
	{	
		float w = (sunSkyNormal - (1 - sunGlareMax * 150)) * (1 / (sunGlareMax * 150));
		w *= 0.9;
		if(w > 1)
		{	
			w = 1;
		}
		w = pow(w, 8);
		w *= 1 + ((sunSkyMix * riseSet) / 200);
		//frag_colour += w * vec4(1, 1, 1, 1);
		if(normalPass.y < -0.2)
		{
			//frag_colour.xyz = 1 * frag_colour.xyz + w *((sunSkyMix * riseSet));
		}
		//else
		{
			frag_colour.xyz = (1 - w) * frag_colour.xyz + w *((sunSkyMix * riseSet) * ((sunColour + vec3(1,1,1) * 4) / 5) *2 + (1 - (sunSkyMix * riseSet)) * 2) * vec3(1, 1, 1) * 1;
		}
		
		//glowOut = (1 - w) * vec4(0, 0, 0, 1) + w * vec4(1, 1, 1, 1);
	}

	if(star.r > 0.6)
	{
		starT = vec3(1, 1, 1);
		float brightness = normalize(sunSky).length();
		if(sunSky.r + sunSky.g + sunSky.b < 0.8)
		{
			brightness = clamp(sunSky.r + sunSky.g + sunSky.b, 0.0, 0.8);

			float bs = 1 - (brightness * 1.25);
			frag_colour.xyz = frag_colour.xyz * (1 - bs) + bs * vec3(1, 1, 1);
		}
	}

	if(normalPass.y < 0)
	{
		//sunSky = vec3(0,0,0);
	}
	//float e = abs(pow(1 - sin(time * 3.14 * 4) - 1, 7));
	frag_colour.w = 1;
	normalOut = vec4(1, 1, 1, 1);
	specularOut = vec4(0, 0, 0, 1);
	glowOut = vec4(0, 0, 0, 1);
	positionOut = vec4(1, 1, 1, 1);
}