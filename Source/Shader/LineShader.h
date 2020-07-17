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

	void loadProjectionMatrix(Matrix4f matrix);

	void loadOffset(float x, float y, float z);

	void loadCamera(Matrix4f matrix);

	void render(FBO f);

	void init();

protected:

	virtual void loadAllUniformLocations();
};

