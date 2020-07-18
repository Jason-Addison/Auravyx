#pragma once
#include "Chunk.h"
#include <vector>
#include <memory>
#include "Camera.h"
#include <atomic>
#include "ChunkHeight.h"
class World
{
public:
	World();
	~World();

	std::vector<std::shared_ptr<Chunk>> overworld;

	std::vector<std::shared_ptr<ChunkHeight>> overworldHeightmap;

	std::atomic_int unloadLock = false;

	Model tMesh;

	void generate();

	void update();

	void create();

	void render(Camera * cam, Matrix4f* projectionMatrix);
	
	void addChunk(std::shared_ptr<Chunk> chunk);

	std::shared_ptr<Chunk> getChunk(int x, int y, int z);

	std::shared_ptr<ChunkHeight> getChunkHeightmap(int x, int z);

	bool isLoaded(int x, int y, int z);

	void sphere(float xP, float yP, float zP, float radius, float power);

	void cuboid(float xP, float yP, float zP, float xS, float yS, float zS, float power);

	std::shared_ptr<World> getOverworld();

	void setVoxel(int x, int y, int z, int type, float density);

	void setOverworldTime(long long int time);

	long long int getOverworldTime();

	long long int getOverworldDayCycle();

private:

	long long int overworldDayCycle = 240000;

	long long int overworldTime = 240000 * 10.2;

};

