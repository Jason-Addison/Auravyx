#include "stdafx.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "WindowManager.h"
#include <string>
#include <iostream>
#include <WinSock2.h>
#include "al.h"
#include "SoundManager.h"
#include "Texture.h"
#include "Log.h"
#include "WAVE.h"
#include "Sound.h"
#include "Control.h"
#include "Resource.h"
#include "GFX.h"
#include "Assets.h"
#include "Shaders.h"
#include <chrono>
#include "FBO.h"
#include <list>
#include "GUI.h"
#include <windows.h>
#include "psapi.h"
#include "GUIContainer.h"
#include "Button.h"
#include "FontLoader.h"
#include "ScrollBar.h"
#include "efx.h"
#include "FileIO.h"
#include "ListMenu.h"
#include "Image.h"
#include <math.h>
#include "DropdownMenu.h"
#include "Clock.h"
#include <thread>
#include <chrono>
#include "World.h"
#include "ServerManager.h"
#include "ClientManager.h"
#include "PacketMsg.h"
#include "Server.h"
#include "Mouse.h"
#include "Settings.h"
#include "Resource.h"
#include "resource.h"
#include <GameManager.h>
#include "Chunk.h"
#include "Voxel.h"
#include <iomanip>
#include <M.h>
#include "ChunkIO.h"
#include "Chat.h"
#include <Profiler.h>
#include "SOIL/SOIL.h"
WindowManager window;

double UPS = 60;
double thisFrame = 0;
double nextFrame = 0;

std::chrono::duration<long, std::milli> timeStep = std::chrono::milliseconds(1);

void createWindow()
{
	window.create();
}
Chunk ch;

World w;
void glfwErrorCallback(int error, const char* description)
{
	std::string errorCodeString;
	errorCodeString.append("[");
	errorCodeString.append(std::to_string(error));
	errorCodeString.append("] ");
	errorCodeString.append(description);
	Log::out("OpenGL", "GLFW Error : " + errorCodeString + " " + description, LBLUE);
}
std::thread renderer;
std::vector<Chunk> chunks;

Chat chat;

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

std::vector<std::shared_ptr<Chunk>> chunksToLoad;
std::shared_ptr<Chunk> nextChunk;
std::atomic_int lock = false;
bool loading = false;
auto start = std::chrono::high_resolution_clock::now();
void render()
{
	window.update();
	FBO::unbind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1);
	Camera cam = Camera(GFX::CAM);
	auto finish = std::chrono::high_resolution_clock::now();
	double t = (double) std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();

	world();
	chat.w = &w;
}
bool end = false;
bool updateEnd = false;
void update()
{
	GFX::CAM.getPlayerInput();
	w.update();
}

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
	float height = -6128;

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
void startChunkLoader()
{
	//while (true)
	{
		Camera cam = GFX::CAM;
		////////FOLLOW PLAYER/////////////////////////chunkLoading(cam.cX, 0, cam.cZ);
		chunkLoading(0, 0, 0);
	}
	while (false)
	{
		if (glfwGetMouseButton(WindowManager::window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			//w.cuboid(GFX::CAM.x, GFX::CAM.y, GFX::CAM.z, 1, 1, 1, 1);
			w.setVoxel(GFX::CAM.x, GFX::CAM.y, GFX::CAM.z, 1, 1);
			//std::cout << GFX::CAM.x << " " << GFX::CAM.y << " " << GFX::CAM.z << "\n";
			//w.sphere(GFX::CAM.x, GFX::CAM.y, GFX::CAM.z, 10, 1);
			std::shared_ptr<Chunk> c = w.getChunk(GFX::CAM.cX, GFX::CAM.cY, GFX::CAM.cZ);
			if (c != nullptr)
			{
				float x = GFX::CAM.x * 2 - GFX::CAM.cX * 32 + 2;
				float y = GFX::CAM.y * 2 - GFX::CAM.cY * 32 - 2;
				float z = GFX::CAM.z * 2 - GFX::CAM.cZ * 32 + 2;

				c->sphere(x, y, z, 10, 1);

				c->generate();
				c->chunkUpdate = true;
			}
		}
		else if (glfwGetMouseButton(WindowManager::window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			std::shared_ptr<Chunk> c = w.getChunk(GFX::CAM.cX, GFX::CAM.cY, GFX::CAM.cZ);

			if (c != nullptr)
			{
				float x = GFX::CAM.x * 2 - GFX::CAM.cX * 32 + 2;
				float y = GFX::CAM.y * 2 - GFX::CAM.cY * 32 - 2;
				float z = GFX::CAM.z * 2 - GFX::CAM.cZ * 32 + 2;

				c->sphere(x, y, z, 8, -5);

				c->generate();
				//c->chunkUpdate = true;
			}
		}
	}
}
std::string s;
ServerManager iserver;
std::thread chunkLoader;

void updater()
{
	chunkLoader = std::thread(startChunkLoader);
	ClientManager c;
	double lerp = 0;
	double updatesPerSecond = 1 / UPS;
	double lastTimeUPS = -1;
	double thisTimeUPS = 0;
	Mouse m;
	int time;
	double deltaUpdate;
	double lastTimeUPSOld = 0;
	while (!end)
	{
		//double lerp = delta * UPS;
		thisTimeUPS = glfwGetTime();
		deltaUpdate = thisTimeUPS - lastTimeUPS;
		lastTimeUPSOld = lastTimeUPS;
		lerp = deltaUpdate;
		if (lerp >= 1)
			lerp = fmod(lerp, 1.0);
		if (deltaUpdate >= updatesPerSecond)
		{
			Controller::update();
			if (Controller::isKeyDown(GLFW_KEY_ENTER))
			{
				s = s.substr(0, s.size() - 1);
				if (s.size() != 0)
				{
					c.send(4, s);
					s.clear();
				}
			}
			
			if (Controller::isKeyDown(GLFW_KEY_Q))
			{
				PacketMsg::sendMessage("hello!!!");
			}
			lastTimeUPS = thisTimeUPS;
			update();
			c.update();
			chat.update();
			m.update(&(GameManager::world));
		}
		else
		{
			time = (updatesPerSecond - (glfwGetTime() - lastTimeUPSOld)) * 1000;
			std::this_thread::sleep_for(std::chrono::milliseconds(time));
		}
		Clock::lerp = lerp;
	}
	updateEnd = true;
	c.stop();
	iserver.stop();
}

void serverStart()
{
	iserver.start();
}
int frameCount = 0;
double deltaRender = 0;
void loop()
{
	//World::world->generate();
	float pos = 0;
	
	double lastTimeFPS = -1;
	double thisTimeFPS = 0;

	float volume = 1;
	bool host = Settings::getBool("host");
	std::thread server;
	if (host)
	{
		server = std::thread(serverStart);
		while (!ServerManager::ready)
		{

		}
	}
	
	
	std::thread updater(updater);
	Profiler p;
	p.init();
	double cpuUsageA = 0;
	double cpuUsageB = 1;

	double now = 0;
	double last = 0;
	float fpsCounter = 0;
	float lastFpsCounter = 0;
	int time;
	float test = 0;
	std::shared_ptr<Chunk> chunkToModel = nullptr;
	double lastTimeFPSOld = 0;
	Sound s;
	//s.play(Assets::getAudio("song"));
	float pitch = 1;
	float amt = 0.001;
	while (!window.closeRequested())
	{
		double framesPerSecond = 1.0 / GFX::FPS;
		thisTimeFPS = glfwGetTime();
		deltaRender = thisTimeFPS - lastTimeFPS;
		lastTimeFPSOld = lastTimeFPS;
		if (GFX::FPS == GFX::UNLIMITED_FPS || deltaRender >= framesPerSecond)
		{
			if (Controller::isKeyDown(GLFW_KEY_1))
			{
				pitch -= amt;
				s.setPitch(pitch);
			}
			if (Controller::isKeyDown(GLFW_KEY_2))
			{
				pitch += amt;
				s.setPitch(pitch);
			}
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
			now = glfwGetTime();
			int curFPS = 0;
			if (now > last + 0.5)
			{
				last = glfwGetTime();
				lastFpsCounter = fpsCounter;
				fpsCounter = roundf((float) (1.0 / deltaRender));
				cpuUsageA = cpuUsageB;
				cpuUsageB = p.getCurrentProcessCPU();
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
			GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)& pmc, sizeof(pmc));
			SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
			//fbo.bind();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			lastTimeFPS = thisTimeFPS;
			render();
			glEnable(GL_BLEND);

			int di = 0;
			int dim = 25;

			int rdi = 0;

			std::string fpsCap = std::to_string((int) GFX::FPS);
			if (GFX::FPS == GFX::UNLIMITED_FPS)
			{
				fpsCap = "No cap";
			}
			curFPS = (int)roundf(((fpsCounter + lastFpsCounter) / 2));
			fps = std::to_string(curFPS);
			fps += std::string(" (" + fpsCap + ")");
			GFX::drawStringBG("fps : " + fps, 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
			GFX::drawStringBGC("authenticated", 0, 0, 30, WindowManager::getWidth(), 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
			GFX::drawStringBG("x : " + Util::removeDecimal(GFX::CAM.x, 3), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
			GFX::drawStringBG("y : " + Util::removeDecimal(GFX::CAM.y, 3), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
			GFX::drawStringBG("z : " + Util::removeDecimal(GFX::CAM.z, 3), 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
			GFX::drawStringBG("c : " + std::to_string(GFX::CAM.cX) + " / " + std::to_string(GFX::CAM.cY) + " / " +
				std::to_string(GFX::CAM.cZ), 0, dim* (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

			float velocity = sqrt(pow(GFX::CAM.xVel, 2) + pow(GFX::CAM.yVel, 2) + pow(GFX::CAM.zVel, 2));
			velocity *= UPS;
			GFX::drawStringBG("v : " + Util::removeDecimal(velocity, 1) + " m/s", 0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
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
				std::to_string((int) ((w.getOverworldTime() % w.getOverworldDayCycle() / (3600)) % 60)) + ":" +
				std::to_string((int)((w.getOverworldTime() % 10000 / 60 % 60))) + " (" +
				std::to_string((int)(((w.getOverworldTime()) + 60000) / w.getOverworldDayCycle())) + ")",
				0, dim* (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
			GFX::drawStringBG("tick : " + std::to_string((int)(w.getOverworldTime())),
				0, dim * (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

			GFX::drawStringBG("physics : " + std::string("no clip"),
				0, dim* (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

			GFX::drawStringBG("fly : " + std::string("on"),
				0, dim* (di++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

			GFX::drawStringBGR("cpu usage : " + Util::removeDecimal((cpuUsageA + cpuUsageB)/2, 2) + "%",
				0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

			GFX::drawStringBGR("mem : " + Util::removeDecimal(((float) static_cast<long long>(physMemUsedByMe) / 1000000000.0), 3) + " / " +
				Util::removeDecimal((float) static_cast<long long>(totalPhysMem) / 1000000000.0, 1) + " GB"
				, 0, dim* (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

			GFX::drawStringBGR("view distance : " + std::to_string(GFX::viewDistance) + " (" + 
				std::to_string(GFX::viewDistance * Chunk::CHUNK_SIZE) + "m)", 0, dim* (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

			GFX::drawStringBGR("brightness : " + Util::removeDecimal(GFX::brightness, 2), 0, dim* (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);

			if (Profiler::showAdvancedDebugInfo)
			{
				GFX::drawStringBGR("advanced debug on", 0, dim * (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
				GFX::drawStringBGR("mipmap bias : " + std::to_string(GFX::mipmapBias), 0, dim* (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
				GFX::drawStringBGR("terrain tex res : " + std::to_string(GFX::terrainTextureResolution), 0, dim* (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
				GFX::drawStringBGR("physics objects : " + std::to_string(0), 0, dim* (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
			}
			else
			{
				GFX::drawStringBGR("advanced debug off", 0, dim* (rdi++), 30, 1, 1, 1, 1, 0, 0, 0, -5, 0, 0, 0, 0.3);
			}
			chat.render();
		}
		else
		{
			time = (framesPerSecond - (glfwGetTime() - lastTimeFPSOld)) * 1000;
			std::this_thread::sleep_for(std::chrono::milliseconds(time));
		}
	}
	
	end = true;
	updater.join();
	chunkLoader.join();
	while (iserver.running)
	{

	}
	if (host)
	{
		server.join();
	}
}

void printInfo()
{
	std::string glVersion = (char*)glGetString(GL_VERSION);
	std::string glVendor = (char*)glGetString(GL_VENDOR);
	std::string glRenderer = (char*)glGetString(GL_RENDERER);
	Log::out("[OpenGL]", "Version: " + glVersion + ", Vendor: " + glVendor + ", GPU: " + glRenderer, LBLUE);
	std::cout << "\n";
}

void loadAssetsAsync()
{
	Resource::loadAllAsyncAssets();
	
}

int main(int argc, char* argv[])
{
	Resource::DIR = std::string(argv[0]) + "\\..";
	Log::out("Game", "Loading...    UPS : " + std::to_string(UPS) + "  FPS : " + std::to_string(GFX::FPS), GREEN);
	
	if (!glfwInit())
	{
		Log::out("OpenGL", "GLFW Init() failed.", RED);
	}

	glfwSetErrorCallback(glfwErrorCallback);
	createWindow();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		printf("[Error] %s\n", glewGetErrorString(err));
		system("PAUSE");
	}

	Resource::loadBootAssets();
	
	GFX::init();

	printInfo();
	
	std::thread asyncLoader(loadAssetsAsync);

	double last = 0;
	double now = 0;

	while (!Resource::loadAllResources())
	{
		now = glfwGetTime();
		if (now - last > 1.0 / 144.0)
		{
			last = now;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0, 0, 0, 1);
			Resource::renderProgress();
			window.update();
		}
		std::this_thread::sleep_for(timeStep);
	}
	
	Resource::clearPreloadedResources();
	Shaders::lineShader->init();
	asyncLoader.join();
	WindowManager::hideMouse();
	double nn = glfwGetTime();
	Packet::loadPackets();

	GFX::drawString("", 100, 100, 50, 1, 0, 0, 1);

	w.create();
	GFX::drawString("", 100, 100, 50, 1, 0, 0, 1);
	loop();
	glfwTerminate();
	return 0;
}
