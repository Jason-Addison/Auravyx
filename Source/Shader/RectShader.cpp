#include "stdafx.h"
#include "Shader/RectShader.h"

RectShader::RectShader()
{
	shader = "Rect";
}

RectShader::~RectShader()
{
}

void RectShader::loadAllUniformLocations()
{
	transformationMatrix = getUniformLocation("transformationMatrix");
	colour = getUniformLocation("colour");
}

void RectShader::loadTransformation(const Matrix4f& matrix)
{
	loadMatrix4f(transformationMatrix, matrix);
}

void RectShader::loadColour(const float r, const float g, const float b, const float a)
{
	loadVec4f(colour, r, g, b, a);
}