#include "GameState.h"
#include <winsock2.h>
#include <stdio.h>
#include <GFX.h>
#include <Util.h>
#include <WindowManager.h>
#include <chrono>
#include <Controller.h>
#include <Chunk.h>
#include <Chat.h>
#include <ChunkIO.h>
#include <ServerManager.h>
#include <thread>
#include <PacketMsg.h>
#include <Sound.h>
#include <ClientManager.h>
#include <Psapi.h>
#include <Profiler.h>
#include <Settings.h>
#include <Assets.h>
#include <iostream>
#include <GLManager.h>
GameState::GameState()
{
}

GameState::~GameState()
{
}
std::vector<Chunk> chunks;

Chat chat;
//ClientManager c;
std::shared_ptr<Chunk> chunkToModel = nullptr;

std::vector<std::shared_ptr<Chunk>> chunksToLoad;
std::shared_ptr<Chunk> nextChunk;
std::atomic_int lock = false;
bool loading = false;

ServerManager iserver;
std::thread chunkLoader;

Chunk ch;

World w;

void chunkLoading(int xC, int yC, int zC)
{
	int size = GFX::viewDistance * 2;
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
				if (GFX::CAM.cX != xC || 0 != yC || GFX::CAM.cZ != zC)
				{
					//std::cout << GFX::CAM.cX << "  " << GFX::CAM.cY << "\n";
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
					//	ch->generate(cX, cZ);
				}
				for (int y = -2; y < 4; y++)
				{
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


							std::shared_ptr<Chunk> c = cio.readChunk(cX, cY, cZ);// new Chunk(cX, cY, cZ);

							//std::shared_ptr<Chunk> c (new Chunk(cX, cY, cZ));
							//c->generateTerrain(ch);

							//std::shared_ptr<Chunk> c (new Chunk(cX, cY, cZ));
							//c->generateTerrain(ch)

							if (c)
							{
								/*if (GFX::CAM.cX == c->x && GFX::CAM.cY == c->z)
								{

									for (int i = 0; i < Chunk::CHUNK_SIZE; i++)
									{
										if (c->getDensity(GFX::CAM.cX * Chunk::CHUNK_SIZE - GFX::CAM.x, Chunk::CHUNK_SIZE - 1 - i,
											GFX::CAM.cZ * Chunk::CHUNK_SIZE - GFX::CAM.z) != 0)
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
	//GFX::CAM.y = height + 3;
	
	for (auto c : w.overworld)
	{
		if (!c->loaded && c->neighboursLoaded())
		{
			double before = glfwGetTime();
			c->generate();
			double now = glfwGetTime();
			avgg += now - before;
			count++;
			c->loaded = true;
			c->chunkUpdate = true;
		}
	}
	//std::cout << avgg / (double)count << " AVG\n";

	for (auto c : w.overworld)
	{
		//c->clearDensity();
	}
	return;
}

void GameState::update()
{
	Controller::update();
	if (Controller::isKeyDown(GLFW_KEY_ENTER))
	{
		//s = s.substr(0, s.size() - 1);
		//if (s.size() != 0)
		{
			//	c.send(4, s);
			//	s.clear();
		}
	}

	if (Controller::isKeyDown(GLFW_KEY_Q))
	{
		PacketMsg::sendMessage("hello!!!");
	}
	
	GFX::CAM.getPlayerInput();
	w.update();
	///////////c.update();
}

void world()
{
	Matrix4f m4;
	int fov = GFX::CAM.fov;
	if (glfwGetMouseButton(WindowManager::window, GLFW_MOUSE_BUTTON_4) == GLFW_PRESS)
	{
		fov = 30;
	}
	m4.createProjectionMatrix(WindowManager::width, WindowManager::height, GFX::viewDistance * 1000, 0.1, fov);

	w.render(&GFX::CAM, &m4);
	Camera cam = GFX::CAM;
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
		chunkToModel = std::shared_ptr<Chunk>(new Chunk(*nextChunk));
		w.addChunk(std::shared_ptr<Chunk>(new Chunk(*nextChunk)));
		nextChunk.reset();
		lock = false;
	}
	if (w.unloadLock)
	{
		w.unloadLock = false;
	}
	std::string fps = std::to_string(GFX::FPS);
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
	Camera cam = Camera(GFX::CAM);
	auto finish = std::chrono::high_resolution_clock::now();

	world();
	chat.w = &w;




	glEnable(GL_BLEND);

	int di = 0;
	int dim = 25;

	int rdi = 0;

	std::string fpsCap = std::to_string((int)GFX::FPS);
	if (GFX::FPS == GFX::UNLIMITED_FPS)
	{
		fpsCap = "No cap";
	}
	curFPS = (int)roundf(((fpsCounter + lastFpsCounter) / 2));
	fps = std::to_string(curFPS);
	fps += std::string(" (" + fpsCap + ")");
	GFX::drawStringBG("fps : " + fps, 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
	GFX::drawStringBGC("authenticated", 0, 0, 30, WindowManager::getWidth(), 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
	GFX::drawStringBG("x : " + Util::removeDecimal(GFX::CAM.x, 3), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
	GFX::drawStringBG("y : " + Util::removeDecimal(GFX::CAM.y, 3), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
	GFX::drawStringBG("z : " + Util::removeDecimal(GFX::CAM.z, 3), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
	GFX::drawStringBG("c : " + std::to_string(GFX::CAM.cX) + " / " + std::to_string(GFX::CAM.cY) + " / " +
		std::to_string(GFX::CAM.cZ), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);

	float velocity = sqrt(pow(GFX::CAM.xVel, 2) + pow(GFX::CAM.yVel, 2) + pow(GFX::CAM.zVel, 2));
	velocity *= 60;
	GFX::drawStringBG("v : " + Util::removeDecimal(velocity, 1) + " m/s", 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3f);
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
	GFX::drawStringBG("render : " + std::to_string(renderableChunk) + " / " +
		std::to_string(w.overworld.size()), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	GFX::drawStringBG("time : " +
		std::to_string(((w.getOverworldTime() + 60000) % w.getOverworldDayCycle()) / 10000) + ":" +
		std::to_string((int)((w.getOverworldTime() % w.getOverworldDayCycle() / (3600)) % 60)) + ":" +
		std::to_string((int)((w.getOverworldTime() % 10000 / 60 % 60))) + " (" +
		std::to_string((int)(((w.getOverworldTime()) + 60000) / w.getOverworldDayCycle())) + ")",
		0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
	GFX::drawStringBG("tick : " + std::to_string((int)(w.getOverworldTime())),
		0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	GFX::drawStringBG("physics : " + std::string("no clip"),
		0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	GFX::drawStringBG("fly : " + std::string("on"),
		0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	GFX::drawStringBGR("cpu usage : " + Util::removeDecimal((cpuUsageA + cpuUsageB) / 2, 2) + "%",
		0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	GFX::drawStringBGR("mem : " + Util::removeDecimal(((float) static_cast<long long>(physMemUsedByMe) / 1000000000.0), 3) + " / " +
		Util::removeDecimal((float) static_cast<long long>(totalPhysMem) / 1000000000.0, 1) + " GB"
		, 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	GFX::drawStringBGR("view distance : " + std::to_string(GFX::viewDistance) + " (" +
		std::to_string(GFX::viewDistance * Chunk::CHUNK_SIZE) + "m)", 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	GFX::drawStringBGR("brightness : " + Util::removeDecimal(GFX::brightness, 2), 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

	if (Profiler::showAdvancedDebugInfo)
	{
		GFX::drawStringBGR("advanced debug on", 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
		GFX::drawStringBGR("mipmap bias : " + std::to_string(GFX::mipmapBias), 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
		GFX::drawStringBGR("terrain tex res : " + std::to_string(GFX::terrainTextureResolution), 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
		GFX::drawStringBGR("physics objects : " + std::to_string(0), 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
	}
	else
	{
		GFX::drawStringBGR("advanced debug off", 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
	}
	chat.render();
}
void startChunkLoader()
{
	//while (true)
	{
		Camera cam = GFX::CAM;
		////////FOLLOW PLAYER/////////////////////////chunkLoading(cam.cX, 0, cam.cZ);
		chunkLoading(0, 0, 0);
	}
}
std::thread server;
void serverStart()
{
	iserver.start();
}
bool hostServer = false;
void GameState::start()
{
	//Sound s;
	//s.play(Assets::getAudio("song"));
	hostServer = false;// Settings::getBool("host");
	
	if (hostServer)
	{
		server = std::thread(serverStart);
		while (!ServerManager::ready)
		{

		}
	}
	chunkLoader = std::thread(startChunkLoader);
	w.create();
}

void GameState::stop()
{
	/////////////c.stop();
	iserver.stop();
	chunkLoader.join();
	while (iserver.running)
	{

	}
	if (hostServer)
	{
		server.join();
	}
}
