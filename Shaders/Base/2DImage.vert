#version 140

in vec2 position;

out vec2 textureCoordsPass;
uniform mat4 transformationMatrix;

void main()
{
	textureCoordsPass = position;
	textureCoordsPass.y = textureCoordsPass.y;
	gl_Position = transformationMatrix * vec4(position, 0.0, 1.0);
	gl_Position.y = gl_Position.y;
}