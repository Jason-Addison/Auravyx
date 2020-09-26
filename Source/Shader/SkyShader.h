#pragma once
#include "Shader/Shader.h"
class SkyShader : public Shader
{
public:
	SkyShader();
	~SkyShader();

	GLint projectionMatrix;
	GLint viewMatrix;
	GLint time;
	GLint sun;
	GLint screenResolution;
	GLint transformationMatrix;

	void loadProjectionMatrix(Matrix4f matrix);

	void loadTransformationMatrix(Matrix4f matrix);

	void loadCamera(Matrix4f matrix);

	void loadTime(double time);

	void loadSun(float x, float y, float z);

	void loadScreenResolution(float x, float y);

	void render();

protected:

	virtual void loadAllUniformLocations();
};

