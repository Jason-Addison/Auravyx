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

	std::map<std::string, std::string> loadShaders(const std::string& dir);

	std::string DIR;

	void loadBootAssets();

	int loadTexture(const std::string& dir);

	void loadTextureSilent(const std::string& texture);

	void loadAllAsyncAssets();

	void renderProgress();

	void clearPreloadedResources();

	void printLoadingMessage(const std::vector<std::string>& lines, const double delay, const std::vector<Vec3f>& colours);

	void cleanupPrimaryResources();

	void cleanupRemainingResources();

	static Resource& getInstance();

private:

	void loadAllTextures();

	void loadAllTerrainTextures();

	void loadAllModels();

	void loadAllAudio();

	void loadAllMods();

};

