#pragma once
#include "Shader/Shader.h"
#include "Engine/Model.h"
#include "Engine/FBO.h"
class LineShader : public Shader
{
public:
	LineShader();
	~LineShader();

	void loadProjectionMatrix(const Matrix4f& matrix);

	void loadOffset(const float x, const float y, const float z);

	void loadCamera(const Matrix4f& matrix);

	void render();

	void renderPoint(const float x, const float y, const float z, const Matrix4f& projectionMatrix);

	void init();

protected:

	virtual void loadAllUniformLocations();

	GLint projectionMatrix;
	GLint viewMatrix;
	GLint offset;

	Model model;
	Model point;
};

