#include "stdafx.h"
#include "Graphics/FontShader.h"


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

void FontShader::loadColour(const float r, const float g, const float b, const float a)
{
	loadVec4f(colour, r, g, b, a);
}

void FontShader::loadColour(const char colour)
{
	loadFloat(mode, colour);
}

void FontShader::loadThickness(const float inner, const float blur)
{
	loadVec2f(thickness, inner, blur);
}

void FontShader::loadTransformation(const Matrix4f& matrix)
{
	loadMatrix4f(transformationMatrix, matrix);
}
