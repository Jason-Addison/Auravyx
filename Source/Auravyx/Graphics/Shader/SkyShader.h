#pragma once
#include "Auravyx/Graphics/Shader/Shader.h"
class SkyShader : public Shader
{
public:
	SkyShader();
	~SkyShader();

	void loadProjectionMatrix(const Matrix4f& matrix);

	void loadTransformationMatrix(const Matrix4f& matrix);

	void loadCamera(const Matrix4f& matrix);

	void loadTime(const double time);

	void loadSun(const float x, const float y, const float z);

	void loadScreenResolution(const float x, const float y);

	void render();

protected:

	GLint projectionMatrix;
	GLint viewMatrix;
	GLint time;
	GLint sun;
	GLint screenResolution;
	GLint transformationMatrix;

	virtual void loadAllUniformLocations();
};

