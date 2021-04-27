#pragma once

/*
 *  ----------------------------------------------------------------
 *	   ,---.
 *	  /  O  \ ,--.,--.,--.--. ,--,--.,--.  ,--.,--. ,--.,--.  ,--.
 *	 |  .-.  ||  ||  ||  .--'' ,-.  | \  `'  /  \  '  /  \  `'  /
 *	 |  | |  |'  ''  '|  |   \ '-'  |  \    /    \   '   /  /.  \
 *	 `--' `--' `----' `--'    `--`--'   `--'   .-'  /   '--'  '--'
 *	                                           `---'
 *  ----------------------------------------------------------------
 *
 * Authour: Jason Addison
 * https://github.com/Jason-Addison
 *
 * Auravyx is a voxel based 3D sandbox game written in C++ using OpenGL and OpenAL.
 *
 */

#define GLEW_MX
#include "Engine/GameManager.h"
#include "Graphics/GFX.h"
#include "Engine/Window.h"
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

	static Auravyx& getInstance();

	static GameManager gameManager;

	static GameManager getManager();

	static void start();

	static void stop();

	static void setInstance(const Auravyx& auravyx);

	Resource* getResources();

	SoundManager* getSoundManager();

	Modify* getModify();

	Assets* getAssets();

	Renderer* getRenderer();

	GFX* getOverlay();

	Window* getWindow();

	OutputConsole* getConsole();

	ThreadManager* getThreadManager();

	/**
	 * @brief
	 * @param
	 * @warning Do not use this
	*/
	Auravyx(Auravyx const&) = delete;
	void operator=(Auravyx const&) = delete;

private:

	Auravyx() = default;
	/**
	 * @brief Game's context, which is sent to mod DLLs so they can use the same environement.
	*/


	/**
	 * @brief Set singleton instances for current context. This allows for DLLs to communicate properly.
	*/
	static void setupAllInstances();

	/**
	 * @brief Singletons for game's core functionalities.
	*/
	GFX overlayGraphics;

	Window window;

	Renderer renderer;

	Assets assets;

	Modify modify;

	SoundManager soundManager;

	Resource resources;

	OutputConsole outputConsole;

	ThreadManager threadManager;
};