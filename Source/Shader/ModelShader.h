#pragma once
#include "Shader/Shader.h"
class ModelShader : public Shader
{
public:

	ModelShader();
	~ModelShader();

	void loadProjectionMatrix(const Matrix4f& matrix);

	void loadTransformationMatrix(const Matrix4f& matrix);

	void loadCamera(const Matrix4f& matrix);

	void loadCamera(const float x, const float y, const float z, const float farPlane);

	void loadDiffuseColor(const float r, const float g, const float b, const float a);

protected:

	GLint projectionMatrix;
	GLint viewMatrix;
	GLint transformationMatrix;
	GLint cameraPosition;
	GLint farPlane;
	GLint diffuseColor;

	virtual void loadAllUniformLocations();
};

