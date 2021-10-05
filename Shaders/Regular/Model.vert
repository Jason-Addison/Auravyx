#version 140
#extension GL_ARB_explicit_attrib_location : enable

in vec3 position;
in vec3 normal;
in vec2 uv;

out vec3 normalPass;
out vec2 uvPass;
out vec3 positionPass;
out vec3 colorPass;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformationMatrix;

uniform vec3 cameraPosition;
uniform float farPlane;

uniform float time;

void main()
{
	vec4 worldPosition = vec4(position, 1.0) * transformationMatrix;
	vec4 positionRelativeToCam = viewMatrix * worldPosition;
	gl_Position = projectionMatrix * viewMatrix * transformationMatrix * vec4(position, 1.0);//positionRelativeToCam;

	mat3 normalMatrix = mat3(transformationMatrix);
	normalMatrix = inverse(normalMatrix);
	normalMatrix = transpose(normalMatrix);
	normalPass = normalize(normal * normalMatrix);
	normalPass = (vec3(normalPass.x, normalPass.y, normalPass.z) + 1) / 2;
	uvPass = uv;

	positionPass = vec3(((-worldPosition.xyz) / farPlane + 1) / 2);
}