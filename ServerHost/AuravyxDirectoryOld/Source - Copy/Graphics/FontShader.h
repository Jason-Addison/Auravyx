#pragma once
#include "Shader/Shader.h"
class FontShader : public Shader
{
public:
	FontShader();
	~FontShader();

	void loadTransformation(const Matrix4f& matrix);

	void loadColour(const float r, const float g, const float b, const float a);

	void loadColour(const char colour);

	void loadThickness(const float inner, const float blur);

	virtual void loadAllUniformLocations();

private:

	GLint transformationMatrix;
	GLint colour;
	GLint thickness;
	GLint mode;
};

