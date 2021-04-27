#pragma once
#include "Shader/Shader.h"
#include <vector>

class DeferredShader : public Shader
{
public:
	DeferredShader();
	~DeferredShader();

	virtual void loadAllUniformLocations();

	void loadSunDirection(const float x, const float y, const float z);

	void loadCamera(const float x, const float y, const float z, const float farPlane, const Matrix4f& viewMatrix);

	void loadDepthMVP(const Matrix4f& mat);

	//void loadPointLights(const std::vector<PointLight>& lights);

	void loadAmbientLight(const float light, const float r, const float g, const float b);

private:

	GLint sunDirection;
	GLint normal;
	GLint farPlane;
	GLint cameraPosition;
	GLint viewMatrix;
	GLint depthMVP;
	GLint pointLightCount;
	GLint ambientLight;
};

