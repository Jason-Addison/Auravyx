#pragma once
#include "Shader.h"
//#include "PointLight.h"
#include <vector>

class DeferredShader : public Shader
{
public:
	DeferredShader();
	~DeferredShader();

	GLint sunDirection;
	GLint normal;
	GLint farPlane;
	GLint cameraPosition;
	GLint viewMatrix;
	GLint depthMVP;
	GLint pointLightCount;
	GLint ambientLight;

	void loadTransformation(Matrix4f matrix);

	virtual void loadAllUniformLocations();

	void loadSunDirection(float x, float y, float z);

	void loadCamera(float x, float y, float z, float farPlane, Matrix4f viewMatrix);

	void loadDepthMVP(Matrix4f mat);

	//void loadPointLights(std::vector<PointLight> lights);

	void loadAmbientLight(float light, float r, float g, float b);
};

