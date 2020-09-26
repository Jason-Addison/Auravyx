#pragma once
#include "Shader/Shader.h"

class ImageShader : public Shader
{
public:
	ImageShader();
	~ImageShader();

	GLint transformationMatrix;

	void loadTransformation(Matrix4f matrix);

	virtual void loadAllUniformLocations();

};

