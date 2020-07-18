#pragma once
#include "GL\glew.h"
#include "xstring"    // for string
class Texture
{
public:
	Texture();
	Texture(GLuint texture, int width, int height);
	~Texture();

	GLuint texture;
	int width;
	int height;
	std::string name;

	GLuint getTexture();

	int getWidth();

	int getHeight();
};

