#pragma once
#include "GL/glew.h"
class ModelMaterial
{
public:

	GLuint albedo;
	GLuint normal;
	GLuint specular;
	GLuint ambientOcclusion;

	GLuint length = -1;

	bool textured = false;

	float r;
	float g;
	float b;
	float a;
};

