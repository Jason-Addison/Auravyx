#include "stdafx.h"
#include "ShadowShader.h"
//#include "EntityModel.h"
//#include "Position.h"
//#include "Entity\Entity.h"
#include "ShadowMap.h"
//#include "Game.h"
#include <iostream>
ShadowShader::ShadowShader()
{
	shader = "Shadow";
}


ShadowShader::~ShadowShader()
{
}

void ShadowShader::loadAllUniformLocations()
{
	depthMVP = getUniformLocation("depthMVP");
	time = getUniformLocation("time");
	wind = getUniformLocation("wind");
	transformationMatrix = getUniformLocation("transformationMatrix");
}

void ShadowShader::loadDepthMVP(Matrix4f matrix)
{
	loadMatrix4f(depthMVP, matrix);
}
Matrix4f biasMatrix(
	0.5, 0.0, 0.0, 0.5,
	0.0, 0.5, 0.0, 0.5,
	0.0, 0.0, 0.5, 0.5,
	0.0, 0.0, 0.0, 1.0
);
void ShadowShader::render(World &world, ShadowMap &shadowMap, Camera &camera, Vec3f &position)
{
	start();
	Matrix4f depthMVP;
	Matrix4f viewMatrix;

	float xRot = position.x;// asin(position.x) * 180 / 3.14;
	float yRot = position.y;// acos(position.y) * 180 / 3.14;
	float zRot = position.z;// acos(position.z) * 180 / 3.14;

	viewMatrix.createViewMatrix((int) camera.getX(), (int) camera.getY(), (int) camera.getZ(), xRot, yRot, zRot);
	Matrix4f modelMatrix;
	Matrix4f projectionMatrix;
	float size = 400;
	projectionMatrix.createOrthographicMatrix(-size, size, -size, size, -size, size);

	depthMVP = projectionMatrix.multiply(viewMatrix);
	//depthMVP.multiply(modelMatrix);
	
	loadDepthMVP(depthMVP);
	Matrix4f swap;
	swap.set(biasMatrix);
	swap.multiply(depthMVP);
	shadowMap.depthMVP = swap;
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	/*//loadTime(Game::time);
	loadWind(1, 1, 1);*/
	/*for (int i = 0; i < world.entities.size(); i++)
	{
		std::shared_ptr<Entity> entity = world.entities.at(i);
		if (entity->contains<EntityModel>())
		{
			Model model = *entity->get<EntityModel>()->getModel().get();
			
			Matrix4f depthMVPT = depthMVP;
			Matrix4f transform = entity->get<Position>()->getAsMatrix();
			depthMVPT.multiply(transform);

			loadDepthMVP(depthMVPT);
			loadTransformation(transform);
			glBindVertexArray(model.getVAO());
			glEnableVertexArrayAttrib(model.getVAO(), 0);
			glEnableVertexArrayAttrib(model.getVAO(), 3);
			glDrawArrays(GL_TRIANGLES, 0, model.getCount());
		}
	}*/
	loadDepthMVP(depthMVP);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (auto c : world.overworld)
	{
		//glDisable(GL_CULL_FACE);
		if (c->ready)
		{
			c->render(camera, projectionMatrix);
		}
	}
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
}

void ShadowShader::loadWind(float power, float xDir, float zDir)
{
	loadVec3f(wind, power, xDir, zDir);
}

void ShadowShader::loadTime(double time)
{
	loadFloat(this->time, time);
}

void ShadowShader::loadTransformation(Matrix4f matrix)
{
	loadMatrix4f(transformationMatrix, matrix);
}