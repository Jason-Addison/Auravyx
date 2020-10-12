#pragma once
#include "World/Voxel.h"
#include "Engine/Model.h"
#include "Engine/Camera.h"
#include <memory>
#include "World/ChunkHeight.h"
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

	bool priorityLoad = false;

	int material;

	bool isThreadSafe = false;

	bool chunkUpdate = false;

	std::vector<std::vector<float>> chunkInfo;

	std::vector<unsigned int> allMaterials;

	Model mesh;

	std::vector<std::vector<float>> generate();

	void generateTerrain(std::shared_ptr<ChunkHeight> heights);

	void render(Camera camera, Matrix4f projectionMatrix);

	void addVertex(Vec3f pos, std::vector<float>* vec, float x, float y, float z);

	void addTriangle(Voxel* v1, Voxel* v2, Voxel* v3, Voxel* v4, std::vector<float>* vec, float x, float y, float z);

	unsigned short getDensity(int x, int y, int z);

	unsigned short getVoxelID(int x, int y, int z);

	unsigned short getVoxel(int x, int y, int z);

	float calcDensity(int x, int y, int z, int xX, int yY, int zZ);

	int scan(int x, int y, int z, int axis, int axisA, int axisB, int &material);
		
	void destroy();

	void refresh();

	void sphere(float xP, float yP, float zP, float radius, int id);

	void receiveDeleteNotification(int x, int y, int z);

	void sendDeleteNotification();

	void addNeighbour(Chunk* c, Chunk* localChunk);

	bool neighboursLoaded();

	void setDensity(int x, int y, int z, float density);

	void setDensity(int x, int y, int z, unsigned short id, unsigned short density);

	void clearDensity();

	bool threadSafe();

	void fill(int x, int y, int z, int nx, int ny, int nz, int id, int density);

	static void setPredef();

	void setRelativeVoxel(int x, int y, int z, int id, int density);

	int getRelativeVoxelID(int x, int y, int z);

private:

	char BORDER_WITH_UNLOADED_CHUNK = -1;

	bool isValidNormal(Voxel* vert);
	bool isValid(Voxel* vert);

	Voxel* at(int x, int y, int z);

	int relativeDensity(int x, int y, int z);

	void zeroNeighbours();
};

