#include "Engine/GameState.h"
#include <winsock2.h>
#include <stdio.h>
#include <Graphics/GFX.h>
#include <Utilities/Util.h>
#include <Engine/WindowManager.h>
#include <chrono>
#include <Engine/Controller.h>
#include <World/Chunk.h>
#include <Engine/Chat.h>
#include <World/ChunkIO.h>
#include <ServerManager.h>
#include <thread>
#include <PacketMsg.h>
#include <Audio/Sound.h>
#include <Server/ClientManager.h>
#include <Psapi.h>
#include <Utilities/Profiler.h>
#include <Utilities/Settings.h>
#include <Utilities/Assets.h>
#include <iostream>
#include <Engine/GLManager.h>
#include <Physics/Physics.h>
#include <Physics/PhysicsSphere.h>
#include <Physics/PhysicsWorld.h>
#include <mutex>
#include "Modify/Modify.h"
#include <Auravyx.h>
#include <stdexcept>
#include <Utilities\Log.h>
#include <Utilities\io.h>
#include <Utilities\M.h>
GameState::GameState()
{
}

GameState::~GameState()
{
}
std::vector<Chunk> chunks;

Chat chat;

//ClientManager c;
std::shared_ptr<Chunk> nextChunk;
std::atomic_int lock = false;
bool loading = false;

std::atomic_bool cleanup = false;

ServerManager iserver;
std::thread chunkLoader;
std::thread chunkMeshGenerator;
std::thread server;

Chunk ch;

void chunkLoading(const int xC, const int yC, const int zC)
{
	int size = GFX::getOverlay()->viewDistance * 2;
	double avg = 0;
	bool origin = 0;
	int cX = 0;
	int cY = 0;
	int cZ = 0;

	int z = 0;
	
	double start = glfwGetTime();
	for (int x = 0; x < size / 2; x++)
	{
		for (int z = 0; z < x + 1; z++)
		{
			for (int a = 0; a < 4; a++)
			{
				if (GFX::getOverlay()->CAM.cX != xC || 0 != yC || GFX::getOverlay()->CAM.cZ != zC)
				{
					//std::cout << GFX::getOverlay()->CAM.cX << "  " << GFX::getOverlay()->CAM.cY << "\n";
					//return;
				}
				if (a == 0)
				{
					cX = z;
					cZ = z - x;
				}
				else if (a == 1)
				{
					cX = -z;
					cZ = -(z - x);
				}
				else if (a == 2)
				{
					cX = -z;
					cZ = z - x;
				}
				else if (a == 3)
				{
					cX = z;
					cZ = -(z - x);
				}
				//std::shared_ptr<ChunkHeight> ch = w.getChunkHeightmap(cX, cZ);
				//if (ch == nullptr)
				{
					//ch = std::shared_ptr<ChunkHeight>(new ChunkHeight());
					//ch->generate(cX, cZ);
				}
				for (int y = -1; y < 7; y++)
				{
					if (cleanup)
					{
						return;
					}
					
					if (!lock && !GameManager::world.unloadLock)
					{
						cY = y;
						cX += xC;
						cY += yC;
						cZ += zC;
						if (origin && cX == 0 && cZ == 0)
						{
							origin = true;
						}
						//avg += c.generate();
						if (!origin && !GameManager::world.isLoaded(cX, cY, cZ))
						{
							nextChunk = ChunkIO::readChunk(cX, cY, cZ, "myworld");// new Chunk(cX, cY, cZ);
							//std::shared_ptr<Chunk> c (new Chunk(cX, cY, cZ));
							//c->generateTerrain(ch);

							//nextChunk = c;

							//std::shared_ptr<Chunk> c (new Chunk(cX, cY, cZ));
							//c->generateTerrain(ch)

							if (nextChunk)
							{
								/*if (GFX::getOverlay()->CAM.cX == c->x && GFX::getOverlay()->CAM.cY == c->z)
								{

									for (int i = 0; i < Chunk::CHUNK_SIZE; i++)
									{
										if (c->getDensity(GFX::getOverlay()->CAM.cX * Chunk::CHUNK_SIZE - GFX::getOverlay()->CAM.x, Chunk::CHUNK_SIZE - 1 - i,
											GFX::getOverlay()->CAM.cZ * Chunk::CHUNK_SIZE - GFX::getOverlay()->CAM.z) != 0)
										{
											if (c->y * Chunk::CHUNK_SIZE + Chunk::CHUNK_SIZE - 1 - i > height)
											{
												height = c->y * Chunk::CHUNK_SIZE + Chunk::CHUNK_SIZE - 1 - i;
											}
										}
									}
								}*/
								lock = true;
							}
						}
						GameManager::world.unloadLock = true;
					}
					else
					{
						y--;
					}
				}
			}
		}
	}
	double end = glfwGetTime();
	//std::cout << end - start << " DONE\n";
	double avgg = 0;
	int count = 0;
	for (int i = 0; i < 0; i++)
	{//Loop overworld
		//c->clearDensity();
	}
	return;
}
std::mutex chunkGuard;
std::atomic_int chunkGenerationSize = 0;

std::vector<Chunk*> chunksToMesh;
std::atomic_int chunkMeshingInProgress = false;
void chunkMeshGeneration()
{
	ThreadManager::getThreadManager()->registerThread(std::this_thread::get_id(), "Chunk Mesher");
	while (!cleanup)
	{
		if (chunkMeshingInProgress)
		{
			if (cleanup)
			{
				return;
			}
			for (auto c : chunksToMesh)
			{
				if (c && !c->loaded && c->neighboursLoaded())
				{
					if (!c->editingData)
					{
						c->generating = true;
						c->generate();
						c->generating = false;
						c->loaded = true;
						c->chunkUpdate = true;
					}
				}
			}
			chunksToMesh = std::vector<Chunk*>();
			chunkMeshingInProgress = false;
		}
	}
}

void GameState::update()
{
	//std::cout << s1.checkCollision(&s2) << "\n";
	if (WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_ENTER))
	{
		//s = s.substr(0, s.size() - 1);
		//if (s.size() != 0)
		{
			//	c.send(4, s);
			//	s.clear();
		}
	}

	if (WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_Q))
	{
		//PacketMsg::sendMessage("hello!!!");
	}

	GameManager::world.update();
	
	int x = GFX::getOverlay()->CAM.cX >> 2;
	int y = GFX::getOverlay()->CAM.cZ >> 2;

	//std::cout << x << " " << y << "\n";

	///////////c.update();
}

void world()
{
	WindowManager::getWindow()->getController()->update();
	GFX::getOverlay()->CAM.getPlayerInput();
	Matrix4f m4;
	int fov = GFX::getOverlay()->CAM.fov;
	if (glfwGetMouseButton(WindowManager::getWindow()->window, GLFW_MOUSE_BUTTON_4) == GLFW_PRESS)
	{
		fov /= 3.6666;
	}
	m4.createProjectionMatrix(WindowManager::getWindow()->getWidth(), WindowManager::getWindow()->getHeight(), GFX::getOverlay()->viewDistance * 1000, 0.1, fov);
	GameManager::world.render(GFX::getOverlay()->CAM, m4);
	
	Camera cam = GFX::getOverlay()->CAM;
}
double cpuUsageA = 0;
double cpuUsageB = 1;
double now;
double last;
double fpsCounter = 0;
double lastFpsCounter = 0;
std::vector<GLuint> ttex;

int id = 19;

void GameState::render()
{
	if (lock)
	{
		Chunk c = Chunk(*nextChunk.get());
		c.loaded = false;
		GameManager::world.addChunk(c);
		nextChunk.reset();
		chunkGenerationSize = GameManager::world.overworld.size() - 1;
		lock = false;
	}
	if (GameManager::world.unloadLock)
	{
		GameManager::world.unloadLock = false;
	}
	std::string fps = std::to_string(GFX::getOverlay()->FPS);
	int curFPS = 0;
	now = glfwGetTime();
	if (now > last + 0.5)
	{
		lastFpsCounter = fpsCounter;
		fpsCounter = GLManager::getFPS();
		last = glfwGetTime();
		cpuUsageA = cpuUsageB;
		cpuUsageB = Profiler::getCurrentProcessCPU();
	}
	
	bool meshFlag = false;
	if (!chunkMeshingInProgress)
	{
		for (auto& c : GameManager::world.overworld)
		{
			if (!c->loaded && c->neighboursLoaded())
			{
				//c->fill(64, 64, 64, 80, 80, 80, 8, 255);
				//c->sphere(64, 64, 64, 9, 7);
				if (c->priorityLoad)
				{
					chunksToMesh.insert(chunksToMesh.begin(), c.get());
					c->priorityLoad = false;
				}
				else
				{
					chunksToMesh.emplace_back(c.get());
				}
				meshFlag = true;
			}
		}
		if (meshFlag)
		{
			chunkMeshingInProgress = true;
		}
	}
	for (auto &c : GameManager::world.overworld)
	{
		if (c->chunkUpdate)
		{
			c->refresh();
			c->chunkUpdate = false;
			c->ready = true;
		}
	}
	if (WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_KP_0))
	{
		id = 18;
	}
	if (WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_KP_1))
	{
		id = 19;
	}
	if (WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_KP_2))
	{
		id = 20;
	}
	if (WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_KP_3))
	{
		id = 21;
	}
	if (WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_KP_4))
	{
		id = 22;
	}
	if (WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_KP_5))
	{
		id = 23;
	}
	if (WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_KP_6))
	{
		id = 24;
	}
	if (WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_KP_7))
	{
		id = 25;
	}
	if (WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_KP_8))
	{
		id = 26;
	}
	if (WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_KP_9))
	{
		id = 100;
	}
	if (WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_KP_ADD))
	{
		id = 7;
	}
	if (WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_KP_SUBTRACT))
	{
		id = 11;
	}
	if (WindowManager::getWindow()->getController()->isKeyDown(GLFW_KEY_KP_DIVIDE))
	{
		id = 6;
	}
	if (WindowManager::getWindow()->getController()->isMouseDown(GLFW_MOUSE_BUTTON_1))
	{
		Chunk* c = GameManager::world.getChunk(GFX::getOverlay()->CAM.cX, GFX::getOverlay()->CAM.cY, GFX::getOverlay()->CAM.cZ);
		if (c)
		{
			int x = ((int) floor(GFX::getOverlay()->CAM.xPos)) % 64;
			int y = ((int) floor(GFX::getOverlay()->CAM.yPos)) % 64;
			int z = ((int) floor(GFX::getOverlay()->CAM.zPos)) % 64;
			if (x < 0)
			{
				x = Chunk::CHUNK_SIZE + x;
			}
			if (y < 0)
			{
				y = Chunk::CHUNK_SIZE + y;
			}
			if (z < 0)
			{
				z = Chunk::CHUNK_SIZE + z;
			}
			int size = WindowManager::getWindow()->mainScroll;
			if (size < 1)
			{
				size = 1;
			}
			c->fill(x, y - 7, z, size, size, size, id, 255);
		}
	}
	if (WindowManager::getWindow()->getController()->isMouseDown(GLFW_MOUSE_BUTTON_2))
	{
		Chunk* c = GameManager::world.getChunk(GFX::getOverlay()->CAM.cX, GFX::getOverlay()->CAM.cY, GFX::getOverlay()->CAM.cZ);
		if (c)
		{
			int x = ((int)floor(GFX::getOverlay()->CAM.xPos)) % 64;
			int y = ((int)floor(GFX::getOverlay()->CAM.yPos)) % 64;
			int z = ((int)floor(GFX::getOverlay()->CAM.zPos)) % 64;
			if (x < 0)
			{
				x = Chunk::CHUNK_SIZE + x;
			}
			if (y < 0)
			{
				y = Chunk::CHUNK_SIZE + y;
			}
			if (z < 0)
			{
				z = Chunk::CHUNK_SIZE + z;
			}
			int size = WindowManager::getWindow()->mainScroll;
			if (size < 1)
			{
				size = 1;
			}
			c->clear(x, y - 7, z, size, size, size, 0);
		}
	}
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;

	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
	//fbo.bind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FBO::unbind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1);
	Camera cam = Camera(GFX::getOverlay()->CAM);
	auto finish = std::chrono::high_resolution_clock::now();

	world();

	chat.w = &GameManager::world;


	glEnable(GL_BLEND);

	int di = 0;
	int dim = 25;

	int rdi = 0;

	std::string fpsCap = std::to_string((int)GFX::getOverlay()->FPS);
	if (GFX::getOverlay()->FPS == GFX::getOverlay()->UNLIMITED_FPS)
	{
		fpsCap = "No cap";
	}
	curFPS = (int)roundf(((fpsCounter + lastFpsCounter) / 2));
	fps = std::to_string(curFPS);
	fps += std::string(" (" + fpsCap + ")");
	GFX::getOverlay()->drawStringBG("fps: " + fps, 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
	//GFX::getOverlay()->drawStringBGC("authenticated", 0, 0, 30, WindowManager::getWindow()->getWidth(), 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
	GFX::getOverlay()->drawStringBG("x: " + Util::removeDecimal(GFX::getOverlay()->CAM.xPos, 3), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
	GFX::getOverlay()->drawStringBG("y: " + Util::removeDecimal(GFX::getOverlay()->CAM.yPos, 3), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
	GFX::getOverlay()->drawStringBG("z: " + Util::removeDecimal(GFX::getOverlay()->CAM.zPos, 3), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
	GFX::getOverlay()->drawStringBG("c: " + std::to_string(GFX::getOverlay()->CAM.cX) + " / " + std::to_string(GFX::getOverlay()->CAM.cY) + " / " +
		std::to_string(GFX::getOverlay()->CAM.cZ), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);

	GFX::getOverlay()->drawStringBG("rot x: " + Util::removeDecimal(GFX::getOverlay()->CAM.xRot, 3), 0, dim* (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
	GFX::getOverlay()->drawStringBG("rot y: " + Util::removeDecimal(GFX::getOverlay()->CAM.yRot, 3), 0, dim* (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
	GFX::getOverlay()->drawStringBG("rot z: " + Util::removeDecimal(GFX::getOverlay()->CAM.zRot, 3), 0, dim* (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);

	float velocity = sqrt(pow(GFX::getOverlay()->CAM.xVel, 2) + pow(GFX::getOverlay()->CAM.yVel, 2) + pow(GFX::getOverlay()->CAM.zVel, 2));
	GFX::getOverlay()->drawStringBG("v: " + Util::removeDecimal(velocity, 1) + " m/s", 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
	SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
	DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
	int renderableChunk = 0;
	for (auto &c : GameManager::world.overworld)
	{
		if (c != nullptr && c->ready)
		{
			renderableChunk++;
		}
	}
	GFX::getOverlay()->drawStringBG("render: " + std::to_string(renderableChunk) + " / " +
		std::to_string(GameManager::world.overworld.size()), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	GFX::getOverlay()->drawStringBG("time: " +
		std::to_string(((GameManager::world.getOverworldTime() + 60000) % GameManager::world.getOverworldDayCycle()) / 10000) + ":" +
		std::to_string((int)((GameManager::world.getOverworldTime() % GameManager::world.getOverworldDayCycle() / (3600)) % 60)) + ":" +
		std::to_string((int)((GameManager::world.getOverworldTime() % 10000 / 60 % 60))) + " (" +
		std::to_string((int)(((GameManager::world.getOverworldTime()) + 60000) / GameManager::world.getOverworldDayCycle())) + ")",
		0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
	GFX::getOverlay()->drawStringBG("tick: " + std::to_string((int)(GameManager::world.getOverworldTime())),
		0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	GFX::getOverlay()->drawStringBG("physics: " + std::string("no clip"),
		0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	GFX::getOverlay()->drawStringBG("fly: " + std::string("on"),
		0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	GFX::getOverlay()->drawStringBGR("cpu usage: " + Util::removeDecimal((cpuUsageA + cpuUsageB) / 2, 2) + "%",
		0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	GFX::getOverlay()->drawStringBGR("mem: " + Util::removeDecimal(((float) static_cast<long long>(physMemUsedByMe) / 1000000000.0), 3) + " / " +
		Util::removeDecimal((float) static_cast<long long>(totalPhysMem) / 1000000000.0, 1) + " GB"
		, 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	GFX::getOverlay()->drawStringBGR("mods active: " + std::to_string(Modify::getModify()->getEnabledModCount()) +
		" / " + std::to_string(Modify::getModify()->getEnabledModCount())
		, 0, dim* (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	GFX::getOverlay()->drawStringBGR("view distance: " + std::to_string(GFX::getOverlay()->viewDistance) + " (" +
		std::to_string(GFX::getOverlay()->viewDistance * Chunk::CHUNK_SIZE) + "m)", 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	GFX::getOverlay()->drawStringBGR("brightness: " + Util::removeDecimal(GFX::getOverlay()->brightness, 2), 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	if (Profiler::showAdvancedDebugInfo)
	{
		GFX::getOverlay()->drawStringBG("rot x: " + Util::removeDecimal(GFX::getOverlay()->CAM.xRot, 3), 0, dim* (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
		GFX::getOverlay()->drawStringBG("rot y: " + Util::removeDecimal(GFX::getOverlay()->CAM.yRot, 3), 0, dim* (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
		GFX::getOverlay()->drawStringBG("rot z: " + Util::removeDecimal(GFX::getOverlay()->CAM.zRot, 3), 0, dim* (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);

		Vec3f normalizedRotation = Vec3f(-sin(M::toRadians(GFX::getOverlay()->CAM.yRot)) * (cos(M::toRadians(GFX::getOverlay()->CAM.xRot))),
			sin(M::toRadians(GFX::getOverlay()->CAM.xRot)),
			-cos(M::toRadians(GFX::getOverlay()->CAM.yRot)) * (cos(M::toRadians(GFX::getOverlay()->CAM.xRot))));

		Vec3f normalizedUpRotation = Vec3f(sin(M::toRadians(GFX::getOverlay()->CAM.yRot)) * (sin(M::toRadians(GFX::getOverlay()->CAM.xRot))),
			cos(M::toRadians(GFX::getOverlay()->CAM.xRot)),
			cos(M::toRadians(GFX::getOverlay()->CAM.yRot)) * (sin(M::toRadians(GFX::getOverlay()->CAM.xRot))));

		GFX::getOverlay()->drawStringBG("n rot x: " + Util::removeDecimal(normalizedRotation.x, 3), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
		GFX::getOverlay()->drawStringBG("n rot y: " + Util::removeDecimal(normalizedRotation.y, 3), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
		GFX::getOverlay()->drawStringBG("n rot z: " + Util::removeDecimal(normalizedRotation.z, 3), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);

		GFX::getOverlay()->drawStringBG("n up rot x: " + Util::removeDecimal(normalizedUpRotation.x, 3), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
		GFX::getOverlay()->drawStringBG("n up rot y: " + Util::removeDecimal(normalizedUpRotation.y, 3), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
		GFX::getOverlay()->drawStringBG("n up rot z: " + Util::removeDecimal(normalizedUpRotation.z, 3), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);

		GFX::getOverlay()->drawStringBGR("advanced debug on", 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
		GFX::getOverlay()->drawStringBGR("mipmap bias: " + std::to_string(GFX::getOverlay()->mipmapBias), 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
		GFX::getOverlay()->drawStringBGR("terrain tex res: " + std::to_string(GFX::getOverlay()->terrainTextureResolution), 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
		GFX::getOverlay()->drawStringBGR("physics objects: " + std::to_string(0), 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
	}
	else
	{
		GFX::getOverlay()->drawStringBGR("advanced debug off", 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
	}
	chat.render();
	//Auravyx::getAuravyx()->draw();
	std::vector<std::pair<std::function<void()>, GLContext*>>* functions = Renderer::getRenderer()->getRenderFunctions();

	for (int i = 0; i < functions->size(); i++)
	{
		functions->at(i).second->set();
		functions->at(i).first();
	}
	SoundManager::getSoundManager()->setListener(GFX::getOverlay()->CAM);
}
void startChunkLoader()
{
	ThreadManager::getThreadManager()->registerThread(std::this_thread::get_id(), "Chunk Loader");
	//while (true)
	{
		Camera cam = GFX::getOverlay()->CAM;
		////////FOLLOW PLAYER/////////////////////////chunkLoading(cam.cX, 0, cam.cZ);
		chunkLoading(0, 0, 0);
	}
}
void serverStart()
{
	iserver.start();
}
bool hostServer = false;
void physicsCallback(std::string s)
{
	std::cout << s;
}
void GameState::start()
{
	ChunkIO::saveArea();
	Physics::addCallback(physicsCallback);
	
	Sound s;
	s.play(Assets::getAssets()->getAudio("song1"));
	s.setGain(10);
	s.setPitch(0.9);
	s.setTime(30);
	s.setPosition(0, 0, 0);
	hostServer = false;// Settings::getBool("host");
	Modify::getModify()->loadAllMods();
	
	if (hostServer)
	{
		server = std::thread(serverStart);
		while (!ServerManager::ready)
		{

		}
	}
	chunkLoader = std::thread(startChunkLoader);
	chunkMeshGenerator = std::thread(chunkMeshGeneration);
	GameManager::world.create();
	GameManager::world.test();
}

void GameState::stop()
{
	cleanup = true;
	/////////////c.stop();
	iserver.stop();
	chunkLoader.join();
	chunkMeshGenerator.join();
	//while (iserver.running)
	{

	}
	if (hostServer)
	{
		server.join();
	}
	glfwPollEvents();
	glfwDestroyWindow(WindowManager::getWindow()->window);
}
