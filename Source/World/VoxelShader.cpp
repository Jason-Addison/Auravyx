#include "stdafx.h"
#include "VoxelShader.h"


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

void VoxelShader::loadProjectionMatrix(Matrix4f matrix)
{
	loadMatrix4f(projectionMatrix, matrix);
}

void VoxelShader::loadShadowDepthMVP(Matrix4f inDepthMVP)
{
	loadMatrix4f(depthMVP, inDepthMVP);
}

void VoxelShader::loadCamera(Matrix4f matrix)
{
	loadMatrix4f(viewMatrix, matrix);
}

void VoxelShader::loadReflectivity(float shine, float damper)
{
	loadFloat(this->shine, shine);
	loadFloat(this->damper, damper);
}

void VoxelShader::loadCamera(float x, float y, float z, float farPlane)
{
	loadVec3f(cameraPosition, x, y, z);
	loadFloat(this->farPlane, farPlane);
}
