#version 140

in vec2 position;
in vec2 texCoords;

out vec2 textureCoords;
out vec4 colourPass;

out vec2 thicknessPass;

uniform mat4 transformationMatrix;
uniform vec4 colour;

uniform vec2 thickness;
uniform float mode;

void main(void)
{
	textureCoords = texCoords;
    gl_Position = transformationMatrix * vec4(position.x, position.y, 0.0, 1.0);
    colourPass = colour;
    thicknessPass = thickness;
}