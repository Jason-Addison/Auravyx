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
	transformationMatrix = getUniformLocation((GLchar*) "transformationMatrix");
	colour = getUniformLocation((GLchar*) "colour");
}

void RectShader::loadTransformation(Matrix4f matrix)
{
	loadMatrix4f(transformationMatrix, matrix);
}

void RectShader::loadColour(float r, float g, float b, float a)
{
	loadVec4f(colour, r, g, b, a);
}