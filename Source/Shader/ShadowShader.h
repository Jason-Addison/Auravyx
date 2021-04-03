#pragma once
#include "Shader/Shader.h"
#include "World/World.h"
#include "Engine/Camera.h"
#include "Shader/ShadowMap.h"

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

	void loadDepthMVP(const Matrix4f& matrix);

	void render(const World &world, ShadowMap &shadowMap, const Camera &camera, const Vec3f &position);

	void loadWind(const float power, const float xDir, const float zDir);

	void loadTime(const double time);

	void loadTransformation(const Matrix4f& matrix);

};

