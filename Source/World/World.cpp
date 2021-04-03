#include "stdafx.h"
#include "World/World.h"
#include "World/Chunk.h"
#include <Auravyx.h>
#include "Utilities/Profiler.h"
#include "Engine/WindowManager.h"
#include <Engine/Clock.h>
#include <Engine/Controller.h>
#include <Utilities/M.h>
#include <Shader/Shaders.h>
#include <Utilities/Assets.h>
#include "Physics/PhysicsSphere.h"
#include "Physics/PhysicsWorld.h"
#include <iostream>
#include <Auravyx.h>
#include <Utilities\Log.h>
FBO fbo;
std::vector<std::string> fboStrings;

std::shared_ptr<PhysicsSphere>s1(new PhysicsSphere());
std::shared_ptr<PhysicsSphere>s2(new PhysicsSphere());
PhysicsWorld physicsWorld;
World::World()
{
}


World::~World()
{
}

void World::generate()
{

}

void World::update()
{
	float speed = 500000;
	if (WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_EQUAL))
	{
		overworldTime += Clock::get(speed);
	}
	if (WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_MINUS))
	{
		overworldTime -= Clock::get(speed);
	}
	overworldTime += 1;
	physicsWorld.update();
}
ShadowMap shadowMap;
void World::create()
{
	fbo = FBO(WindowManager::getWindow()->getWidth(), WindowManager::getWindow()->getHeight());
	shadowMap = ShadowMap(2048 * 2, 2048 * 2);
}

void World::render(const Camera& cam, const Matrix4f& projectionMatrix)
{
	fbo.unbind();

	static float m = -40;
	
	m = fmod((double)(getOverworldTime() % getOverworldDayCycle()) * 0.0015, 360);
	shadowMap.bind();
	glDisable(GL_BLEND);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Vec3f dir(0, -90, -m);
	Renderer::getRenderer()->getShaders()->shadowShader->render(*this, shadowMap, cam, dir);
	
	shadowMap.unbind();
	fbo.update(WindowManager::getWindow()->getWidth(), WindowManager::getWindow()->getHeight());
	//fbo.checkStatus();
	//Log::out(std::to_string(fbo.checkStatus()));
	fbo.bind();
	fbo.clear();

	Renderer::getRenderer()->getShaders()->skyShader->start();
	Renderer::getRenderer()->getShaders()->skyShader->loadProjectionMatrix(projectionMatrix);
	Camera altCam = cam;
	altCam.x = 0; altCam.y = 0; altCam.z = 0;

	double p = 0.0174532925;

	Renderer::getRenderer()->getShaders()->skyShader->loadCamera(altCam.getViewMatrix());
	Renderer::getRenderer()->getShaders()->skyShader->loadSun(-cos(m * p), sin(m * p), 0);
	Renderer::getRenderer()->getShaders()->skyShader->loadTime((double)((getOverworldTime() + 60000) % getOverworldDayCycle()) / (double) getOverworldDayCycle());
	Renderer::getRenderer()->getShaders()->skyShader->loadScreenResolution(WindowManager::getWindow()->getWidth(), WindowManager::getWindow()->getHeight());
	double rot = (double)(getOverworldTime() % getOverworldDayCycle()) / (double)360;
	Matrix4f t = M::createTransformationMatrix(0, 0, 0, 1, 1, 1, 70, rot, 0);
	Renderer::getRenderer()->getShaders()->skyShader->loadTransformationMatrix(t);
	Renderer::getRenderer()->getShaders()->skyShader->render();
	if (Profiler::showChunkMetrics)
	{
		Renderer::getRenderer()->getShaders()->lineShader->start();
		Renderer::getRenderer()->getShaders()->lineShader->loadCamera(cam.getViewMatrix());
		Renderer::getRenderer()->getShaders()->lineShader->loadProjectionMatrix(projectionMatrix);
		Renderer::getRenderer()->getShaders()->lineShader->loadOffset(GFX::getOverlay()->CAM.cX, GFX::getOverlay()->CAM.cY, GFX::getOverlay()->CAM.cZ);
		Renderer::getRenderer()->getShaders()->lineShader->render();
	}
	Renderer::getRenderer()->getShaders()->lineShader->stop();

	Renderer::getRenderer()->getShaders()->voxelShader->start();
	Renderer::getRenderer()->getShaders()->voxelShader->loadProjectionMatrix(projectionMatrix); // *projectionMatrix);
	Renderer::getRenderer()->getShaders()->voxelShader->loadCamera(cam.getViewMatrix());
	Renderer::getRenderer()->getShaders()->voxelShader->loadCamera(cam.xPos, cam.yPos, cam.zPos, GFX::getOverlay()->viewDistance * 64);

	std::vector<int> toRemove;
	int distance;
	/*for (int i = 0; i < overworld.size(); i++)
	{
		distance = abs(cam->cX - overworld.at(i)->x) + abs(cam->cY - overworld.at(i)->y) + abs(cam->cZ - overworld.at(i)->z);
		if (distance <= GFX::getOverlay()->viewDistance * 2)
		{
			if (overworld.at(i) != nullptr)
			{
				overworld.at(i)->render(*cam, *projectionMatrix);
			}
		}
		else
		{
			//overworld.at(i)->destroy();
			//overworld.erase(overworld.begin() + i);
			//toRemove.emplace_back(i);
			//i--;
			toRemove.emplace_back(i);
		}
	}
	for (auto i : toRemove)
	{
		overworld.erase(overworld.begin() + i);
	}*/
	/*if (unloadLock)
	{
		/*for (auto it = overworld.begin(); it != overworld.end();)
		{
			distance = abs(cam->cX - (*it)->x) + abs(cam->cZ - (*it)->z);
			//distance = 10000;
			/*if (distance <= GFX::getOverlay()->viewDistance * 2)
			{
				if ((*it) != nullptr)
				{
					(*it)->render(*cam, *projectionMatrix);
				}
				++it;
			}
			if (distance > GFX::getOverlay()->viewDistance * 2 && (*it) != nullptr && (*it)->loaded)
			{
				//std::shared_ptr<Chunk> e = (*it);
				//(*it)->destroy();
				it = overworld.erase(it);
				//++it;
				//overworld.clear();
			}
			else if (distance <= GFX::getOverlay()->viewDistance * 2)
			{
				++it;
			}
		}*/
		/*for (int i = 0; i < overworld.size();)
		{
			distance = abs(cam->cX - overworld.at(i)->x) + abs(cam->cZ - overworld.at(i)->z);
			if (distance > GFX::getOverlay()->viewDistance * 10000.2)
			{
				//////////////////////overworld.at(i)->destroy();
				overworld.erase(overworld.begin() + i);
			}
			else
			{
				if (overworld.at(i)->chunkUpdate)
				{
					overworld.at(i)->refresh();
					overworld.at(i)->chunkUpdate = false;
				}
				//overworld.at(i)->render(*cam, *projectionMatrix);
				i++;
			}
		}
		unloadLock = false;
	}*/

	if (WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_R))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, GFX::getOverlay()->terrainMaterials);
	
	for (auto &c : overworld)
	{
		if (c != nullptr && c->ready)
		{
			c->render();
		}
	}
	Renderer::getRenderer()->getShaders()->voxelShader->stop();
	//GFX::getOverlay()->renderModel(GFX::getOverlay()->CAM.xPos, GFX::getOverlay()->CAM.yPos, GFX::getOverlay()->CAM.zPos,
	//	1, 1, 1, -90, 0, (GFX::getOverlay()->CAM.yRot + 180) * 0, Assets::getAssets()->getAssets()->getModel("model").get(),
	//	&(GFX::getOverlay()->CAM), projectionMatrix, Assets::getAssets()->getAssets()->getTexture("mod").get());

		GFX::getOverlay()->renderModelIndex(GFX::getOverlay()->CAM.xPos, GFX::getOverlay()->CAM.yPos, GFX::getOverlay()->CAM.zPos,
			1, 1, 1, 0, 90, (GFX::getOverlay()->CAM.yRot + 180) * 0, *Assets::getAssets()->getAssets()->getModel("mesh").get(),
			(GFX::getOverlay()->CAM), projectionMatrix, *Assets::getAssets()->getAssets()->getTexture("face").get());
	
		GFX::getOverlay()->renderModelIndex(0, 0, 0,
			1, 1, 1, 0, 0, (0 + 180) * 0, *Assets::getAssets()->getAssets()->getModel("mesh").get(),
			(GFX::getOverlay()->CAM), projectionMatrix, *Assets::getAssets()->getAssets()->getTexture("face").get());
	//s1.setPosition(GFX::getOverlay()->CAM.x, GFX::getOverlay()->CAM.y - 5, GFX::getOverlay()->CAM.z);
	
	GFX::getOverlay()->renderModel(s1->getX(), s1->getY(), s1->getZ(), 1, 1, 1, 0, 0, 0, *Assets::getAssets()->getAssets()->getModel("sky").get(),
		(GFX::getOverlay()->CAM), projectionMatrix, *Assets::getAssets()->getAssets()->getTexture("light_blue").get());
	//if (s1->checkCollision(*s2.get()))
	{
		//GFX::getOverlay()->renderModel(0, 0, 0, 1, 1, 1, 0, 0, 0, Assets::getAssets()->getAssets()->getModel("sky").get(), &GFX::getOverlay()->CAM, projectionMatrix, Assets::getAssets()->getAssets()->getTexture("yellow").get());
	}
	//else
	{
		//GFX::getOverlay()->renderModel(0, 0, 0, 1, 1, 1, 0, 0, 0, Assets::getAssets()->getAssets()->getModel("sky").get(), &GFX::getOverlay()->CAM, projectionMatrix, Assets::getAssets()->getAssets()->getTexture("light_blue").get());
	}

	fbo.unbind();
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GFX::getOverlay()->drawImage(0, 0, WindowManager::getWindow()->getWidth(), WindowManager::getWindow()->getHeight(), fbo.texture);
	
	Renderer::getRenderer()->getShaders()->deferredShader->start();

	Renderer::getRenderer()->getShaders()->deferredShader->loadDepthMVP(shadowMap.depthMVP);
	//Renderer::getRenderer()->getShaders()->deferredShader->loadPointLights(lights);
	Renderer::getRenderer()->getShaders()->deferredShader->loadSunDirection(cos(M::toRadians(m)), -sin(M::toRadians(m)), 0);
	Renderer::getRenderer()->getShaders()->deferredShader->loadCamera(cam.xPos, cam.yPos, cam.zPos, GFX::getOverlay()->viewDistance * 64, cam.getViewMatrix());
	double lightFactor = (1 - 1) * 0.9 + 0.1;
	   
	double step = (double)(getOverworldTime() % getOverworldDayCycle()) / (double)getOverworldDayCycle();
	double brightness = sin((double)(getOverworldTime() % getOverworldDayCycle()) / (double)getOverworldDayCycle() * 3.14 * 6);
	if (step > (1.0 / 12.0) && step < 5.0 / 12.0)
	{
		brightness = 1;
	}
	else if (step > 7.0 / 12.0 && step < 11.0 / 12.0)
	{
		brightness = -1;
	}
	Renderer::getRenderer()->getShaders()->deferredShader->loadAmbientLight((brightness + 1 + GFX::getOverlay()->brightness) / (2 + GFX::getOverlay()->brightness), 1, 1, 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo.buffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fbo.buffers[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, fbo.buffers[2]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, fbo.buffers[3]);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, fbo.buffers[4]);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, shadowMap.depthTexture);

	glBindVertexArray(GFX::getOverlay()->quad.getVAO());
	glEnableVertexArrayAttrib(GFX::getOverlay()->quad.getVAO(), 0);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 3);


	int width = WindowManager::getWindow()->getWidth() / 6;
	int height = (int) (width * ((float) WindowManager::getWindow()->getHeight() / (float) WindowManager::getWindow()->getWidth()));
	
	if (Profiler::showAdvancedDebugInfo)
	{
		GFX::getOverlay()->fillRect(0, WindowManager::getWindow()->getHeight() - height - 1, WindowManager::getWindow()->getWidth(), 1, 0, 0, 0, 1);
		for (int i = 0; i < 5; i++)
		{
			GFX::getOverlay()->drawImage(width * i, 0, width, height, fbo.buffers[i]);
			GFX::getOverlay()->drawStringBG(fboStrings.at(i), i * width, WindowManager::getWindow()->getHeight() - height, WindowManager::getWindow()->getWidth() / 80,
				1, 1, 1, 1, 0, 0, 0, WindowManager::getWindow()->getWidth() / 300 - 10, 0, 0, 0, 0.3);
		}
		GFX::getOverlay()->drawImage(width * 5, 0, width, height, shadowMap.depthTexture);
		GFX::getOverlay()->drawImage(width * 5, height, width, height, Assets::getAssets()->getAssets()->getTexture("font_plain")->texture);
		GFX::getOverlay()->drawStringBG("shadow", width * 5, WindowManager::getWindow()->getHeight() - height, WindowManager::getWindow()->getWidth() / 80,
			1, 1, 1, 1, 0, 0, 0, WindowManager::getWindow()->getWidth() / 300 - 10, 0, 0, 0, 0.3);
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
bool nx;
bool ny;
int xP;
int yP;

void World::addChunk(const Chunk& chunk)
{
	//if (getChunk(chunk->x, chunk->y) == nullptr)
	{
		overworld.emplace_back(new Chunk(chunk));
		Chunk* newChunk = overworld.at(overworld.size() - 1).get();
		Chunk* c = nullptr;
		for (int x = 0; x < 3; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				for (int z = 0; z < 3; z++)
				{
					c = getChunk(chunk.x + x - 1, chunk.y + y - 1, chunk.z + z - 1);
					if (c != nullptr)
					{
						c->addNeighbour(newChunk, c);
					}
				}
			}
		}
	}
}

Chunk* World::getChunk(const int x, const int y, const int z)
{
	for (auto &c : overworld)
	{
		if (c != nullptr && c->x == x && c->y == y && c->z == z)
		{
			return c.get();
		}
	}
	return nullptr;
}

std::shared_ptr<ChunkHeight> World::getChunkHeightmap(const int x, const int z)
{
	for (auto c : overworldHeightmap)
	{
		if (c != nullptr && c->x == x && c->z == z)
		{
			return c;
		}
	}
	return nullptr;
}

bool World::isLoaded(const int x, const int y, const int z)
{
	return getChunk(x, y, z) != nullptr;
}

std::shared_ptr<World> World::getOverworld()
{
	return nullptr;
}

void World::setVoxel(const int x, const int y, const int z, const int type, const float density)
{
	Chunk* c = getChunk(((float) x / (float) c->CHUNK_SIZE) * 2, (float)y / (float)c->CHUNK_SIZE * 2, (float)z / (float) c->CHUNK_SIZE * 2);
	if (c != nullptr)
	{

	}
}

void World::setOverworldTime(const long long int time)
{
	this->overworldTime = time;
}

long long int World::getOverworldTime()
{
	return this->overworldTime;
}

long long int World::getOverworldDayCycle()
{
	return this->overworldDayCycle;
}

void World::test()
{
	//physicsWorld.addObject(s1);
	//s1->addImpulse(0, 1, 0);
	//s1->setPosition(50, 0, 50);
	//s2->setPosition(20, 0, 10);
	//physicsWorld.addObject(s2);
}

Chunk* World::getChunk(int i)
{
	return overworld.at(i).get();
}

void World::setup()
{
	fboStrings = std::vector<std::string>();
	overworld = std::vector<std::unique_ptr<Chunk>>();
	fboStrings.emplace_back("albedo");
	fboStrings.emplace_back("normal");
	fboStrings.emplace_back("spec / light");
	fboStrings.emplace_back("glow");
	fboStrings.emplace_back("position");
	fboStrings.emplace_back("shadow");
	fboStrings.emplace_back("glow");
	fboStrings.emplace_back("-");
	fboStrings.emplace_back("-");
	fboStrings.emplace_back("-");
	fboStrings.emplace_back("position");
}

void World::sphere(const float xP, const float yP, const float zP, const float radius, const float power)
{
	Chunk* c = nullptr;
	for (int x = floor(-radius / 2); x < ceil(radius / 2); x++)
	{
		for (int z = floor(-radius / 2); z < ceil(radius / 2); z++)
		{
			for (int y = floor(-radius / 2); y < ceil(radius / 2); y++)
			{
				int nX = x + xP;
				int nY = y + yP;
				int nZ = z + zP;
				float distance = abs(M::distance(Vec3f(xP, yP, zP), Vec3f(nX, nY, nZ)));

				if (distance < radius / 2)
				{
					c = getChunk(GFX::getOverlay()->CAM.cX, GFX::getOverlay()->CAM.cY, GFX::getOverlay()->CAM.cZ);
					
					if (c != nullptr)
					{
						//if (nX >= 0 && nY >= 0 && nZ >= 0 &&
						//	nX < size && nY < size && nZ < size)
						{
							c->setVoxel(nX, nY, nZ, c->getDensity(nX, nY, nZ) + power * 0.01 * ((radius - distance) / radius), 255);
							if (c->getDensity(nX, nY, nZ) > 1)
							{
								c->setVoxel(nX, nY, nZ, 1, 255);
							}
							else if (c->getDensity(nX, nY, nZ) < 0)
							{
								c->setVoxel(nX, nY, nZ, 0, 255);
							}
							if (c->getDensity(nX, nY, nZ) < 0.1 && power < 0)
							{
								c->setVoxel(nX, nY, nZ, 0, 255);
							}
						}
					}
					c = nullptr;
				}
			}
		}
	}
}

void World::cuboid(float xP, float yP, float zP, float xS, float yS, float zS, float power)
{
	std::shared_ptr<Chunk> c = nullptr;
	for (int x = xP - xS; x < xP + xS; x++)
	{
		for (int z = zP - zS; z < zP + zS; z++)
		{
			for (int y = yP - yS; y < yP + yS; y++)
			{
				setVoxel(x, y, z, 1, 1);
			}
		}
	}
}
