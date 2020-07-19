#pragma once
#include "Shader.h"
#include "Model.h"
#include "FBO.h"
class LineShader : public Shader
{
public:
	LineShader();
	~LineShader();

	GLint projectionMatrix;
	GLint viewMatrix;
	GLint offset;

	Model model;

	Model point;

	void loadProjectionMatrix(Matrix4f matrix);

	void loadOffset(float x, float y, float z);

	void loadCamera(Matrix4f matrix);

	void render();

	void renderPoint(float x, float y, float z, Matrix4f projectionMatrix);

	void init();

protected:

	virtual void loadAllUniformLocations();
};

