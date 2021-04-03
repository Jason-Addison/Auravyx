#pragma once
#include "World/Chunk.h"
#include <vector>
#include <memory>
#include "Engine/Camera.h"
#include <atomic>
#include "World/ChunkHeight.h"
class World
{
public:
	World();
	~World();

	std::vector<std::unique_ptr<Chunk>> overworld;

	std::vector<std::shared_ptr<ChunkHeight>> overworldHeightmap;

	std::atomic_int unloadLock = false;

	Model tMesh;

	void generate();

	void update();

	void create();

	void render(const Camera& cam, const Matrix4f& projectionMatrix);
	
	void addChunk(const Chunk& chunk);

	Chunk* getChunk(const int x, const int y, const int z);

	std::shared_ptr<ChunkHeight> getChunkHeightmap(const int x, const int z);

	bool isLoaded(const int x, const int y, const int z);

	void sphere(const float xP, const float yP, const float zP, const float radius, const float power);

	void cuboid(const float xP, const float yP, const float zP, const float xS, const float yS, const float zS, const float power);

	std::shared_ptr<World> getOverworld();

	void setVoxel(const int x, const int y, const int z, const int type, const float density);

	void setOverworldTime(const long long int time);

	long long int getOverworldTime();

	long long int getOverworldDayCycle();

	void test();

	Chunk *getChunk(const int i);

	void setup();

private:

	long long int overworldDayCycle = 240000;

	long long int overworldTime = (long long int) (240000.0 * 10.32);
};

