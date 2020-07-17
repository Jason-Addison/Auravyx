#pragma once
#include "Shader.h"
class FontShader : public Shader
{
public:
	FontShader();
	~FontShader();

	GLint transformationMatrix;
	GLint colour;
	GLint thickness;
	GLint mode;

	void loadTransformation(Matrix4f matrix);

	void loadColour(float r, float g, float b, float a);

	void loadColour(char colour);

	void loadThickness(float inner, float blur);

	virtual void loadAllUniformLocations();
};

