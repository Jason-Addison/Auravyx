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
#include "Auravyx/Core/GameManager.h"
#include "Auravyx/Graphics/GFX.h"
#include "Auravyx/UI/Window.h"
#include "Auravyx/Graphics/Renderer.h"
#include "Auravyx/Core/Assets.h"
#include "Auravyx/Modify/Modify.h"
#include "Auravyx/Audio/SoundManager.h"
#include "Auravyx/Utility/IO/Resource.h"
#include "Logger/OutputConsole.h"
#include "Auravyx/Core/ThreadManager.h"

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