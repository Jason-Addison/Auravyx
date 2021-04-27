#include "Auravyx/Graphics/Shader/VoxelShader.h"

VoxelShader::VoxelShader()
{
	shader = "Voxel";
}

VoxelShader::~VoxelShader()
{
}

void VoxelShader::loadAllUniformLocations()
{
	projectionMatrix = getUniformLocation("projectionMatrix");
	viewMatrix = getUniformLocation("viewMatrix");
	//depthMVP = getUniformLocation("depthMVP");
	shine = getUniformLocation("shine");
	damper = getUniformLocation("damper");
	cameraPosition = getUniformLocation("cameraPosition");
	farPlane = getUniformLocation("farPlane");
	loadTexture("terrainTexture", 0);
	loadTexture("shadowMap", 1);
	//bindAttribute(1, (GLchar*)"normalOut");
	//bindAttribute(10, "cameraVector");
}

void VoxelShader::loadProjectionMatrix(const Matrix4f& matrix)
{
	loadMatrix4f(projectionMatrix, matrix);
}

void VoxelShader::loadShadowDepthMVP(const Matrix4f& inDepthMVP)
{
	loadMatrix4f(depthMVP, inDepthMVP);
}

void VoxelShader::loadCamera(const Matrix4f& matrix)
{
	loadMatrix4f(viewMatrix, matrix);
}

void VoxelShader::loadReflectivity(const float shine, const float damper)
{
	loadFloat(this->shine, shine);
	loadFloat(this->damper, damper);
}

void VoxelShader::loadCamera(const float x, const float y, const float z, const float farPlane)
{
	loadVec3f(cameraPosition, x, y, z);
	loadFloat(this->farPlane, farPlane);
}
