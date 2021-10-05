#version 140
#extension GL_ARB_explicit_attrib_location : enable

in vec3 normalPass;
in vec2 uvPass;
in vec3 positionPass;

layout(location = 0) out vec4 albedo;
layout(location = 1) out vec4 normalOut;
layout(location = 2) out vec4 specularOut;
layout(location = 3) out vec4 glowOut;
layout(location = 4) out vec4 positionOut;

layout(location = 10) out vec4 settings;

uniform sampler2D texture;

void main()
{
	albedo = texture2D(texture, uvPass);
	if(albedo.w < 1)
	{
		discard;
	}
	normalOut = vec4(normalPass.x, normalPass.y, normalPass.z, 1);
	specularOut = vec4(0.01, 0.01, 1, 1);
	glowOut = vec4(0, 0, 0, 1);
	positionOut = vec4(positionPass, 1.0);
	//settings = vec4(1, 0, 0, 1);
}