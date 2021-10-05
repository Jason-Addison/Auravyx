#version 140

in vec2 position;
uniform vec4 colour;
out vec4 colourPass;
uniform mat4 transformationMatrix;

void main()
{
	gl_Position = transformationMatrix * vec4(position, 0.0, 1.0);
	gl_Position.y = gl_Position.y;
	colourPass = colour;
}