#pragma once

#include "Shader/Shader.h"
class VoxelShader : public Shader
{
public:
	VoxelShader();
	~VoxelShader();

	GLint transformationMatrix;
	GLint projectionMatrix;
	GLint viewMatrix;
	GLint depthMVP;
	GLint damper;
	GLint shine;
	GLint time;
	GLint wind;

	GLint farPlane;
	GLint cameraPosition;

	void loadTransformation(Matrix4f matrix);

	void loadProjectionMatrix(Matrix4f matrix);

	void loadShadowDepthMVP(Matrix4f inDepthMVP);

	void loadCamera(Matrix4f matrix);

	void loadReflectivity(float shine, float damper);

	void loadCamera(float x, float y, float z, float farPlane);

	void loadWind(float power, float xDir, float zDir);

	void loadTime(double time);

protected:

	virtual void loadAllUniformLocations();

};

