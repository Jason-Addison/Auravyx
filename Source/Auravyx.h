#pragma once
#define GLEW_MX
#include "Engine/GameManager.h"
#include "Graphics/GFX.h"
#include "Engine/WindowManager.h"
#include "Graphics/Renderer.h"
#include "Utilities\Assets.h"
#include "Modify\Modify.h"
#include "Audio/SoundManager.h"
#include "Utilities/Resource.h"
#include "Engine/OutputConsole.h"
#include "Engine/ThreadManager.h"

class Auravyx
{
public:

	Auravyx();

	GFX overlayGraphics;

	WindowManager window;

	Renderer renderer;

	Assets assets;

	Modify modify;

	SoundManager soundManager;

	Resource resources;

	OutputConsole outputConsole;

	ThreadManager threadManager;

	static Auravyx *getAuravyx();

	static GameManager gameManager;

	static GameManager getManager();

	static void start();

	static void stop();

	static void setInstance(Auravyx * a);

	Resource* getResources();

	SoundManager* getSoundManager();

	Modify* getModify();

	Assets* getAssets();

	Renderer* getRenderer();

	GFX* getOverlay();

	WindowManager* getWindow();

	OutputConsole* getConsole();

	ThreadManager* getThreadManager();

private:


	static Auravyx* instance;

	static void create();
};