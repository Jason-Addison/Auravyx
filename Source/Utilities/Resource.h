#pragma once
#include <map>
#include <string>
#include <vector>
#include "Utilities/Vec3f.h"
class Resource
{
public:
	Resource();
	~Resource();

	static bool loadAllResources();

	static std::map<std::string, std::string> loadShaders(std::string dir);

	static std::string DIR;

	static void loadBootAssets();

	static int loadTexture(std::string dir);

	static void loadTextureSilent(std::string texture);

	static void loadAllAsyncAssets();

	static void renderProgress();

	static void clearPreloadedResources();

	static void printLoadingMessage(std::vector<std::string> lines, double delay, std::vector<Vec3f> colours);

	static void cleanupResources();

private:

	static void loadAllTextures();

	static void loadAllTerrainTextures();

	static void loadAllModels();

	static void loadAllAudio();

	static void loadAllMods();

};

