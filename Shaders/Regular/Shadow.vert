#version 140
#extension GL_ARB_explicit_attrib_location : enable

in vec3 position;
layout(location = 3) in vec3 material;

uniform mat4 depthMVP;
uniform mat4 transformationMatrix;
uniform float time;
uniform vec3 wind;

void main()
{
	float strength = material.x * 0.5;
	float weight = material.y * 0.5;

	vec4 globalPosition = vec4(0,0,0,1);//vec4(position, 1); // transformationMatrix *
	vec3 weatherPosition = position;
	//weatherPosition.x += sin(cos(position.x) * (wind.x * time * 30 * strength) + 2.3) * weight * 0.1;
	//weatherPosition.z += sin(sin(position.z) * (wind.x * time * 30 * strength)) * weight * 0.1;

	//weatherPosition.x += (sin(time * wind.x * 6 + globalPosition.x) * (wind.y * position.y) * 0.1 + wind.y * 0.2 * position.y - wind.y * 0.1 * position.y) * strength;
	//weatherPosition.z += (sin(time * wind.x * 6 + globalPosition.z) * (wind.z * position.y) * 0.1 + wind.z * 0.2 * position.y - wind.z * 0.1 * position.y) * strength;

	gl_Position =  depthMVP * (transformationMatrix * vec4(position, 1.0));
}