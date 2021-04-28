#include "Auravyx/Graphics/Shader/SkyShader.h"
#include "Auravyx/Core/Assets.h"

SkyShader::SkyShader()
{
	shader = "Sky";
}


SkyShader::~SkyShader()
{
}

void SkyShader::loadProjectionMatrix(const Matrix4f& matrix)
{
	loadMatrix4f(projectionMatrix, matrix);
}

void SkyShader::loadTransformationMatrix(const Matrix4f& matrix)
{
	loadMatrix4f(transformationMatrix, matrix);
}

void SkyShader::loadCamera(const Matrix4f& matrix)
{
	loadMatrix4f(viewMatrix, matrix);
}

void SkyShader::loadTime(const double time)
{
	loadFloat(this->time, (float) time);
}

void SkyShader::loadSun(const float x, const float y, const float z)
{
	loadVec3f(sun, x, y, z);
}

void SkyShader::loadScreenResolution(const float x, const float y)
{
	loadVec2f(screenResolution, x, y);
}

void SkyShader::render()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	Model mesh = *Assets::getAssets()->getAssets()->getModel("sky");
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Assets::getAssets()->getAssets()->getTexture("stars")->texture);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindVertexArray(mesh.getVAO());
	glEnableVertexArrayAttrib(mesh.getVAO(), 0);
	glEnableVertexArrayAttrib(mesh.getVAO(), 1);
	glEnableVertexArrayAttrib(mesh.getVAO(), 2);
	glDrawArrays(GL_TRIANGLES, 0, mesh.getCount());
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void SkyShader::loadAllUniformLocations()
{
	projectionMatrix = getUniformLocation("projectionMatrix");
	transformationMatrix = getUniformLocation("transformationMatrix");
	viewMatrix = getUniformLocation("viewMatrix");
	time = getUniformLocation("time");
	sun = getUniformLocation("sun");
	screenResolution = getUniformLocation("screenResolution");
}
