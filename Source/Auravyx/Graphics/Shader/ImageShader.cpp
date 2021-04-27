#include "Auravyx/Graphics/Shader/ImageShader.h"

ImageShader::ImageShader()
{
	shader = "2DImage";
}

ImageShader::~ImageShader()
{
}

void ImageShader::loadAllUniformLocations()
{
	transformationMatrix = getUniformLocation("transformationMatrix");
}

void ImageShader::loadTransformation(const Matrix4f& matrix)
{
	loadMatrix4f(transformationMatrix, matrix);
}