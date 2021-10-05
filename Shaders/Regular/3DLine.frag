#version 140
#extension GL_ARB_explicit_attrib_location : enable

in vec4 colourPass;

layout(location = 0) out vec4 frag_colour;
layout(location = 1) out vec4 normalOut;
layout(location = 2) out vec4 specularOut;
layout(location = 3) out vec4 glowOut;
layout(location = 4) out vec4 positionOut;

void main()
{
	frag_colour = colourPass;

	normalOut = vec4(1, 1, 1, 1);

	specularOut = vec4(0, 0, 0, 0);

	glowOut = vec4(0, 0, 0, 0);

	positionOut = vec4(0, 0, 0, 0);
}