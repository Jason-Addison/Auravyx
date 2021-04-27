#pragma once
#include "Shader/Shader.h"

class ImageShader : public Shader
{
public:
	ImageShader();
	~ImageShader();

	void loadTransformation(const Matrix4f& matrix);

	virtual void loadAllUniformLocations();

private:
	GLint transformationMatrix;
};

