#pragma once
#include "Voxel.h"
#include "Model.h"
#include "Camera.h"
#include <memory>
#include "ChunkHeight.h"
class Chunk
{
public:
	Chunk();
	Chunk(int x, int y, int z);
	~Chunk();
	int x, y, z;
	char lod = 1;

	static int CHUNK_SIZE;

	int size = 128;
	const int BASE_SIZE = 128;
	std::shared_ptr<Chunk> neighbours[3][3][3];
	short *density = new short[2097152];
	bool ready = false;
	bool loaded = false;
	bool emptyChunk = false;
	int material;

	bool chunkUpdate = false;

	std::vector<std::vector<float>> chunkInfo;

	std::vector<unsigned int> allMaterials;

	Model mesh;

	std::vector<std::vector<float>> generate();

	void generateTerrain(std::shared_ptr<ChunkHeight> heights);

	void render(Camera camera, Matrix4f projectionMatrix);

	void addVertex(Vec3f pos, std::vector<float>* vec, float x, float y, float z);

	void addTriangle(Voxel* v1, Voxel* v2, Voxel* v3, Voxel* v4, std::vector<float>* vec, float x, float y, float z);

	float getDensity(int x, int y, int z);

	float calcDensity(int x, int y, int z, int xX, int yY, int zZ);

	int scan(int x, int y, int z, int axis, int axisA, int axisB, int &material);
		
	void destroy();

	void refresh();

	void sphere(float xP, float yP, float zP, float radius, float power);

	void receiveDeleteNotification(int x, int y, int z);

	void sendDeleteNotification();

	void addNeighbour(std::shared_ptr<Chunk> c, std::shared_ptr<Chunk> localChunk);

	bool neighboursLoaded();

	void setDensity(int x, int y, int z, float density);

	void clearDensity();

private:

	char BORDER_WITH_UNLOADED_CHUNK = -1;

	bool isValidNormal(Voxel* vert);
	bool isValid(Voxel* vert);

	Voxel* at(int x, int y, int z);

	float relativeDensity(int x, int y, int z);

	void zeroNeighbours();
};

