#include "stdafx.h"
#include "ImageShader.h"

ImageShader::ImageShader()
{
	shader = "2DImage";
}

ImageShader::~ImageShader()
{
}

void ImageShader::loadAllUniformLocations()
{
	const char* s = std::string("transformationMatrix").c_str();
	char* s1 = (char *) s;
	transformationMatrix = getUniformLocation(s1);
}

void ImageShader::loadTransformation(Matrix4f matrix)
{
	loadMatrix4f(transformationMatrix, matrix);
}