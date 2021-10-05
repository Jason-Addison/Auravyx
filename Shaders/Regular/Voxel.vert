#version 140
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_EXT_texture_array : enable

in vec3 position;
in vec3 normal;
in vec3 positionA;
in vec3 positionB;
in vec3 positionC;
in uvec3 material;
//in vec2 textureCoords;
//in vec3 colour;

out vec3 normalPass;
out vec2 textureCoordsPass;
out vec4 shadowCoord;
out vec3 normalDirectionCS;
out vec3 surfaceNormal;
out vec3 lightDirectionCS;
out vec3 lightDirection;
out vec3 toCameraVector;
out vec3 positionPass;
flat out vec3 positionPassA;
flat out vec3 positionPassB;
flat out vec3 positionPassC;

flat out uint materialPassA;
flat out uint materialPassB;
flat out uint materialPassC;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 depthMVP;

uniform float time;
uniform vec3 wind;

const float dir = -0.70710666564;
const float no = 0.1;
void main()
{
	vec4 worldPosition = vec4(position, 1.0);
	vec4 positionRelativeToCam = viewMatrix * worldPosition;
	gl_Position = projectionMatrix * positionRelativeToCam;
	
	vec3 normalWS = (vec4(normal, 0.0)).xyz;
	surfaceNormal = normal;////////normalWS;
	normalDirectionCS = (viewMatrix * vec4(normal, 0.0)).xyz;
	
	shadowCoord = depthMVP * vec4(worldPosition.x + normalWS.x * no, worldPosition.y + normalWS.y * no, worldPosition.z + normalWS.z * no, worldPosition.w);

	toCameraVector = (inverse(viewMatrix) * vec4(0.0,0.0,0.0,1.0)).xyz - worldPosition.xyz;

	surfaceNormal = normalWS;

	positionPass = worldPosition.xyz;

	positionPassA = positionA;
	positionPassB = positionB;
	positionPassC = positionC;

	materialPassA = material.x;
	materialPassB = material.y;
	materialPassC = material.z;
}