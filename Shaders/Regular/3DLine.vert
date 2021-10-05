#version 140
#extension GL_ARB_explicit_attrib_location : enable

in vec3 position;
in vec4 colours;

out vec4 colourPass;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 offset;

void main()
{
	vec4 worldPosition = vec4(position + offset * 64, 1.0);

	vec4 positionRelativeToCam = viewMatrix * worldPosition;
	gl_Position = projectionMatrix * positionRelativeToCam;

	colourPass = colours;
}