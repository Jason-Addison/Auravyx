#pragma once
#include "Library\GL\glew.h"
#include <string>
class Texture
{
public:
	Texture();
	Texture(const GLuint texture, const int width, const int height);
	~Texture();

	GLuint texture;
	int width;
	int height;
	std::string name;

	GLuint getTexture();

	int getWidth();

	int getHeight();

	void destroy();
};

