#pragma once
#include "Auravyx/Graphics/Shader/Shader.h"

class RectShader : public Shader
{
public:
	RectShader();
	~RectShader();

	void loadTransformation(const Matrix4f& matrix);

	void loadColour(const float r, const float g, const float b, const float a);

	virtual void loadAllUniformLocations();

protected:

	GLint transformationMatrix;
	GLint colour;
};

