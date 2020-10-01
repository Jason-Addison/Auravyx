#pragma once
#include "Engine/GameManager.h"
#include "Graphics/GFX.h"
#include "Engine/WindowManager.h"
#include "Graphics/Renderer.h"
#include "Utilities\Assets.h"
#include "Modify\Modify.h"
#include "Audio/SoundManager.h"
#include "Utilities/Resource.h"

class Auravyx
{
public:

	Auravyx();

	int valll = 8;

	GFX overlayGraphics;

	WindowManager window;

	Renderer renderer;

	Assets assets;

	Modify modify;

	SoundManager soundManager;

	Resource resources;

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

private:


	static Auravyx* instance;

	static void create();
};