#version 140

in vec2 textureCoordsPass;
out vec4 frag_colour;

uniform sampler2D texture2d;

void main()
{
	frag_colour = texture2D(texture2d, vec2(textureCoordsPass.x, textureCoordsPass.y));
}