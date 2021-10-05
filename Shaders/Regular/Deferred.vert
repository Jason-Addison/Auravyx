#version 140
#extension GL_ARB_explicit_attrib_location : enable

in vec2 position;

out vec3 cameraPos;
out vec3 toCameraVector;

out vec2 textureCoords;
out vec3 sunDirection;
out vec4 ambientLightPass;

uniform vec4 ambientLight;

void main(void)
{
	gl_Position = vec4((position - 0.5) * 2, 0.0, 1.0);

    textureCoords = position;

    ambientLightPass = vec4(1, 1, 1, 1);
}