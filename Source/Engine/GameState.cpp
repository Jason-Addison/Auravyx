#include "Engine/GameState.h"
#include <winsock2.h>
#include <stdio.h>
#include <Graphics/GFX.h>
#include <Util.h>
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
GameState::GameState()
{
}

GameState::~GameState()
{
}
std::vector<Chunk> chunks;

Chat chat;
//ClientManager c;

std::vector<std::shared_ptr<Chunk>> chunksToLoad;
std::shared_ptr<Chunk> nextChunk;
std::atomic_int lock = false;
bool loading = false;

std::atomic_bool cleanup = false;

ServerManager iserver;
std::thread chunkLoader;
std::thread chunkMeshGenerator;
std::thread server;

Chunk ch;

World w;

void chunkLoading(int xC, int yC, int zC)
{
	int size = Auravyx::getAuravyx()->getOverlay()->viewDistance * 2;
	double avg = 0;
	bool origin = 0;
	int cX = 0;
	int cY = 0;
	int cZ = 0;

	int z = 0;
	
	ChunkIO cio;
	double start = glfwGetTime();
	for (int x = 0; x < size / 2; x++)
	{
		for (int z = 0; z < x + 1; z++)
		{
			for (int a = 0; a < 4; a++)
			{
				if (Auravyx::getAuravyx()->getOverlay()->CAM.cX != xC || 0 != yC || Auravyx::getAuravyx()->getOverlay()->CAM.cZ != zC)
				{
					//std::cout << Auravyx::getAuravyx()->getOverlay()->CAM.cX << "  " << Auravyx::getAuravyx()->getOverlay()->CAM.cY << "\n";
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
					//	ch = std::shared_ptr<ChunkHeight>(new ChunkHeight());
						//ch->generate(cX, cZ);
				}
				for (int y = -2; y < 4; y++)
				{
					if (cleanup)
					{
						return;
					}
					
					if (!lock && !w.unloadLock)
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
						if (!origin && !w.isLoaded(cX, cY, cZ))
						{
							//nextChunk.release();
							//chunksToLoad.emplace_back(std::unique_ptr<Chunk>(new Chunk(c)));

							std::shared_ptr<Chunk> c = cio.readChunk(cX, cY, cZ, "myworld");// new Chunk(cX, cY, cZ);

							//std::shared_ptr<Chunk> c (new Chunk(cX, cY, cZ));
							//c->generateTerrain(ch);

							//std::shared_ptr<Chunk> c (new Chunk(cX, cY, cZ));
							//c->generateTerrain(ch)

							if (c)
							{
								/*if (Auravyx::getAuravyx()->getOverlay()->CAM.cX == c->x && Auravyx::getAuravyx()->getOverlay()->CAM.cY == c->z)
								{

									for (int i = 0; i < Chunk::CHUNK_SIZE; i++)
									{
										if (c->getDensity(Auravyx::getAuravyx()->getOverlay()->CAM.cX * Chunk::CHUNK_SIZE - Auravyx::getAuravyx()->getOverlay()->CAM.x, Chunk::CHUNK_SIZE - 1 - i,
											Auravyx::getAuravyx()->getOverlay()->CAM.cZ * Chunk::CHUNK_SIZE - Auravyx::getAuravyx()->getOverlay()->CAM.z) != 0)
										{
											if (c->y * Chunk::CHUNK_SIZE + Chunk::CHUNK_SIZE - 1 - i > height)
											{
												height = c->y * Chunk::CHUNK_SIZE + Chunk::CHUNK_SIZE - 1 - i;
											}
										}
									}
								}*/
								nextChunk = c;

								lock = true;
							}
						}
						w.unloadLock = true;
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
	
	for (auto c : w.overworld)
	{
		//c->clearDensity();
	}
	return;
}
std::mutex chunkGuard;
std::atomic_int chunkGenerationSize = 0;
void chunkMeshGeneration()
{
	while (true)
	{
		int temp = chunkGenerationSize;
		
		for (int i = 0; i < temp; i++)
		{
			if (cleanup)
			{
				return;
			}
			try
			{
				std::lock_guard<std::mutex> guard(chunkGuard);
				auto c = w.overworld.at(i);

				if (c && !c->loaded && c->neighboursLoaded())
				{
					c->generate();
					c->loaded = true;
					c->chunkUpdate = true;
				}
			}
			catch (const std::exception& e)
			{
				std::cout << "Crash! " << e.what() << "\n";
			}
		}
	}
}

void GameState::update()
{
	Auravyx::getAuravyx()->getWindow()->getController()->update();
	//std::cout << s1.checkCollision(&s2) << "\n";
	if (Auravyx::getAuravyx()->getWindow()->getController()->isKeyDown(GLFW_KEY_ENTER))
	{
		//s = s.substr(0, s.size() - 1);
		//if (s.size() != 0)
		{
			//	c.send(4, s);
			//	s.clear();
		}
	}

	if (Auravyx::getAuravyx()->getWindow()->getController()->isKeyDown(GLFW_KEY_Q))
	{
		//PacketMsg::sendMessage("hello!!!");
	}
	
	Auravyx::getAuravyx()->getOverlay()->CAM.getPlayerInput();
	w.update();
	
	int x = Auravyx::getAuravyx()->getOverlay()->CAM.cX >> 2;
	int y = Auravyx::getAuravyx()->getOverlay()->CAM.cZ >> 2;

	//std::cout << x << " " << y << "\n";

	///////////c.update();
}

void world()
{
	Matrix4f m4;
	int fov = Auravyx::getAuravyx()->getOverlay()->CAM.fov;
	if (glfwGetMouseButton(Auravyx::getAuravyx()->getWindow()->window, GLFW_MOUSE_BUTTON_4) == GLFW_PRESS)
	{
		fov = 30;
	}
	m4.createProjectionMatrix(Auravyx::getAuravyx()->getWindow()->getWidth(), Auravyx::getAuravyx()->getWindow()->getHeight(), Auravyx::getAuravyx()->getOverlay()->viewDistance * 1000, 0.1, fov);

	w.render(&Auravyx::getAuravyx()->getOverlay()->CAM, &m4);
	
	Camera cam = Auravyx::getAuravyx()->getOverlay()->CAM;
}
double cpuUsageA = 0;
double cpuUsageB = 1;
double now;
double last;
double fpsCounter = 0;
double lastFpsCounter = 0;

void GameState::render()
{
	if (lock)
	{
		w.addChunk(std::shared_ptr<Chunk>(new Chunk(*nextChunk)));
		nextChunk.reset();
		chunkGenerationSize = w.overworld.size();
		lock = false;
	}
	if (w.unloadLock)
	{
		w.unloadLock = false;
	}
	std::string fps = std::to_string(Auravyx::getAuravyx()->getOverlay()->FPS);
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
	
	for (auto c : w.overworld)
	{
		if (c->chunkUpdate)
		{
			c->refresh();
			c->chunkUpdate = false;
			c->ready = true;
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
	Camera cam = Camera(Auravyx::getAuravyx()->getOverlay()->CAM);
	auto finish = std::chrono::high_resolution_clock::now();

	world();

	chat.w = &w;




	glEnable(GL_BLEND);

	int di = 0;
	int dim = 25;

	int rdi = 0;

	std::string fpsCap = std::to_string((int)Auravyx::getAuravyx()->getOverlay()->FPS);
	if (Auravyx::getAuravyx()->getOverlay()->FPS == Auravyx::getAuravyx()->getOverlay()->UNLIMITED_FPS)
	{
		fpsCap = "No cap";
	}
	curFPS = (int)roundf(((fpsCounter + lastFpsCounter) / 2));
	fps = std::to_string(curFPS);
	fps += std::string(" (" + fpsCap + ")");
	Auravyx::getAuravyx()->getOverlay()->drawStringBG("fps : " + fps, 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
	Auravyx::getAuravyx()->getOverlay()->drawStringBGC("authenticated", 0, 0, 30, Auravyx::getAuravyx()->getWindow()->getWidth(), 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
	Auravyx::getAuravyx()->getOverlay()->drawStringBG("x : " + Util::removeDecimal(Auravyx::getAuravyx()->getOverlay()->CAM.x, 3), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
	Auravyx::getAuravyx()->getOverlay()->drawStringBG("y : " + Util::removeDecimal(Auravyx::getAuravyx()->getOverlay()->CAM.y, 3), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
	Auravyx::getAuravyx()->getOverlay()->drawStringBG("z : " + Util::removeDecimal(Auravyx::getAuravyx()->getOverlay()->CAM.z, 3), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
	Auravyx::getAuravyx()->getOverlay()->drawStringBG("c : " + std::to_string(Auravyx::getAuravyx()->getOverlay()->CAM.cX) + " / " + std::to_string(Auravyx::getAuravyx()->getOverlay()->CAM.cY) + " / " +
		std::to_string(Auravyx::getAuravyx()->getOverlay()->CAM.cZ), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);

	float velocity = sqrt(pow(Auravyx::getAuravyx()->getOverlay()->CAM.xVel, 2) + pow(Auravyx::getAuravyx()->getOverlay()->CAM.yVel, 2) + pow(Auravyx::getAuravyx()->getOverlay()->CAM.zVel, 2));
	velocity *= 60;
	Auravyx::getAuravyx()->getOverlay()->drawStringBG("v : " + Util::removeDecimal(velocity, 1) + " m/s", 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
	SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
	DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
	int renderableChunk = 0;
	for (auto c : w.overworld)
	{
		if (c != nullptr && c->ready)
		{
			renderableChunk++;
		}
	}
	Auravyx::getAuravyx()->getOverlay()->drawStringBG("render : " + std::to_string(renderableChunk) + " / " +
		std::to_string(w.overworld.size()), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	Auravyx::getAuravyx()->getOverlay()->drawStringBG("time : " +
		std::to_string(((w.getOverworldTime() + 60000) % w.getOverworldDayCycle()) / 10000) + ":" +
		std::to_string((int)((w.getOverworldTime() % w.getOverworldDayCycle() / (3600)) % 60)) + ":" +
		std::to_string((int)((w.getOverworldTime() % 10000 / 60 % 60))) + " (" +
		std::to_string((int)(((w.getOverworldTime()) + 60000) / w.getOverworldDayCycle())) + ")",
		0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
	Auravyx::getAuravyx()->getOverlay()->drawStringBG("tick : " + std::to_string((int)(w.getOverworldTime())),
		0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	Auravyx::getAuravyx()->getOverlay()->drawStringBG("physics : " + std::string("no clip"),
		0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	Auravyx::getAuravyx()->getOverlay()->drawStringBG("fly : " + std::string("on"),
		0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	Auravyx::getAuravyx()->getOverlay()->drawStringBGR("cpu usage : " + Util::removeDecimal((cpuUsageA + cpuUsageB) / 2, 2) + "%",
		0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	Auravyx::getAuravyx()->getOverlay()->drawStringBGR("mem : " + Util::removeDecimal(((float) static_cast<long long>(physMemUsedByMe) / 1000000000.0), 3) + " / " +
		Util::removeDecimal((float) static_cast<long long>(totalPhysMem) / 1000000000.0, 1) + " GB"
		, 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	Auravyx::getAuravyx()->getOverlay()->drawStringBGR("mods active : " + std::to_string(Auravyx::getAuravyx()->getModify()->getEnabledModCount()) +
		" / " + std::to_string(Auravyx::getAuravyx()->getModify()->getDisabledModCount())
		, 0, dim* (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	Auravyx::getAuravyx()->getOverlay()->drawStringBGR("view distance : " + std::to_string(Auravyx::getAuravyx()->getOverlay()->viewDistance) + " (" +
		std::to_string(Auravyx::getAuravyx()->getOverlay()->viewDistance * Chunk::CHUNK_SIZE) + "m)", 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	Auravyx::getAuravyx()->getOverlay()->drawStringBGR("brightness : " + Util::removeDecimal(Auravyx::getAuravyx()->getOverlay()->brightness, 2), 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	if (Profiler::showAdvancedDebugInfo)
	{
		Auravyx::getAuravyx()->getOverlay()->drawStringBGR("advanced debug on", 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
		Auravyx::getAuravyx()->getOverlay()->drawStringBGR("mipmap bias : " + std::to_string(Auravyx::getAuravyx()->getOverlay()->mipmapBias), 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
		Auravyx::getAuravyx()->getOverlay()->drawStringBGR("terrain tex res : " + std::to_string(Auravyx::getAuravyx()->getOverlay()->terrainTextureResolution), 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
		Auravyx::getAuravyx()->getOverlay()->drawStringBGR("physics objects : " + std::to_string(0), 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
	}
	else
	{
		Auravyx::getAuravyx()->getOverlay()->drawStringBGR("advanced debug off", 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
	}
	chat.render();
	//Auravyx::getAuravyx()->draw();
}
void startChunkLoader()
{
	//while (true)
	{
		Camera cam = Auravyx::getAuravyx()->getOverlay()->CAM;
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
	ChunkIO c;
	c.saveArea();
	Physics::addCallback(physicsCallback);
	
	Sound s;
	s.play(Auravyx::getAuravyx()->getAssets()->getAudio("Fall"));
	hostServer = false;// Settings::getBool("host");
	
	if (hostServer)
	{
		server = std::thread(serverStart);
		while (!ServerManager::ready)
		{

		}
	}
	chunkLoader = std::thread(startChunkLoader);
	chunkMeshGenerator = std::thread(chunkMeshGeneration);
	w.create();
	w.test();

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
}
