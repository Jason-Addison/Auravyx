#include "Auravyx/Core/Auravyx.h"
#include "Logger/Log.h"
#include "Auravyx/Core/Clock.h"
#include "Auravyx/UI/Control/Mouse.h"
#include "Auravyx/Utility/Profiler.h"
#include "Auravyx/Graphics/GL/GLManager.h"
#include "Auravyx/Core/State/GameState.h"
#include "Auravyx/Graphics/Model/Collada/ColladaParser.h"

GameManager Auravyx::gameManager;

std::atomic_bool end = false;
std::atomic_bool updateEnd = false;

void updater()
{
	ThreadManager::getThreadManager()->registerThread(std::this_thread::get_id(), "Updater");
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
			m.update((GameManager::world));
		}
		else
		{
			time = (updatesPerSecond - (glfwGetTime() - lastTimeUPSOld)) * 1000;
			std::this_thread::sleep_for(std::chrono::milliseconds(time));
		}
		Clock::lerp = lerp;
		m.update((GameManager::world));
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
	double lastTimeFPSOld = 0;

	while ((!Window::getWindow()->closeRequested() && !OutputConsole::getConsole()->shutdown))
	{
		double framesPerSecond = 1.0 / GFX::getOverlay()->FPS;
		thisTimeFPS = glfwGetTime();
		deltaRender = thisTimeFPS - lastTimeFPS;
		lastTimeFPSOld = lastTimeFPS;
		Clock::DELTA = deltaRender;
		if (GFX::getOverlay()->FPS == GFX::getOverlay()->UNLIMITED_FPS || deltaRender >= framesPerSecond)
		{
			Window::getWindow()->update();
			Auravyx::getManager().getCurrentState()->render();
			//mod.render();
			lastTimeFPS = thisTimeFPS;
			GLManager::setFPS(roundf((float)(1.0 / (deltaRender))));
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds((int)((framesPerSecond - (glfwGetTime() - lastTimeFPSOld)) * 1000)));
		}
	}
	if (Window::getWindow()->closeRequested())
	{
		Log::out("Closing main window because it was requested.");
	}
	if (OutputConsole::getConsole()->shutdown)
	{
		Log::out("Closing main window because of console shutdown.");
	}
	end = true;
	Auravyx::getManager().getCurrentState()->stop();
	updater.join();
}

void loadAssetsAsync()
{
	ThreadManager::getThreadManager()->registerThread(std::this_thread::get_id(), "Async Loader");
	Resource::getInstance().loadAllAsyncAssets();
}
int main(int argc, char* argv[])
{
	Auravyx::start();
	ThreadManager::getThreadManager()->registerThread(std::this_thread::get_id(), "Main");
	Resource::getInstance().DIR = std::string(argv[0]) + "\\..";
	#ifdef NDEBUG
	#else
	Log::out("[Debug] : [!] Auravyx is running in debug mode, expect very slow world generation!", RED);
	#endif
	GameManager::world.setup();
	
	OutputConsole::getConsole()->start();

	while (!OutputConsole::getConsole()->isReady())
	{

	}
	GLManager::start();
	std::thread asyncLoader(loadAssetsAsync);
	GFX::getOverlay()->init();
	Resource::getInstance().loadBootAssets();
	Profiler::init();
	double last = 0;
	double now = 0;
	
	Window::getWindow()->update();

	while (!Resource::getInstance().loadAllResources())
	{
		now = glfwGetTime();
		if (now - last > 1.0 / 144.0)
		{
			last = now;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.1, 0.1, 0.1, 1);
			Resource::getInstance().renderProgress();
			
			Window::getWindow()->update();
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	//AnimatedMesh meshes = ColladaParser::parse("C:\\Users\\jason\\Downloads\\Audi\\cottage.dae");
	//AnimatedMesh meshes = ColladaParser::parse("C:\\Users\\jason\\Downloads\\Zelda\\Zeld\\Zelda.dae");
	//Model m = Model::loadIndexed3DModel(meshes.vertices, meshes.normals, meshes.textureCoords, meshes.colors, meshes.indices);
	//m.setMaterials(meshes.materials);

	//Assets::getAssets()->addModel("Zelda", m);

	Resource::getInstance().clearPreloadedResources();
	Renderer::getRenderer()->getShaders()->lineShader->init();
	asyncLoader.join();
	Window::getWindow()->hideMouse();

	Window::getWindow()->getController()->resetMouse();
	
	loop();

	Log::out("Cleanup", "Cleaning up audio...", LIGHT_GRAY);
	SoundManager::getSoundManager()->stop();
	Resource::getInstance().cleanupPrimaryResources();
	OutputConsole::getConsole()->cleanup();
	Resource::getInstance().cleanupRemainingResources();
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

Window* Auravyx::getWindow()
{
	return &window;
}

OutputConsole* Auravyx::getConsole()
{
	return &outputConsole;
}

ThreadManager* Auravyx::getThreadManager()
{
	return &threadManager;
}

Auravyx& Auravyx::getInstance()
{
	static Auravyx instance;
	return instance;
}

GameManager Auravyx::getManager()
{
	return Auravyx::gameManager;
}

void Auravyx::start()
{
	setupAllInstances();
}

void Auravyx::stop()
{
	
}

void Auravyx::setInstance(const Auravyx& auravyx)
{
	//instance = auravyx;
	setupAllInstances();
}

Resource* Auravyx::getResources()
{
	return &resources;
}

void Auravyx::setupAllInstances()
{
	GFX(getInstance().getOverlay());
	Assets(getInstance().getAssets());
	Window(getInstance().getWindow());
	Renderer(getInstance().getRenderer());
	Modify(getInstance().getModify());
	SoundManager(getInstance().getSoundManager());
	//Resource(getInstance().getResources());
	OutputConsole(getInstance().getConsole());
	ThreadManager(getInstance().getThreadManager());
}
