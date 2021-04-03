#include "stdafx.h"
#include "Shader/ShadowShader.h"
//#include "EntityEngine/Model.h"
//#include "Position.h"
//#include "Entity\Engine/Entity.h"
#include "Shader/ShadowMap.h"
//#include "Game.h"
#include <iostream>
#include <Graphics/GFX.h>
#include <Utilities/Assets.h>
#include <Utilities\M.h>
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

void ShadowShader::loadDepthMVP(const Matrix4f& matrix)
{
	loadMatrix4f(depthMVP, matrix);
}
Matrix4f biasMatrix(
	0.5, 0.0, 0.0, 0.5,
	0.0, 0.5, 0.0, 0.5,
	0.0, 0.0, 0.5, 0.5,
	0.0, 0.0, 0.0, 1.0
);
void ShadowShader::render(const World &world, ShadowMap &shadowMap, const Camera &camera, const Vec3f &position)
{
	start();
	Matrix4f depthMVP;
	Matrix4f viewMatrix;

	float xRot = position.x;// asin(position.x) * 180 / 3.14;
	float yRot = position.y;// acos(position.y) * 180 / 3.14;
	float zRot = position.z;// acos(position.z) * 180 / 3.14;

	viewMatrix.createViewMatrix((int) camera.xPos, (int) camera.yPos, (int) camera.zPos, xRot, yRot, zRot);
	Matrix4f modelMatrix;
	Matrix4f projectionMatrix;
	float size = 100;
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

	Matrix4f iMatrix;
	iMatrix.setIdentity();
	loadTransformation(iMatrix);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	for (auto &c : world.overworld)
	{
		if (c->ready)
		{
			c->render();
		}
	}
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);

	std::shared_ptr<Model> m = Assets::getAssets()->getAssets()->getModel("mesh");
	glBindVertexArray(m->getVAO());
	glEnableVertexArrayAttrib(m->getVAO(), 0);

	Matrix4f matrix = M::createTransformationMatrix(GFX::getOverlay()->CAM.xPos, GFX::getOverlay()->CAM.yPos, GFX::getOverlay()->CAM.zPos,
		1, 1, 1, 0, 90, (GFX::getOverlay()->CAM.yRot + 180) * 0);
	loadTransformation(matrix);
	
	glDrawElements(GL_TRIANGLES, m->indexCount, GL_UNSIGNED_INT, 0);
	

	//GFX::getOverlay()->renderModelIndex(GFX::getOverlay()->CAM.xPos, GFX::getOverlay()->CAM.yPos, GFX::getOverlay()->CAM.zPos,
	//		1, 1, 1, 0, 0, (GFX::getOverlay()->CAM.yRot + 180) * 0, Assets::getAssets()->getAssets()->getModel("mesh").get(),
	//		&(GFX::getOverlay()->CAM), projectionMatrix, Assets::getAssets()->getAssets()->getTexture("face").get());


	//GFX::getOverlay()->renderModel(GFX::getOverlay()->CAM.xPos, GFX::getOverlay()->CAM.yPos, GFX::getOverlay()->CAM.zPos, 1, 1, 1, -90, 0, (GFX::getOverlay()->CAM.yRot + 180) * 0, Assets::getAssets()->getAssets()->getModel("model").get(), &(GFX::getOverlay()->CAM), projectionMatrix, Assets::getAssets()->getAssets()->getTexture("mod").get());
}

void ShadowShader::loadWind(const float power, const float xDir, const float zDir)
{
	loadVec3f(wind, power, xDir, zDir);
}

void ShadowShader::loadTime(const double time)
{
	loadFloat(this->time, time);
}

void ShadowShader::loadTransformation(const Matrix4f& matrix)
{
	loadMatrix4f(transformationMatrix, matrix);
}