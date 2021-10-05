#version 140
#extension GL_ARB_explicit_attrib_location : enable

in vec3 position;
in vec3 normal;
in vec2 uv;

out vec3 normalPass;
out vec2 uvPass;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformationMatrix;

uniform float time;

void main()
{

	vec4 worldPosition = vec4(position, 1.0) * transformationMatrix;
	normalPass = normalize(worldPosition.xyz);
	vec4 positionRelativeToCam = viewMatrix * worldPosition;
	gl_Position = projectionMatrix * positionRelativeToCam;

	uvPass = uv;
}