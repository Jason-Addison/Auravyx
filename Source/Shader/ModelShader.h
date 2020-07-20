#pragma once
#include "Shader.h"
class ModelShader : public Shader
{
public:

	ModelShader();
	~ModelShader();

	GLint projectionMatrix;
	GLint viewMatrix;
	GLint transformationMatrix;

	void loadProjectionMatrix(Matrix4f matrix);

	void loadTransformationMatrix(Matrix4f matrix);

	void loadCamera(Matrix4f matrix);

protected:

	virtual void loadAllUniformLocations();
};

