#include "stdafx.h"
#define GLFW_DLL
#include "Auravyx.h"
#include "GL/glew.h"
#include "Utilities/Log.h"
#include "Audio/Sound.h"
#include "Utilities/Resource.h"
#include "Shader/Shaders.h"
#include "Engine/Clock.h"
#include <thread>
#include "Engine/Mouse.h"
#include "Utilities/Settings.h"
#include <Utilities/Profiler.h>
#include <Engine/State.h>
#include <iostream>
#include <Engine/GameManager.h>
#include <Engine/GLManager.h>
#include <wtypes.h>
#include <Engine/Controller.h>
#include <Engine/GameState.h>
#include <Modify/Modify.h>
double thisFrame = 0;
double nextFrame = 0;


std::chrono::duration<long, std::milli> timeStep = std::chrono::milliseconds(1);

GameManager Auravyx::gameManager;

auto start = std::chrono::high_resolution_clock::now();

bool end = false;
bool updateEnd = false;

Auravyx* Auravyx::instance = new Auravyx();

void updater()
{
	double lerp = 0;
	double updatesPerSecond = 1 / GameManager::getUPS();
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
			lastTimeUPS = thisTimeUPS;
			Auravyx::getManager().getCurrentState()->update();
			m.update(&(GameManager::world));
		}
		else
		{
			time = (updatesPerSecond - (glfwGetTime() - lastTimeUPSOld)) * 1000;
			std::this_thread::sleep_for(std::chrono::milliseconds(time));
		}
		Clock::lerp = lerp;
		m.update(&(GameManager::world));
	}
	updateEnd = true;
}

void loop()
{
	GameState gs;
	Auravyx::getManager().setCurrentState(std::make_shared<GameState>(gs));
	Auravyx::getManager().getCurrentState()->start();

	double lastTimeFPS = -1;
	double thisTimeFPS = 0;

	std::thread updater(updater);

	double deltaRender = 0;
	float lastFpsCounter = 0;
	int time;

	double lastTimeFPSOld = 0;
	while (!Auravyx::getAuravyx()->getWindow()->closeRequested())
	{
		double framesPerSecond = 1.0 / Auravyx::getAuravyx()->getOverlay()->FPS;
		thisTimeFPS = glfwGetTime();
		deltaRender = thisTimeFPS - lastTimeFPS;
		lastTimeFPSOld = lastTimeFPS;
		
		if (Auravyx::getAuravyx()->getOverlay()->FPS == Auravyx::getAuravyx()->getOverlay()->UNLIMITED_FPS || deltaRender >= framesPerSecond)
		{
			Auravyx::getAuravyx()->getWindow()->update();
			Auravyx::getManager().getCurrentState()->render();
			//mod.render();
			lastTimeFPS = thisTimeFPS;
			GLManager::setFPS(roundf((float)(1.0 / (deltaRender))));
		}
		else
		{
			time = (framesPerSecond - (glfwGetTime() - lastTimeFPSOld)) * 1000;
			std::this_thread::sleep_for(std::chrono::milliseconds(time));
		}
	}
	end = true;
	
	Auravyx::getManager().getCurrentState()->stop();
	updater.join();
}

void loadAssetsAsync()
{
	Resource::loadAllAsyncAssets();
}

int main(int argc, char* argv[])
{
	Auravyx::start();
	Resource::DIR = std::string(argv[0]) + "\\..";
	GLManager::start();
	
	Resource::loadBootAssets();
	Auravyx::getAuravyx()->getOverlay()->init();
	std::thread asyncLoader(loadAssetsAsync);

	Profiler::init();

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
			Auravyx::getAuravyx()->getWindow()->update();
		}
		std::this_thread::sleep_for(timeStep);
	}
	Resource::clearPreloadedResources();
	Auravyx::getAuravyx()->getRenderer()->getShaders()->lineShader->init();
	asyncLoader.join();
	Auravyx::getAuravyx()->getWindow()->hideMouse();

	Auravyx::getAuravyx()->getWindow()->getController()->resetMouse();
	loop();

	Resource::cleanupResources();
	Auravyx::stop();
	glfwTerminate();
	return 0;
}
#ifdef _WIN32
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	return main(__argc, __argv);
}
#endif

Auravyx::Auravyx()
{

}

SoundManager* Auravyx::getSoundManager()
{
	return &soundManager;
}

Modify* Auravyx::getModify()
{
	return &modify;
}

Assets* Auravyx::getAssets()
{
	return &assets;
}

Renderer* Auravyx::getRenderer()
{
	return &renderer;
}

GFX* Auravyx::getOverlay()
{
	return &overlayGraphics;
}

WindowManager* Auravyx::getWindow()
{
	return &window;
}

void Auravyx::draw()
{
	printf("no");
	Auravyx::getAuravyx()->getOverlay()->fillRect(0, 0, 50, 500, 1, 0, 1, 1);
}

Auravyx* Auravyx::getAuravyx()
{
	return instance;
}

GameManager Auravyx::getManager()
{
	return Auravyx::gameManager;
}

void Auravyx::start()
{
}

void Auravyx::stop()
{
	delete instance;
}

void Auravyx::setInstance(Auravyx* a)
{
	instance = a;
}
