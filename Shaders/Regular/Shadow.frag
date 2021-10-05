#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) out float fragmentdepth;

void main()
{
	fragmentdepth = gl_FragCoord.z;
}