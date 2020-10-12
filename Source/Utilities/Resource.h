#pragma once
#include <map>
#include <string>
#include <vector>
#include "Utilities/Vec3f.h"
class Resource
{
public:
	Resource();
	Resource(Resource* r);
	~Resource();

	bool loadAllResources();

	std::map<std::string, std::string> loadShaders(std::string dir);

	std::string DIR;

	void loadBootAssets();

	int loadTexture(std::string dir);

	void loadTextureSilent(std::string texture);

	void loadAllAsyncAssets();

	void renderProgress();

	void clearPreloadedResources();

	void printLoadingMessage(std::vector<std::string> lines, double delay, std::vector<Vec3f> colours);

	void cleanupPrimaryResources();

	void cleanupRemainingResources();

	static Resource* getResources();

private:

	static Resource* resource;

	void loadAllTextures();

	void loadAllTerrainTextures();

	void loadAllModels();

	void loadAllAudio();

	void loadAllMods();

};

