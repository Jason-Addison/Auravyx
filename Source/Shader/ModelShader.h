#pragma once
#include "Shader/Shader.h"
class ModelShader : public Shader
{
public:

	ModelShader();
	~ModelShader();

	GLint projectionMatrix;
	GLint viewMatrix;
	GLint transformationMatrix;
	GLint cameraPosition;
	GLint farPlane;

	void loadProjectionMatrix(Matrix4f matrix);

	void loadTransformationMatrix(Matrix4f matrix);

	void loadCamera(Matrix4f matrix);

	void loadCamera(float x, float y, float z, float farPlane);

protected:

	virtual void loadAllUniformLocations();
};

