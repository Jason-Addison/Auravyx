#include "stdafx.h"
#include "World.h"
#include "Chunk.h"
#include "GFX.h"
#include "Profiler.h"
#include "WindowManager.h"
#include <Clock.h>
#include <Controller.h>
#include <M.h>
#include <Shaders.h>
FBO fbo;

std::vector<std::string> fboStrings;

World::World()
{
	fboStrings.emplace_back("albedo");
	fboStrings.emplace_back("normal");
	fboStrings.emplace_back("specular");
	fboStrings.emplace_back("glow");
	//fboStrings.emplace_back("-");
	//fboStrings.emplace_back("-");
	//fboStrings.emplace_back("-");
	fboStrings.emplace_back("position");
}


World::~World()
{
}

void World::generate()
{

}

void World::update()
{
	float speed = 50000;
	if (Controller::isKeyDown(GLFW_KEY_EQUAL))
	{
		overworldTime += Clock::get(speed);
	}
	if (Controller::isKeyDown(GLFW_KEY_MINUS))
	{
		overworldTime -= Clock::get(speed);
	}
	overworldTime += 1;
}
ShadowMap shadowMap;
void World::create()
{
	fbo = FBO(WindowManager::width, WindowManager::height);
	shadowMap = ShadowMap(2048 * 2, 2048 * 2);
}
void World::render(Camera* cam, Matrix4f* projectionMatrix)
{
	fbo.unbind();

	shadowMap.bind();
	glDisable(GL_BLEND);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	static float m = -40;
	
	m = fmod((double)(getOverworldTime() % getOverworldDayCycle()) * 0.0015, 360);
	
	Vec3f dir(0, -90, -m);
	Shaders::shadowShader->render(*this, shadowMap, *cam, dir);
	
	shadowMap.unbind();
	fbo.update(WindowManager::width, WindowManager::height);
	fbo.bind();
	fbo.clear();

	Shaders::skyShader->start();
	Shaders::skyShader->loadProjectionMatrix(*projectionMatrix);
	Camera altCam = *cam;
	altCam.x = 0; altCam.y = 0; altCam.z = 0;

	float p = 0.0174532925;

	Shaders::skyShader->loadCamera(altCam.getViewMatrix());
	Shaders::skyShader->loadSun(-cos(m * p), sin(m * p), 0);
	Shaders::skyShader->loadTime((double)((getOverworldTime() + 60000) % getOverworldDayCycle()) / (double) getOverworldDayCycle());
	Shaders::skyShader->loadScreenResolution(WindowManager::getWidth(), WindowManager::getHeight());
	double rot = (double)(getOverworldTime() % getOverworldDayCycle()) / (double)360;
	Matrix4f t = M::createTransformationMatrix(0, 0, 0, 1, 1, 1, 70, rot, 0);
	Shaders::skyShader->loadTransformationMatrix(t);
	Shaders::skyShader->render();
	if (Profiler::showChunkMetrics)
	{
		Shaders::lineShader->start();
		Shaders::lineShader->loadCamera(cam->getViewMatrix());
		Shaders::lineShader->loadProjectionMatrix(*projectionMatrix);
		Shaders::lineShader->loadOffset(GFX::CAM.cX, GFX::CAM.cY, GFX::CAM.cZ);
		Shaders::lineShader->render(fbo);
	}

	Shaders::voxelShader->start();
	Shaders::voxelShader->loadProjectionMatrix(*projectionMatrix); // *projectionMatrix);
	Shaders::voxelShader->loadCamera(cam->getViewMatrix());
	Shaders::voxelShader->loadCamera(cam->x, cam->y, cam->z, GFX::viewDistance * 100);

	std::vector<int> toRemove;
	int distance;
	/*for (int i = 0; i < overworld.size(); i++)
	{
		distance = abs(cam->cX - overworld.at(i)->x) + abs(cam->cY - overworld.at(i)->y) + abs(cam->cZ - overworld.at(i)->z);
		if (distance <= GFX::viewDistance * 2)
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
			/*if (distance <= GFX::viewDistance * 2)
			{
				if ((*it) != nullptr)
				{
					(*it)->render(*cam, *projectionMatrix);
				}
				++it;
			}
			if (distance > GFX::viewDistance * 2 && (*it) != nullptr && (*it)->loaded)
			{
				//std::shared_ptr<Chunk> e = (*it);
				//(*it)->destroy();
				it = overworld.erase(it);
				//++it;
				//overworld.clear();
			}
			else if (distance <= GFX::viewDistance * 2)
			{
				++it;
			}
		}*/
		/*for (int i = 0; i < overworld.size();)
		{
			distance = abs(cam->cX - overworld.at(i)->x) + abs(cam->cZ - overworld.at(i)->z);
			if (distance > GFX::viewDistance * 10000.2)
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

	if (Controller::isKeyDown(GLFW_KEY_R))
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
	glBindTexture(GL_TEXTURE_2D_ARRAY, GFX::terrainMaterials);
	
	for (auto c : overworld)
	{
		if (c != nullptr && c->ready)
		{
			c->render(GFX::CAM, *projectionMatrix);
		}
	}
	Shaders::voxelShader->stop();
	fbo.unbind();
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GFX::drawImage(0, 0, WindowManager::width, WindowManager::height, fbo.texture);
	
	Shaders::deferredShader->start();

	Shaders::deferredShader->loadDepthMVP(shadowMap.depthMVP);
	//Shaders::deferredShader->loadPointLights(lights);
	//float amt = sin(M::toRadians(-45));
	Shaders::deferredShader->loadSunDirection(cos(M::toRadians(m)), -sin(M::toRadians(m)), 0);
	Shaders::deferredShader->loadCamera(cam->x, cam->y, cam->z, GFX::viewDistance * 100, cam->getViewMatrix());
	
	float lightFactor = (1 - 1) * 0.9 + 0.1;
	   
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
	Shaders::deferredShader->loadAmbientLight((brightness + 1 + GFX::brightness) / (2 + GFX::brightness), 1, 1, 1);
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
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, fbo.buffers[1]);
	glBindVertexArray(GFX::quad.getVAO());
	glEnableVertexArrayAttrib(GFX::quad.getVAO(), 0);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 3);


	int width = WindowManager::width / 6;
	int height = width * ((float) WindowManager::height / (float) WindowManager::width);
	
	if (Profiler::showAdvancedDebugInfo)
	{
		GFX::fillRect(0, WindowManager::height - height - 1, WindowManager::width, 1, 0, 0, 0, 1);
		for (int i = 0; i < 5; i++)
		{
			GFX::drawImage(width * i, 0, width, height, fbo.buffers[i]);
			GFX::drawStringBG(fboStrings.at(i), i * width, WindowManager::height - height, 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
		}
		GFX::drawImage(width * 5, 0, width, height, shadowMap.depthTexture);
		GFX::drawStringBG("shadow", width * 5, WindowManager::height - height, 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
bool nx;
bool ny;
int xP;
int yP;

void World::addChunk(std::shared_ptr<Chunk> chunk)
{
	//if (getChunk(chunk->x, chunk->y) == nullptr)
	{
		overworld.emplace_back(chunk);
		std::shared_ptr<Chunk> c = nullptr;
		for (int x = 0; x < 3; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				for (int z = 0; z < 3; z++)
				{
					c = getChunk(chunk->x + x - 1, chunk->y + y - 1, chunk->z + z - 1);
					if (c != nullptr)
					{
						c->addNeighbour(chunk, c);
					}
				}
			}
		}
	}
}

std::shared_ptr<Chunk> World::getChunk(int x, int y, int z)
{
	for (auto c : overworld)
	{
		if (c != nullptr && c->x == x && c->y == y && c->z == z)
		{
			return c;
		}
	}
	return nullptr;
}

std::shared_ptr<ChunkHeight> World::getChunkHeightmap(int x, int z)
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

bool World::isLoaded(int x, int y, int z)
{
	return getChunk(x, y, z) != nullptr;
}

std::shared_ptr<World> World::getOverworld()
{
	return nullptr;
}

void World::setVoxel(int x, int y, int z, int type, float density)
{
	std::shared_ptr<Chunk> c = getChunk(((float) x / (float) c->CHUNK_SIZE) * 2, (float)y / (float)c->CHUNK_SIZE * 2, (float)z / (float) c->CHUNK_SIZE * 2);
	if (c != nullptr)
	{
		//std::cout << x - (c->x * c->CHUNK_SIZE / 2) << " " << c->x << "  " << (y - c->y * c->CHUNK_SIZE) * 2 << "  " << (z - c->z * c->CHUNK_SIZE) * 2 << "\n";
		//c->setDensity((x - c->x * c->CHUNK_SIZE) * 2, (y - c->y * c->CHUNK_SIZE) * 2, (z - c->z * c->CHUNK_SIZE) * 2, density);
		//c->generate();
		//c->chunkUpdate = true;
	}
}

void World::setOverworldTime(long long int time)
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

void World::sphere(float xP, float yP, float zP, float radius, float power)
{
	std::shared_ptr<Chunk> c = nullptr;
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
					c = getChunk(GFX::CAM.cX, GFX::CAM.cY, GFX::CAM.cZ);
					
					if (c != nullptr)
					{
						//if (nX >= 0 && nY >= 0 && nZ >= 0 &&
						//	nX < size && nY < size && nZ < size)
						{
							c->setDensity(nX, nY, nZ, c->getDensity(nX, nY, nZ) + power * 0.01 * ((radius - distance) / radius));
							if (c->getDensity(nX, nY, nZ) > 1)
							{
								c->setDensity(nX, nY, nZ, 1);
							}
							else if (c->getDensity(nX, nY, nZ) < 0)
							{
								c->setDensity(nX, nY, nZ, 0);
							}
							if (c->getDensity(nX, nY, nZ) < 0.1 && power < 0)
							{
								c->setDensity(nX, nY, nZ, 0);
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
