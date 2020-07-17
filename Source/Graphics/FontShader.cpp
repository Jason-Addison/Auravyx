#include "stdafx.h"
#include "FontShader.h"


FontShader::FontShader()
{
	shader = "Font";
}

FontShader::~FontShader()
{
}
void FontShader::loadAllUniformLocations()
{
	transformationMatrix = getUniformLocation((GLchar*) "transformationMatrix");
	colour = getUniformLocation((GLchar*) "colour");
	thickness = getUniformLocation((GLchar*) "thickness");
	mode = getUniformLocation((GLchar*) "mode");
}

void FontShader::loadColour(float r, float g, float b, float a)
{
	loadVec4f(colour, r, g, b, a);
}

void FontShader::loadColour(char colour)
{
	loadFloat(mode, colour);
}

void FontShader::loadThickness(float inner, float blur)
{
	loadVec2f(thickness, inner, blur);
}

void FontShader::loadTransformation(Matrix4f matrix)
{
	loadMatrix4f(transformationMatrix, matrix);
}
