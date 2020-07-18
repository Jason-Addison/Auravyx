#include "SkyShader.h"
#include "Assets.h"


SkyShader::SkyShader()
{
	shader = "Sky";
}


SkyShader::~SkyShader()
{
}

void SkyShader::loadProjectionMatrix(Matrix4f matrix)
{
	loadMatrix4f(projectionMatrix, matrix);
}

void SkyShader::loadTransformationMatrix(Matrix4f matrix)
{
	loadMatrix4f(transformationMatrix, matrix);
}

void SkyShader::loadCamera(Matrix4f matrix)
{
	loadMatrix4f(viewMatrix, matrix);
}

void SkyShader::loadTime(double time)
{
	//std::cout << time << "\n";
	loadFloat(this->time, (float) time);
}

void SkyShader::loadSun(float x, float y, float z)
{
	loadVec3f(sun, x, y, z);
}

void SkyShader::loadScreenResolution(float x, float y)
{
	loadVec2f(screenResolution, x, y);
}

void SkyShader::render()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	Model mesh = *Assets::getModel("sky");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Assets::getTexture("stars")->texture);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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
