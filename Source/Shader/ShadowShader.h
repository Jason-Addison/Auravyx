#pragma once
#include "Shader.h"
#include "World\World.h"
#include "Camera.h"
#include "ShadowMap.h"

class ShadowShader : public Shader
{
public:
	ShadowShader();
	~ShadowShader();

	GLuint depthMVP;
	GLuint time;
	GLuint wind;
	GLuint transformationMatrix;

	void loadAllUniformLocations();

	void loadDepthMVP(Matrix4f matrix);

	void render(World &world, ShadowMap &shadowMap, Camera &camera, Vec3f &position);

	void loadWind(float power, float xDir, float zDir);

	void loadTime(double time);

	void loadTransformation(Matrix4f matrix);

};

