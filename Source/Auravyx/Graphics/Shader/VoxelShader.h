#pragma once

#include "Auravyx/Graphics/Shader/Shader.h"
class VoxelShader : public Shader
{
public:
	VoxelShader();
	~VoxelShader();

	void loadTransformation(const Matrix4f& matrix);

	void loadProjectionMatrix(const Matrix4f& matrix);

	void loadShadowDepthMVP(const Matrix4f& inDepthMVP);

	void loadCamera(const Matrix4f& matrix);

	void loadReflectivity(const float shine, const float damper);

	void loadCamera(const float x, const float y, const float z, const float farPlane);

	//void loadWind(const float power, const float xDir, const float zDir);

	//void loadTime(const double time);

protected:

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

	virtual void loadAllUniformLocations();

};

