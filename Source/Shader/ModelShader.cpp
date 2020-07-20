#include "ModelShader.h"

ModelShader::ModelShader()
{
	shader = "Model";
}

ModelShader::~ModelShader()
{
}

void ModelShader::loadProjectionMatrix(Matrix4f matrix)
{
	loadMatrix4f(projectionMatrix, matrix);
}

void ModelShader::loadTransformationMatrix(Matrix4f matrix)
{
	loadMatrix4f(transformationMatrix, matrix);
}

void ModelShader::loadCamera(Matrix4f matrix)
{
	loadMatrix4f(viewMatrix, matrix);
}

void ModelShader::loadAllUniformLocations()
{
	projectionMatrix = getUniformLocation("projectionMatrix");
	transformationMatrix = getUniformLocation("transformationMatrix");
	viewMatrix = getUniformLocation("viewMatrix");
}