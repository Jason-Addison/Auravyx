#version 140
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_EXT_texture_array : enable

in vec3 normalPass;
in vec2 textureCoordsPass;
in vec4 shadowCoord;
in vec3 lightDirection;
in vec3 position;
in vec3 cameraPos;
in vec3 positionPass;
flat in vec3 positionPassA;
flat in vec3 positionPassB;
flat in vec3 positionPassC;
in vec3 toCameraVector;
flat in uint materialPassA;
flat in uint materialPassB;
flat in uint materialPassC;

in vec3 normalDirectionCS;
in vec3 lightDirectionCS;
in vec3 surfaceNormal;

layout(location = 0) out vec4 frag_colour;
layout(location = 1) out vec4 normalOut;
layout(location = 2) out vec4 specularOut;
layout(location = 3) out vec4 glowOut;
layout(location = 4) out vec4 positionOut;
uniform sampler2DArray terrainTexture;
uniform sampler2D specular;
uniform sampler2D shadowMap;

uniform vec3 cameraPosition;
uniform float farPlane;

uniform float shine;
uniform float damper;

const int pcfCount = 2;
const float texelCount = (pcfCount * 2.0 + 1.0) * (pcfCount * 2.0 + 1.0);
const float mapSize = 2048;
const float texelSize = 1.0 / mapSize;

float distanceBetweenPoints(vec3 a, vec3 b)
{
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
}

vec3 barycentric(vec3 p, vec3 a, vec3 b, vec3 c)
{
    vec3 v0 = b - a;
	vec3 v1 = c - a;
	vec3 v2 = p - a;
    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;
	return vec3(v, w, u);
}
float b;
float scale = 0.25;

float saturate(float x)
{
	return clamp(x, 0.0, 1.0);
}

vec3 weightedTriMapping(vec3 positionPass, vec3 vPos, uint material, vec3 surfaceNormal)
{
	positionPass.y += 0.125;
	vec3 xaxis = texture( terrainTexture, vec3(positionPass.yz * scale, material)).rgb;
	vec3 yaxis = texture( terrainTexture, vec3(positionPass.xz * scale, material)).rgb;
	vec3 zaxis = texture( terrainTexture, vec3(positionPass.xy * scale, material)).rgb;
	
	vec3 blending = abs(vec3(surfaceNormal.x, surfaceNormal.y, surfaceNormal.z));

	vec2 xzBlend = abs(normalize(surfaceNormal.xz));
	blending.xz = max(vec2(0, 0), xzBlend - 0.67);
	blending.xz /= max(vec2(0.00001,0.00001), dot(blending.xz, vec2(1,1)));
	// Blend for top
	blending.y = saturate((abs(surfaceNormal.y) - 0.675) * 100.0);
	blending.xz *= (1 - blending.y);

	return vec3(xaxis.xyz * blending.x + yaxis.xyz * blending.y + zaxis.xyz * blending.z);
}
vec3 weightedTerrainTexture(vec3 fragPosition, vec3 posA, vec3 posB, vec3 posC, uint materialA, uint materialB, uint materialC, vec3 surfaceNormal)
{
	vec3 a = weightedTriMapping(fragPosition, posA, materialA, surfaceNormal);
	vec3 b = weightedTriMapping(fragPosition, posB, materialB, surfaceNormal);
	vec3 c = weightedTriMapping(fragPosition, posC, materialC, surfaceNormal);
	vec3 weight = barycentric(fragPosition, posA, posB, posC);

	return vec3(a * weight.x + b * weight.y + c * weight.z);
}

void main()
{
	normalOut = (vec4(surfaceNormal, 1) + 1) / 2;
	
	positionOut = vec4(((positionPass - cameraPosition) / farPlane + 1) / 2, 1.0);
	
	float s = 0 * (shine + 100);
	specularOut = vec4(0.01, 0.01, 0, 1);
	
	glowOut = vec4(0, 0, 0, 1);

	frag_colour = vec4(1, 1, 1, 1);

	frag_colour.xyz = weightedTerrainTexture(positionPass.xyz, positionPassA, positionPassB, positionPassC, materialPassA, materialPassB, materialPassC, surfaceNormal);
}