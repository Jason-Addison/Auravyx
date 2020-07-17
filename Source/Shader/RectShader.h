#pragma once
#include "Shader.h"

class RectShader : public Shader
{
public:
	RectShader();
	~RectShader();

	GLint transformationMatrix;
	GLint colour;

	void loadTransformation(Matrix4f matrix);

	void loadColour(float r, float g, float b, float a);

	virtual void loadAllUniformLocations();

};

