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

	std::shared_ptr<ChunkHeight> getChunkHeightmap(const int x, const int z);

	bool isLoaded(const int x, const int y, const int z);

	/**
	 * @brief Creates a sphere of a voxel at a coordinate.
	 * @param x position
	 * @param y position
	 * @param z position
	 * @param type voxel's ID
	 * @param density minimum density of voxels
	 * @param radius distance from inner sphere to edge
	 * @param strength speed of operation
	*/
	void sphere(const float x, const float y, const float z, const short type, const short density, const float radius, const float strength);

	/**
	 * @brief Fills a region of voxels between two points.
	 * @param xA first position
	 * @param yA first position 
	 * @param zA first position 
	 * @param xB second position 
	 * @param yB second position  
	 * @param zB second position  
	 * @param density minimum density of voxels
	 * @param radius distance from inner sphere to edge
	 * @param strength speed of operation
	*/
	void cuboid(const float xA, const float yA, const float zA, const float xB, const float yB, const float zB, const short type, const short density, const float strength);

	/**
	 * @brief 
	 * @return
	 * @TODO add this
	*/
	std::shared_ptr<World> getOverworld();

	/**
	 * @brief Set a voxel at any position in the world.
	 * @param x position
	 * @param y position
	 * @param z position
	 * @param type voxel's ID
	 * @param density voxel's density
	*/
	void setVoxel(const int x, const int y, const int z, const int type, const float density);

	/**
	 * @brief Sets the current time in ticks of the world.
	 * @param time tick value
	*/
	void setOverworldTime(const long long int time);

	/**
	 * @brief Return the current time in ticks in the current day.
	 * @return current time in ticks
	*/
	long long int getOverworldTime();

	/**
	 * @brief Return the amount of ticks in a full day cycle.
	 * @return ticks in a full day
	*/
	long long int getOverworldDayCycle();

	/**
	 * @brief Return chunk at an index.
	 * @param i index
	 * @return chunk at index
	*/
	Chunk *getChunk(const int i);

	/**
	 * @brief Return chunk at a position in terms of chunk units.
	 * @param x position
	 * @param y position
	 * @param z position
	 * @return chunk at the position
	*/
	Chunk* getChunk(const int x, const int y, const int z);

	void test();

	void setup();

private:

	/**
	 * @brief Full day cycle for the world in ticks.
	*/
	long long int overworldDayCycle = 240000;

	/**
	 * @brief The current time in ticks of the world.
	*/
	long long int overworldTime = (long long int) (240000.0 * 10.32);
};

