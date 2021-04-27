#pragma once
#include "Auravyx/Core/World/Voxel.h"
#include "Auravyx/Graphics/Model/Model.h"
#include "Auravyx/Graphics/Camera.h"
#include <memory>
#include "Auravyx/Core/World/ChunkHeight.h"

/**
 * @brief Represents a 3D voxel chunk. Responsible for storage, generation and rendering of chunk.
*/
class Chunk
{
public:
	Chunk();
	Chunk(int x, int y, int z);
	~Chunk();
	int x, y, z;
	char lod = 1;

	static int CHUNK_SIZE;

	int size = 64;
	const int BASE_SIZE = 64;
	Chunk* neighbours[3][3][3];
	unsigned short*density = new unsigned short[262144];
	bool ready = false;
	bool loaded = false;
	bool emptyChunk = false;

	bool dataLoaded = false;

	bool generating = false;

	bool editingData = false;

	bool priorityLoad = false;

	int material;

	bool isThreadSafe = false;

	bool chunkUpdate = false;

	std::vector<std::vector<float>> chunkInfo;

	std::vector<unsigned int> allMaterials;

	std::vector<unsigned int> allLiquidMaterials;

	Model mesh;

	Model liquidMesh;

	/**
	 * @brief Generates geometry for chunk in a thread safe manner.
	 * @return vector containing all geometry data
	*/
	std::vector<std::vector<float>> generate();

	/**
	 * @brief Generates 3D density values from 2D heightmap
	 * @param heights 2D heightmap
	*/
	void generateTerrain(const std::shared_ptr<ChunkHeight>& heights);

	/**
	 * @brief Renders chunk without any setup.
	*/
	void render();

	/**
	 * @brief Returns value containing both the ID and density of a voxel at a certain location.
	 * @param x position
	 * @param y position
	 * @param z position
	 * @return value containing ID and density
	*/
	unsigned short getVoxel(const int x, const int y, const int z);

	/**
	 * @brief Returns density of a voxel at a certain location.
	 * @param x position
	 * @param y position
	 * @param z position
	 * @return voxel's density
	*/
	unsigned short getDensity(const int x, const int y, const int z);

	/**
	 * @brief Returns ID of a voxel at a certain location.
	 * @param x position
	 * @param y position
	 * @param z position
	 * @return voxel's ID
	*/
	unsigned short getVoxelID(const int x, const int y, const int z);
	
	/**
	 * @brief Sets voxel ID and density at point relative to chunk.
	 * @param x position
	 * @param y position
	 * @param z position
	 * @param id voxel ID
	 * @param density voxel density
	 * @warning no inbound checks!
	*/
	void setVoxel(char x, char y, char z, unsigned short id, unsigned short density);

	/**
	 * @brief Sets voxel ID and density at point relative to chunk and it's neighbours.
	 * @param x position
	 * @param y position
	 * @param z position
	 * @param id voxel ID
	 * @param density voxel density
	*/
	void setRelativeVoxel(int x, int y, int z, int id, int density);

	/**
	 * @brief Adds chunk to this chunk's neighbour storage.
	 * @param c chunk to add
	 * @param localChunk this chunk
	*/
	void addNeighbour(Chunk* c, Chunk* const localChunk);

	/**
	 * @brief Notify chunk that one of it's neighbour has been deleted.
	 * @param x deleted chunk position
	 * @param y deleted chunk position
	 * @param z deleted chunk position
	*/
	void receiveDeleteNotification(int x, int y, int z);

	/**
	 * @brief Sends delete notification to all neighbours.
	*/
	void sendDeleteNotification();

	/**
	 * @brief Deletes chunk and notifies neighbours.
	*/
	void destroy();

	/**
	 * @brief Regenerates chunk if needed.
	*/
	void refresh();

	/**
	 * @brief Checks if neighbours are loaded.
	 * @return true if all neighbours are loaded, false if not.
	*/
	bool neighboursLoaded();

	bool threadSafe();

	/**
	 * @brief Sets voxels between two points to ID based off strength value.
	 * @param ax  Point A's x position
	 * @param ay  Point A's y position
	 * @param az  Point A's z position
	 * @param bx Point B's x position
	 * @param by Point B's y position
	 * @param bz Point B's z position
	 * @param id voxel ID to fill
	 * @param strength fill strength
	*/
	void fill(int ax, int ay, int az, int bx, int by, int bz, int id, int strength);

	/**
	 * @brief Clears voxels between two points based off strength value.
	 * @param ax  Point A's x position
	 * @param ay  Point A's y position
	 * @param az  Point A's z position
	 * @param bx Point B's x position
	 * @param by Point B's y position
	 * @param bz Point B's z position
	 * @param strength clear strength
	*/
	void clear(int x, int y, int z, int nx, int ny, int nz, int strength);

	/**
	 * @brief Creates sphere of voxels at given point and of given radius.
	 * @param x sphere center position
	 * @param y sphere center position
	 * @param z sphere center position
	 * @param radius sphere's radius
	 * @param id voxel ID 
	*/
	void sphere(float x, float y, float z, float radius, int id);

private:

	char BORDER_WITH_UNLOADED_CHUNK = -1;

	bool isValidNormal(const Voxel* vert);
	bool isValid(const Voxel* vert);

	Voxel* at(int x, int y, int z);

	void clearDensity();

	int relativeDensity(int x, int y, int z);

	int getRelativeVoxelID(int x, int y, int z);

	void updateRelativeVoxel(int x, int y, int z);

	std::vector<std::vector<float>> generateLiquid();

	void addVertex(const Vec3f& pos, std::vector<float>& vec, const float x, const float y, const float z);

	void addTriangle(const Voxel* v1, const Voxel* v2, const Voxel* v3, const Voxel* v4, std::vector<float>& vec, const float x, const float y, const float z);

	void addTriangleL(const Voxel* v1, const Voxel* v2, const Voxel* v3, const Voxel* v4, std::vector<float>& vec, const float x, const float y, const float z);

	float calcDensity(int x, int y, int z, int xX, int yY, int zZ);

	int scan(int x, int y, int z, int axis, int axisA, int axisB, int& material);

	void zeroNeighbours();
};

