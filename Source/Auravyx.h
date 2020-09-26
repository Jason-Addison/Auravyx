#pragma once
#define GLFW_DLL
#include "Engine/GameManager.h"
#include "Graphics/GFX.h"
#include "Engine/WindowManager.h"
#include "Graphics/Renderer.h"
#include "Utilities\Assets.h"
#include "Modify\Modify.h"
#include "Audio/SoundManager.h"

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

	SoundManager* getSoundManager();

	Modify* getModify();

	Assets* getAssets();

	Renderer* getRenderer();

	GFX *getOverlay();

	WindowManager* getWindow();

	void draw();

	static Auravyx *getAuravyx();


	static GameManager gameManager;

	static GameManager getManager();

	static void start();

	static void stop();

	static void setInstance(Auravyx * a);

private:


	static Auravyx* instance;
};