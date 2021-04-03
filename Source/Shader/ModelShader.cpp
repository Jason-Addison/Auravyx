#include "Shader/ModelShader.h"

ModelShader::ModelShader()
{
	shader = "Model";
}

ModelShader::~ModelShader()
{
}

void ModelShader::loadProjectionMatrix(const Matrix4f& matrix)
{
	loadMatrix4f(projectionMatrix, matrix);
}

void ModelShader::loadTransformationMatrix(const Matrix4f& matrix)
{
	loadMatrix4f(transformationMatrix, matrix);
}

void ModelShader::loadCamera(const Matrix4f& matrix)
{
	loadMatrix4f(viewMatrix, matrix);
}

void ModelShader::loadAllUniformLocations()
{
	projectionMatrix = getUniformLocation("projectionMatrix");
	transformationMatrix = getUniformLocation("transformationMatrix");
	viewMatrix = getUniformLocation("viewMatrix");
	cameraPosition = getUniformLocation("cameraPosition");
	farPlane = getUniformLocation("farPlane");
	diffuseColor = getUniformLocation("diffuseColor");
}
void ModelShader::loadCamera(const float x, const float y, const float z, const float farPlane)
{
	loadVec3f(cameraPosition, x, y, z);
	loadFloat(this->farPlane, farPlane);
}

void ModelShader::loadDiffuseColor(const float r, const float g, const float b, const float a)
{
	loadVec4f(diffuseColor, r, g, b, a);
}