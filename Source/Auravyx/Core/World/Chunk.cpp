#include "Auravyx/Core/World/Chunk.h"
#include <iostream>
#include "Auravyx/Utility/Math/M.h"
#include <chrono>
#include "Auravyx/Utility/Math/RandomNoise.h"
#include <Logger/Log.h>
#include "Auravyx/Core/Clock.h"
Chunk::Chunk()
{
	zeroNeighbours();
}

Chunk::Chunk(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	zeroNeighbours();
}


Chunk::~Chunk()
{

}

int Chunk::CHUNK_SIZE = 64;

void addN(const Vec3f& n, std::vector<float>* vec)
{
	vec->emplace_back(n.x);
	vec->emplace_back(n.y);
	vec->emplace_back(n.z);
}
void addN(const Voxel* const n, std::vector<float>* vec)
{
	vec->emplace_back(n->fn.x);
	vec->emplace_back(n->fn.y);
	vec->emplace_back(n->fn.z);
}
void addNL(const Voxel* const n, std::vector<float>* vec)
{
	vec->emplace_back(n->fnL.x);
	vec->emplace_back(n->fnL.y);
	vec->emplace_back(n->fnL.z);
}

void addMaterial(Voxel* n, int material, std::vector<unsigned int>* mat)
{
	mat->emplace_back(material);
}
void addNormalFace(Voxel* n1, Voxel* n2, Voxel* n3, Voxel* n4, std::vector<float>* vec, char type, bool flipXLock, bool flipYLock, bool flipZLock, Vec3f a, Vec3f b)
{
	if (((type == 0) * flipXLock * (n3->flipX || n2->flipX)) +
		((type == 0) * flipYLock * (n3->flipY || n1->flipY)) +
		((type == 0) * flipZLock * (n3->flipZ || n2->flipZ)))
	{
		a.flip();
		b.flip();
	}
	n1->fn.add(a);
	n2->fn.add(a);
	n3->fn.add(a);

	n4->fn.add(b);
	n3->fn.add(b);
	n2->fn.add(b);
}
void addNormalFaceL(Voxel* n1, Voxel* n2, Voxel* n3, Voxel* n4, std::vector<float>* vec, char type, bool flipXLock, bool flipYLock, bool flipZLock, Vec3f a, Vec3f b)
{
	if (((type == 0) * flipXLock * (n3->flipLX || n2->flipLX)) +
		((type == 0) * flipYLock * (n3->flipLY || n1->flipLY)) +
		((type == 0) * flipZLock * (n3->flipLZ || n2->flipLZ)))
	{
		a.flip();
		b.flip();
	}
	n1->fnL.add(a);
	n2->fnL.add(a);
	n3->fnL.add(a);

	n4->fnL.add(b);
	n3->fnL.add(b);
	n2->fnL.add(b);
}
bool Chunk::isValidNormal(const Voxel* vert)
{
	if (vert->x >= -1 && vert->y >= -1 && vert->z >= -1 &&
		vert->x < BASE_SIZE + 3 && vert->y < BASE_SIZE + 3 && vert->z < BASE_SIZE + 3)
	{
		return true;
	}
	return true;
}
bool Chunk::isValid(const Voxel* vert)
{
	if (vert->x > 0 && vert->y > 0 && vert->z > 0 &&
		(float) vert->x < (float) size - 4.9 && (float) vert->y < (float) size - 4.9 && (float) vert->z < (float) size - 4.9)
	{
		return true;
	}
	return false;
}
void Chunk::addVertex(const Vec3f& pos, std::vector<float>& vec, const float x, const float y, const float z)
{
	vec.emplace_back((pos.x * lod + CHUNK_SIZE * x - 1.5) * 1);
	vec.emplace_back((pos.y * lod + CHUNK_SIZE * y - 1.5) * 1);
	vec.emplace_back((pos.z * lod + CHUNK_SIZE * z - 1.5) * 1);
}

void Chunk::addTriangle(const Voxel* v1, const Voxel* v2, const Voxel* v3, const Voxel* v4, std::vector<float>& vec, const float x, const float y, const float z)
{
	addVertex(v1->smoothAverage, vec, x, y, z);
	addVertex(v2->smoothAverage, vec, x, y, z);
	addVertex(v3->smoothAverage, vec, x, y, z);

	addVertex(v4->smoothAverage, vec, x, y, z);
	addVertex(v3->smoothAverage, vec, x, y, z);
	addVertex(v2->smoothAverage, vec, x, y, z);
}
void Chunk::addTriangleL(const Voxel* v1, const Voxel* v2, const Voxel* v3, const Voxel* v4, std::vector<float>& vec, const float x, const float y, const float z)
{
	addVertex(v1->smoothLiquidAverage, vec, x, y, z);
	addVertex(v2->smoothLiquidAverage, vec, x, y, z);
	addVertex(v3->smoothLiquidAverage, vec, x, y, z);

	addVertex(v4->smoothLiquidAverage, vec, x, y, z);
	addVertex(v3->smoothLiquidAverage, vec, x, y, z);
	addVertex(v2->smoothLiquidAverage, vec, x, y, z);
}
Voxel* v;
Voxel* Chunk::at(int x, int y, int z)
{
	return &v[x + size * (y + size * z)];
}

int Chunk::relativeDensity(int x, int y, int z)
{
	x -= 1;
	y -= 1;
	z -= 1;
	int cX = x >> 6;
	int cY = y >> 6;
	int cZ = z >> 6;

	if (cX == 0 && cY == 0 && cZ == 0)
	{
		int rX = x - (cX << 6);
		int rY = y - (cY << 6);
		int rZ = z - (cZ << 6);

		return getVoxel(rX, rY, rZ);
	}
	if (neighbours[cX + 1][cY + 1][cZ + 1] != nullptr && !neighbours[cX + 1][cY + 1][cZ + 1]->emptyChunk)
	{
		int rX = x - (cX << 6);
		int rY = y - (cY << 6);
		int rZ = z - (cZ << 6);
		return neighbours[cX + 1][cY + 1][cZ + 1]->getVoxel(rX, rY, rZ);
	}
	return -1;
}

void Chunk::zeroNeighbours()
{
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			for (int z = 0; z < 3; z++)
			{
				neighbours[x][y][z] = nullptr;
			}
		}
	}
}
unsigned short Chunk::getDensity(int x, int y, int z)
{
	return getVoxel(x, y, z) & 0x00ff;
}
unsigned short Chunk::getVoxelID(int x, int y, int z)
{
	return getVoxel(x, y, z) >> 8;
}
unsigned short Chunk::getVoxel(int x, int y, int z)
{
	return density[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)];
}
float Chunk::calcDensity(int x, int y, int z, int xX, int yY, int zZ)
{
	float d1 = -1;
	float d2 = -1;

	d1 = (float) (relativeDensity(x * lod, y * lod, z * lod) & 0x00ff) / (float) 255;
	d2 = (float) (relativeDensity(xX * lod, yY * lod, zZ * lod) & 0x00ff) / (float) 255;
	
	float interp = (d1 + d2);
	if (d1 == 0 && d2 == 0)
	{
		return 0.5;
	}
	if (d1 < d2)
	{
		interp = 1 - interp;
	}
	return interp;
}

bool air(int id)
{
	if (id == 0 || id == 100)
	{
		return true;
	}
	return false;
}

bool liquid(int id)
{
	if (id == 100)
	{
		return true;
	}
	return false;
}
int fiA = 0;
int fiB = 0;
bool a = 0;
bool b = 0;
int Chunk::scan(int x, int y, int z, int axis, int axisA, int axisB, int &material)
{
	x = (x - 1) * lod;
	y = (y - 1) * lod;
	z = (z - 1) * lod;

	if (axisA >= 0 && axisB >= 0)
	{
		fiA = (relativeDensity(x, y, z)) >> 8;
		fiB = (relativeDensity(x + ((axis == 0) * lod), y + ((axis == 1) * lod), z + ((axis == 2) * lod))) >> 8;
		a = air(fiA);
		b = air(fiB);

		if (fiA < 0 && fiB < 0)
		{
			return -1;
		}
		if (a && !b)
		{
			material = fiB;
			return fiB;
		}
		if (!a && b)
		{
			material = fiA;
			return fiA;
		}
		if (fiA == 100 && fiB == 0)
		{
			material = 100;
			return 100;
		}
		if (fiB == 100 && fiA == 0)
		{
			material = 100;
			return 100;
		}
	}

	return 0;
}

void Chunk::destroy()
{
	//sendDeleteNotification();
}
void Chunk::refresh()
{
	if (loaded && chunkInfo.size() != 0)
	{
		mesh.destroy();
		liquidMesh.destroy();
		//std::cout << chunkInfo.size() << "\n";
		mesh = Model::load3DModel(chunkInfo.at(0), chunkInfo.at(1), chunkInfo.at(2), chunkInfo.at(3), chunkInfo.at(4), allMaterials);
		
		liquidMesh = Model::load3DModel(chunkInfo.at(5), chunkInfo.at(6), chunkInfo.at(7), chunkInfo.at(8), chunkInfo.at(9), allLiquidMaterials);
		ready = true;
		//loaded = false;
		chunkInfo = std::vector<std::vector<float>>();
		allMaterials = std::vector<unsigned int>();
		allLiquidMaterials = std::vector<unsigned int>();
	}
}
void Chunk::sphere(float xP, float yP, float zP, float radius, int id)
{
	for (int x = (int) floor(-radius); x < (int) ceil(radius); x++)
	{
		for (int z = (int) floor(-radius); z < (int) ceil(radius); z++)
		{
			for (int y = (int) floor(-radius); y < (int) ceil(radius); y++)
			{
				int nX = x + xP;
				int nY = y + yP;
				int nZ = z + zP;
				float distance = abs(M::distance(Vec3f(xP, yP, zP), Vec3f(nX, nY, nZ)));

				if (distance < radius)
				{
					setVoxel(nX, nY, nZ, 6, ((distance) - (float) (int) (distance)) * 255);
					//std::cout << (((distance)-(float)(int)(distance)) * 255) << "\n";
				}
				if (distance <= radius - 0.75)
				{
					setVoxel(nX, nY, nZ, 5, 255);
				}
			}
		}
	}
}
void Chunk::receiveDeleteNotification(int x, int y, int z)
{
	int cX = x - this->x;
	int cY = y - this->y;
	int cZ = z - this->z;

	/////////neighbours[cX + 1][cY + 1][cZ + 1] = nullptr;
}
void Chunk::sendDeleteNotification()
{
	std::cout << "Deleting chunk not";
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			for (int z = 0; z < 3; z++)
			{
				if (neighbours[x][y][z] != nullptr)
				{
					neighbours[x][y][z]->receiveDeleteNotification(this->x, this->y, this->z);
				}
			}
		}
	}
}
void Chunk::addNeighbour(Chunk* c, Chunk* const localChunk)
{
	int cX = c->x - this->x;
	int cY = c->y - this->y;
	int cZ = c->z - this->z;
	
	neighbours[cX + 1][cY + 1][cZ + 1] = c;
	c->neighbours[2 - (cX + 1)][2 - (cY+ 1)][2 - (cZ + 1)] = localChunk;
}
bool Chunk::neighboursLoaded()
{
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			for (int z = 0; z < 3; z++)
			{
				if (!(x == 1 && y == 1 && z == 1) && (neighbours[x][y][z] == nullptr || !neighbours[x][y][z]->dataLoaded))
				{
					return false;
				}
			}
		}
	}
	return true;
}
void Chunk::setVoxel(char x, char y, char z, unsigned short id, unsigned short density)
{
	emptyChunk = false;
	this->density[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)] = (id << 8) + density;
}

void Chunk::clearDensity()
{
	delete[] density;
}
bool Chunk::threadSafe()
{
	return isThreadSafe;
}
void Chunk::clear(int ax, int ay, int az, int bx, int by, int bz, int strength)
{
	if (generating)
	{
		return;
	}
	editingData = true;
	for (int x = ax - bx; x < ax + bx; x++)
	{
		for (int y = ay - by; y < ay + by; y++)
		{
			for (int z = az - bz; z < az + bz; z++)
			{
				if (getRelativeVoxelID(x - 1, y, z) == 0 || getRelativeVoxelID(x + 1, y, z) == 0 || getRelativeVoxelID(x, y - 1, z) == 0 ||
					getRelativeVoxelID(x, y + 1, z) == 0 || getRelativeVoxelID(x, y, z - 1) == 0 || getRelativeVoxelID(x, y, z + 1) == 0)
				{
					int d = relativeDensity(x, y, z) & 0x00ff;
					d -= Clock::get(255) * 10;
					int id = relativeDensity(x, y, z) >> 8;
					if (d < 1)
					{
						d = 0;
						id = 0;
					}
					setRelativeVoxel(x, y, z, id, d);
				}
			}
		}
	}
	editingData = false;
	for (int x = ax - bx - 2; x < ax + bx + 2; x++)
	{
		for (int y = ay - by - 2; y < ay + by + 2; y++)
		{
			for (int z = az - bz - 2; z < az + bz + 2; z++)
			{
				updateRelativeVoxel(x, y, z);
			}
		}
	}
}
void Chunk::fill(int ax, int ay, int az, int bx, int by, int bz, int id, int density)
{
	if (generating)
	{
		return;
	}
	editingData = true;
	for (int x = ax - bx; x < ax + bx - 1; x++)
	{
		for (int y = ay - by; y < ay + by - 1; y++)
		{
			for (int z = az - bz; z < az + bz - 1; z++)
			{
				//if (getRelativeVoxelID(xx - 1, yy, zz) == 0 || getRelativeVoxelID(xx + 1, yy, zz) == 0 || getRelativeVoxelID(xx, yy - 1, zz) == 0 ||
				//	getRelativeVoxelID(xx, yy + 1, zz) == 0 || getRelativeVoxelID(xx, yy, zz - 1) == 0 || getRelativeVoxelID(xx, yy, zz + 1) == 0)
				{
					int d = relativeDensity(x, y, z) & 0x00ff;
					int t = (((bx)-(abs(x - ax))) + ((by)-(abs(y - ay))) + ((bz)-(abs(z - az))) + 1);
					if (t <= 0)
					{
						t = 1;
					}
					d += Clock::get((double) density) * t;
					//int id = relativeDensity(xx, yy, zz) >> 8;
					if (d > 255)
					{
						d = 255;
					}
					if (d > 0)
					{
						setRelativeVoxel(x, y, z, id, 0);
					}
				}
			}
		}
	}
	editingData = false;
	for (int x = ax - bx - 2; x < ax + bx + 2; x++)
	{
		for (int y = ay - by - 2; y < ay + by + 2; y++)
		{
			for (int z = az - bz - 2; z < az + bz + 2; z++)
			{
				updateRelativeVoxel(x, y, z);
			}
		}
	}
}
void Chunk::setRelativeVoxel(int x, int y, int z, int id, int density)
{
	x -= 1;
	y -= 1;
	z -= 1;
	int cX = x >> 6;
	int cY = y >> 6;
	int cZ = z >> 6;

	if (cX == 0 && cY == 0 && cZ == 0)
	{
		int rX = x - (cX << 6);
		int rY = y - (cY << 6);
		int rZ = z - (cZ << 6);

		setVoxel(rX, rY, rZ, id, density);
	}
	if (neighbours[cX + 1][cY + 1][cZ + 1] != nullptr && !neighbours[cX + 1][cY + 1][cZ + 1]->emptyChunk)
	{
		int rX = x - (cX << 6);
		int rY = y - (cY << 6);
		int rZ = z - (cZ << 6);
		//neighbours[cX + 1][cY + 1][cZ + 1]->chunkUpdate = true;
		//neighbours[cX + 1][cY + 1][cZ + 1]->priorityLoad = true;
		//neighbours[cX + 1][cY + 1][cZ + 1]->loaded = false;
		return neighbours[cX + 1][cY + 1][cZ + 1]->setVoxel(rX, rY, rZ, id, density);
	}
}
void Chunk::updateRelativeVoxel(int x, int y, int z)
{
	x -= 1;
	y -= 1;
	z -= 1;
	int cX = x >> 6;
	int cY = y >> 6;
	int cZ = z >> 6;

	if (neighbours[cX + 1][cY + 1][cZ + 1] != nullptr && !neighbours[cX + 1][cY + 1][cZ + 1]->emptyChunk)
	{
		int rX = x - (cX << 6);
		int rY = y - (cY << 6);
		int rZ = z - (cZ << 6);
		neighbours[cX + 1][cY + 1][cZ + 1]->chunkUpdate = true;
		neighbours[cX + 1][cY + 1][cZ + 1]->priorityLoad = true;
		neighbours[cX + 1][cY + 1][cZ + 1]->loaded = false;
	}
}
int Chunk::getRelativeVoxelID(int x, int y, int z)
{
	return relativeDensity(x, y, z) >> 8;
}
std::vector<std::vector<float>> Chunk::generate()
{
	if (emptyChunk)
	{
		return std::vector<std::vector<float>>();
	}
	auto start = std::chrono::high_resolution_clock::now();
	std::vector<float> vertices;
	std::vector<unsigned int> materials;
	std::vector<float> normals;
	std::vector<float> totals;
	std::vector<Voxel*> edges;
	std::vector<char> flips;
	std::vector<char> axis;

	std::vector<float> verticesL;
	std::vector<unsigned int> materialsL;
	std::vector<float> normalsL;
	std::vector<float> totalsL;
	std::vector<Voxel*> edgesL;
	std::vector<char> flipsL;
	std::vector<char> axisL;

	size = 64 / lod + 6;
	Voxel* voxels = new Voxel[size * size * size];
	
	v = voxels;
	Vec3f edge;
	float interp = 0;

	float xx = 0;
	float yy = 0;
	float zz = 0;

	int cubicSize = size * size * size;

	Voxel* voxel;

	for (int i = 0; i < cubicSize; i++)
	{
		int x = i % size;
		int y = (i / size) % size;
		int z = i / size / size;
		float rX = x - 1;
		float rY = y - 1;
		float rZ = z - 1;
		voxel = &v[x + size * (y + size * z)];
		voxel->x = rX;
		voxel->y = rY;
		voxel->z = rZ;
		int id = relativeDensity(rX, rY, rZ) >> 8;
		if (scan(x, y, z, 0, rY, rZ, material) > 0)
		{
			interp = calcDensity(rX, rY, rZ, rX + 1, rY, rZ);
			
			xx = rX + interp;
			yy = rY;
			zz = rZ;
			if (!liquid(material))
			{
				axis.emplace_back(0);
				at(x, y - 1, z - 1)->addAverage(xx, yy, zz, material);
				at(x, y, z - 1)->addAverage(xx, yy, zz, material);
				at(x, y - 1, z)->addAverage(xx, yy, zz, material);
				voxel->addAverage(xx, yy, zz, material);

				at(x, y - 1, z - 1)->addMaterial(material);
				at(x, y, z - 1)->addMaterial(material);
				at(x, y - 1, z)->addMaterial(material);
				voxel->addMaterial(material);

				if (air(id))
				{
					voxel->flipX = true;
				}
				if (voxel->flipX)
				{
					flips.emplace_back(0);
					edges.emplace_back(voxel);
					edges.emplace_back(at(x, y, z - 1));
					edges.emplace_back(at(x, y - 1, z));
					edges.emplace_back(at(x, y - 1, z - 1));
				}
				else
				{
					flips.emplace_back(1);
					edges.emplace_back(at(x, y - 1, z - 1));
					edges.emplace_back(at(x, y, z - 1));
					edges.emplace_back(at(x, y - 1, z));
					edges.emplace_back(voxel);
				}
			}
			else
			{
				axisL.emplace_back(0);
				at(x, y - 1, z - 1)->addLiquidAverage(voxel);
				at(x, y, z - 1)->addLiquidAverage(voxel);
				at(x, y - 1, z)->addLiquidAverage(voxel);
				voxel->addLiquidAverage(voxel);

				at(x, y - 1, z - 1)->addLiquidMaterial(100);
				at(x, y, z - 1)->addLiquidMaterial(100);
				at(x, y - 1, z)->addLiquidMaterial(100);
				voxel->addLiquidMaterial(100);

				if (liquid(id))
				{
				}
				if (voxel->flipLX)
				{
					flipsL.emplace_back(0);
					edgesL.emplace_back(voxel);
					edgesL.emplace_back(at(x, y, z - 1));
					edgesL.emplace_back(at(x, y - 1, z));
					edgesL.emplace_back(at(x, y - 1, z - 1));
				}
				else
				{
					flipsL.emplace_back(1);
					edgesL.emplace_back(at(x, y - 1, z - 1));
					edgesL.emplace_back(at(x, y, z - 1));
					edgesL.emplace_back(at(x, y - 1, z));
					edgesL.emplace_back(voxel);
				}

			}

		}
		if (scan(x, y, z, 1, rX, rZ, material) > 0)
		{
			interp = calcDensity(rX, rY, rZ, rX, rY + 1, rZ);
			
			xx = rX;
			yy = rY + interp;
			zz = rZ;
			if (!liquid(material))
			{
				axis.emplace_back(1);
				at(x - 1, y, z - 1)->addAverage(xx, yy, zz, material);
				at(x, y, z - 1)->addAverage(xx, yy, zz, material);
				at(x - 1, y, z)->addAverage(xx, yy, zz, material);
				voxel->addAverage(xx, yy, zz, material);

				at(x - 1, y, z - 1)->addMaterial(material);
				at(x, y, z - 1)->addMaterial(material);
				at(x - 1, y, z)->addMaterial(material);
				voxel->addMaterial(material);

				if (!air(id))
				{
					voxel->flipY = true;
				}
				if (voxel->flipY)
				{
					flips.emplace_back(2);
					edges.emplace_back(voxel);
					edges.emplace_back(at(x, y, z - 1));
					edges.emplace_back(at(x - 1, y, z));
					edges.emplace_back(at(x - 1, y, z - 1));
				}
				else
				{
					flips.emplace_back(3);
					edges.emplace_back(at(x - 1, y, z - 1));
					edges.emplace_back(at(x, y, z - 1));
					edges.emplace_back(at(x - 1, y, z));
					edges.emplace_back(voxel);
				}
			}
			else
			{
				axisL.emplace_back(1);
				at(x - 1, y, z - 1)->addLiquidAverage(voxel);
				at(x, y, z - 1)->addLiquidAverage(voxel);
				at(x - 1, y, z)->addLiquidAverage(voxel);
				voxel->addLiquidAverage(voxel);

				at(x - 1, y, z - 1)->addLiquidMaterial(100);
				at(x, y, z - 1)->addLiquidMaterial(100);
				at(x - 1, y, z)->addLiquidMaterial(100);
				voxel->addLiquidMaterial(100);
				if (liquid(id))
				{
					//voxel->flipLY = true;
				}
				if (voxel->flipLY)
				{
					flipsL.emplace_back(2);
					edgesL.emplace_back(voxel);
					edgesL.emplace_back(at(x, y, z - 1));
					edgesL.emplace_back(at(x - 1, y, z));
					edgesL.emplace_back(at(x - 1, y, z - 1));
				}
				else
				{
					flipsL.emplace_back(3);
					edgesL.emplace_back(at(x - 1, y, z - 1));
					edgesL.emplace_back(at(x, y, z - 1));
					edgesL.emplace_back(at(x - 1, y, z));
					edgesL.emplace_back(voxel);
				}
			}


		}
		if (scan(x, y, z, 2, rX, rY, material) > 0)
		{
			interp = calcDensity(rX, rY, rZ, rX, rY, rZ + 1);

			xx = rX;
			yy = rY;
			zz = rZ + interp;
			if (!liquid(material))
			{
				axis.emplace_back(2);
				at(x - 1, y - 1, z)->addAverage(xx, yy, zz, material);
				at(x - 1, y, z)->addAverage(xx, yy, zz, material);
				at(x, y - 1, z)->addAverage(xx, yy, zz, material);
				voxel->addAverage(xx, yy, zz, material);

				at(x - 1, y - 1, z)->addMaterial(material);
				at(x - 1, y, z)->addMaterial(material);
				at(x, y - 1, z)->addMaterial(material);
				voxel->addMaterial(material);

				if (!air(id))
				{
					voxel->flipZ = true;
				}
				if (voxel->flipZ)
				{
					flips.emplace_back(4);
					edges.emplace_back(voxel);
					edges.emplace_back(at(x - 1, y, z));
					edges.emplace_back(at(x, y - 1, z));
					edges.emplace_back(at(x - 1, y - 1, z));
				}
				else
				{
					flips.emplace_back(5);
					edges.emplace_back(at(x - 1, y - 1, z));
					edges.emplace_back(at(x - 1, y, z));
					edges.emplace_back(at(x, y - 1, z));
					edges.emplace_back(voxel);
				}
			}
			else
			{
				axisL.emplace_back(2);
				at(x - 1, y - 1, z)->addLiquidAverage(voxel);
				at(x - 1, y, z)->addLiquidAverage(voxel);
				at(x, y - 1, z)->addLiquidAverage(voxel);
				voxel->addLiquidAverage(voxel);

				at(x - 1, y - 1, z)->addLiquidMaterial(material);
				at(x - 1, y, z)->addLiquidMaterial(material);
				at(x, y - 1, z)->addLiquidMaterial(material);
				voxel->addLiquidMaterial(material);

				if (liquid(id))
				{
					voxel->flipLZ = true;
				}
				if (voxel->flipLZ)
				{
					flipsL.emplace_back(4);
					edgesL.emplace_back(voxel);
					edgesL.emplace_back(at(x - 1, y, z));
					edgesL.emplace_back(at(x, y - 1, z));
					edgesL.emplace_back(at(x - 1, y - 1, z));
				}
				else
				{
					flipsL.emplace_back(5);
					edgesL.emplace_back(at(x - 1, y - 1, z));
					edgesL.emplace_back(at(x - 1, y, z));
					edgesL.emplace_back(at(x, y - 1, z));
					edgesL.emplace_back(voxel);
				}
			}
		}
	}
	Voxel* aV;
	Voxel* bV;
	Voxel* cV;
	Voxel* dV;
	for (int i = 0; i < edges.size(); i++)
	{
		edges.at(i)->getAverage();
		edges.at(i)->findMostCommonMaterial();
	}

	Voxel* aVL;
	Voxel* bVL;
	Voxel* cVL;
	Voxel* dVL;
	int axisType = 0;
	for (auto e : edges)
	{
		e->smoothAverage = e->average;
	}
	for (int i = 0; i < edgesL.size(); i++)
	{
		edgesL.at(i)->getLiquidAverage();
		edgesL.at(i)->findMostCommonLiquid();
	}

	int axisTypeL = 0;
	for (auto e : edgesL)
	{
		e->smoothLiquidAverage = e->averageLiquid;
	}

	std::vector<bool> validEdges;

	for (int i = 0; i < edges.size() / 4; i++)
	{
		aV = edges.at(i * 4 + 0);
		bV = edges.at(i * 4 + 1);
		cV = edges.at(i * 4 + 2);
		dV = edges.at(i * 4 + 3);

		validEdges.emplace_back(isValid(aV));
		validEdges.emplace_back(isValid(bV));
		validEdges.emplace_back(isValid(cV));
		validEdges.emplace_back(isValid(dV));

		axisType = axis.at(i);
		if (validEdges.at(i * 4 + 0) && validEdges.at(i * 4 + 1) && validEdges.at(i * 4 + 2) && validEdges.at(i * 4 + 3))
		{
			addTriangle(aV, bV, cV, dV, vertices, x, y, z);

			int material1 = aV->getCommonMaterial();
			int material2 = bV->getCommonMaterial();
			int material3 = cV->getCommonMaterial();
			int material4 = dV->getCommonMaterial();

			addMaterial(bV, material2, &materials);
			addMaterial(cV, material3, &materials);
			addMaterial(aV, material1, &materials);

			addMaterial(cV, material3, &materials);
			addMaterial(bV, material2, &materials);
			addMaterial(dV, material4, &materials);
		}
	}

	std::vector<bool> validEdgesL;

	for (int i = 0; i < edgesL.size() / 4; i++)
	{
		aVL = edgesL.at(i * 4 + 0);
		bVL = edgesL.at(i * 4 + 1);
		cVL = edgesL.at(i * 4 + 2);
		dVL = edgesL.at(i * 4 + 3);

		validEdgesL.emplace_back(isValid(aVL));
		validEdgesL.emplace_back(isValid(bVL));
		validEdgesL.emplace_back(isValid(cVL));
		validEdgesL.emplace_back(isValid(dVL));

		axisTypeL = axisL.at(i);
		if (validEdgesL.at(i * 4 + 0) && validEdgesL.at(i * 4 + 1) && validEdgesL.at(i * 4 + 2) && validEdgesL.at(i * 4 + 3))
		{
			addTriangleL(aVL, bVL, cVL, dVL, verticesL, x, y, z);

			int material1 = aVL->getCommonLiquid();
			int material2 = bVL->getCommonLiquid();
			int material3 = cVL->getCommonLiquid();
			int material4 = dVL->getCommonLiquid();

			addMaterial(bVL, material2, &materialsL);
			addMaterial(cVL, material3, &materialsL);
			addMaterial(aVL, material1, &materialsL);

			addMaterial(cVL, material3, &materialsL);
			addMaterial(bVL, material2, &materialsL);
			addMaterial(dVL, material4, &materialsL);
		}
	}

	bool flipXLock = false;
	bool flipYLock = false;
	bool flipZLock = false;
	for (int i = 0; i < edges.size() / 4; i++)
	{
		Voxel* aV = edges.at(i * 4 + 0);
		Voxel * bV = edges.at(i * 4 + 1);
		Voxel * cV = edges.at(i * 4 + 2);
		Voxel * dV = edges.at(i * 4 + 3);

		Vec3f a = edges.at(i * 4 + 0)->getAverage();
		Vec3f b = edges.at(i * 4 + 1)->getAverage();
		Vec3f c = edges.at(i * 4 + 2)->getAverage();
		Vec3f d = edges.at(i * 4 + 3)->getAverage();

		Vec3f faceNormal(0, 0, 0);

		Vec3f u = Vec3f(b.x - a.x, b.y - a.y, b.z - a.z);
		Vec3f v = Vec3f(c.x - a.x, c.y - a.y, c.z - a.z);

		faceNormal.x = (u.y * v.z) - (u.z * v.y);
		faceNormal.y = (u.z * v.x) - (u.x * v.z);
		faceNormal.z = (u.x * v.y) - (u.y * v.x);

		Vec3f faceNormalB(0, 0, 0);

		Vec3f ub = Vec3f(c.x - d.x, c.y - d.y, c.z - d.z);
		Vec3f vb = Vec3f(b.x - d.x, b.y - d.y, b.z - d.z);

		faceNormalB.x = (ub.y * vb.z) - (ub.z * vb.y);
		faceNormalB.y = (ub.z * vb.x) - (ub.x * vb.z);
		faceNormalB.z = (ub.x * vb.y) - (ub.y * vb.x);

		flipXLock = false;
		flipYLock = false;
		flipZLock = false;
		std::vector<Voxel*> verts = { aV, bV, cV, dV };
		for (Voxel* v : verts)
		{
			if (v->flipX)
			{
				flipXLock = true;
			}
			if (v->flipY)
			{
				flipYLock = true;
			}
			if (v->flipZ)
			{
				flipZLock = true;
			}
		}

		addNormalFace(aV, bV, cV, dV, &normals, axis.at(i), !flipXLock, !flipYLock, !flipZLock, faceNormal, faceNormalB);
	}
	flipXLock = false;
	flipYLock = false;
	flipZLock = false;
	for (int i = 0; i < edgesL.size() / 4; i++)
	{
		Voxel* aV = edgesL.at(i * 4 + 0);
		Voxel* bV = edgesL.at(i * 4 + 1);
		Voxel* cV = edgesL.at(i * 4 + 2);
		Voxel* dV = edgesL.at(i * 4 + 3);

		Vec3f a = edgesL.at(i * 4 + 0)->getLiquidAverage();
		Vec3f b = edgesL.at(i * 4 + 1)->getLiquidAverage();
		Vec3f c = edgesL.at(i * 4 + 2)->getLiquidAverage();
		Vec3f d = edgesL.at(i * 4 + 3)->getLiquidAverage();

		Vec3f faceNormal(0, 0, 0);

		Vec3f u = Vec3f(b.x - a.x, b.y - a.y, b.z - a.z);
		Vec3f v = Vec3f(c.x - a.x, c.y - a.y, c.z - a.z);

		faceNormal.x = (u.y * v.z) - (u.z * v.y);
		faceNormal.y = (u.z * v.x) - (u.x * v.z);
		faceNormal.z = (u.x * v.y) - (u.y * v.x);

		Vec3f faceNormalB(0, 0, 0);

		Vec3f ub = Vec3f(c.x - d.x, c.y - d.y, c.z - d.z);
		Vec3f vb = Vec3f(b.x - d.x, b.y - d.y, b.z - d.z);

		faceNormalB.x = (ub.y * vb.z) - (ub.z * vb.y);
		faceNormalB.y = (ub.z * vb.x) - (ub.x * vb.z);
		faceNormalB.z = (ub.x * vb.y) - (ub.y * vb.x);

		flipXLock = false;
		flipYLock = false;
		flipZLock = false;
		std::vector<Voxel*> verts = { aV, bV, cV, dV };
		for (Voxel* v : verts)
		{
			if (v->flipLX)
			{
				flipXLock = true;
			}
			if (v->flipLY)
			{
				flipYLock = true;
			}
			if (v->flipLZ)
			{
				flipZLock = true;
			}
		}

		addNormalFaceL(aV, bV, cV, dV, &normalsL, axisL.at(i), !flipXLock, !flipYLock, !flipZLock, faceNormal, faceNormalB);
	}

	for (int i = 0; i < edges.size(); i++)
	{
		edges.at(i)->fn.normalize();
	}
	for (int i = 0; i < edgesL.size(); i++)
	{
		edgesL.at(i)->fnL.normalize();
	}
	
	for (int i = 0; i < edges.size() / 4; i++)
	{
		Voxel* aV = edges.at(i * 4 + 0);
		Voxel* bV = edges.at(i * 4 + 1);
		Voxel* cV = edges.at(i * 4 + 2);
		Voxel* dV = edges.at(i * 4 + 3);

		/////For low poly 
		/*Vec3f vc((aV->fn.x + bV->fn.x + cV->fn.x + dV->fn.x),
			(aV->fn.y + bV->fn.y + cV->fn.y + dV->fn.y),
			(aV->fn.z + bV->fn.z + cV->fn.z + dV->fn.z));

		vc.normalize();*/

		if (validEdges.at(i * 4 + 0) && validEdges.at(i * 4 + 1) && validEdges.at(i * 4 + 2) && validEdges.at(i * 4 + 3))
		{
			Vec3f a;
			addN(aV, &normals);
			addN(bV, &normals);
			addN(cV, &normals);

			addN(dV, &normals);
			addN(cV, &normals);
			addN(bV, &normals);

			int material1 = 0;
			int material2 = 0;
			int material3 = 0;
			int material4 = 0;

			if (aV->flipX)
			{
				flips.emplace_back(1);
			}
			else if (aV->flipY)
			{
				flips.emplace_back(2);
			}
			else if (aV->flipZ)
			{
				flips.emplace_back(3);
			}
			else
			{
				flips.emplace_back(0);
			}
		}
	}
	
	for (int i = 0; i < edgesL.size() / 4; i++)
	{
		Voxel* aV = edgesL.at(i * 4 + 0);
		Voxel* bV = edgesL.at(i * 4 + 1);
		Voxel* cV = edgesL.at(i * 4 + 2);
		Voxel* dV = edgesL.at(i * 4 + 3);

		if (validEdgesL.at(i * 4 + 0) && validEdgesL.at(i * 4 + 1) && validEdgesL.at(i * 4 + 2) && validEdgesL.at(i * 4 + 3))
		{
			Vec3f a;
			addNL(aV, &normalsL);
			addNL(bV, &normalsL);
			addNL(cV, &normalsL);

			addNL(dV, &normalsL);
			addNL(cV, &normalsL);
			addNL(bV, &normalsL);

			int material1 = 0;
			int material2 = 0;
			int material3 = 0;
			int material4 = 0;

			if (aV->flipLX)
			{
				flipsL.emplace_back(1);
			}
			else if (aV->flipLY)
			{
				flipsL.emplace_back(2);
			}
			else if (aV->flipLZ)
			{
				flipsL.emplace_back(3);
			}
			else
			{
				flipsL.emplace_back(0);
			}
		}
	}
	
	std::vector<float> posA;
	std::vector<float> posB;
	std::vector<float> posC;
	int i = 0;
	while (i < vertices.size())
	{
		for (int j = 0; j < 3; j++)
		{
			posA.emplace_back(vertices.at(i++));
			posA.emplace_back(vertices.at(i++));
			posA.emplace_back(vertices.at(i++));

			posB.emplace_back(vertices.at(i++));
			posB.emplace_back(vertices.at(i++));
			posB.emplace_back(vertices.at(i++));

			posC.emplace_back(vertices.at(i++));
			posC.emplace_back(vertices.at(i++));
			posC.emplace_back(vertices.at(i++));
			i -= 9;
		}
		i += 9;
	}
	i = 0;
	while (i < materials.size())
	{
		allMaterials.emplace_back(materials.at(i + 0));
		allMaterials.emplace_back(materials.at(i + 1));
		allMaterials.emplace_back(materials.at(i + 2));

		allMaterials.emplace_back(materials.at(i + 0));
		allMaterials.emplace_back(materials.at(i + 1));
		allMaterials.emplace_back(materials.at(i + 2));

		allMaterials.emplace_back(materials.at(i + 0));
		allMaterials.emplace_back(materials.at(i + 1));
		allMaterials.emplace_back(materials.at(i + 2));

		i += 3;
	}

	std::vector<float> posAL;
	std::vector<float> posBL;
	std::vector<float> posCL;
	i = 0;
	while (i < verticesL.size())
	{
		for (int j = 0; j < 3; j++)
		{
			posAL.emplace_back(verticesL.at(i++));
			posAL.emplace_back(verticesL.at(i++));
			posAL.emplace_back(verticesL.at(i++));

			posBL.emplace_back(verticesL.at(i++));
			posBL.emplace_back(verticesL.at(i++));
			posBL.emplace_back(verticesL.at(i++));

			posCL.emplace_back(verticesL.at(i++));
			posCL.emplace_back(verticesL.at(i++));
			posCL.emplace_back(verticesL.at(i++));
			i -= 9;
		}
		i += 9;
	}
	i = 0;
	while (i < materialsL.size())
	{
		allLiquidMaterials.emplace_back(materialsL.at(i + 0));
		allLiquidMaterials.emplace_back(materialsL.at(i + 1));
		allLiquidMaterials.emplace_back(materialsL.at(i + 2));

		allLiquidMaterials.emplace_back(materialsL.at(i + 0));
		allLiquidMaterials.emplace_back(materialsL.at(i + 1));
		allLiquidMaterials.emplace_back(materialsL.at(i + 2));

		allLiquidMaterials.emplace_back(materialsL.at(i + 0));
		allLiquidMaterials.emplace_back(materialsL.at(i + 1));
		allLiquidMaterials.emplace_back(materialsL.at(i + 2));

		i += 3;
	}
	

	
	auto finish = std::chrono::high_resolution_clock::now();

	std::vector<std::vector<float>> data;
	this->chunkInfo.emplace_back(vertices);
	this->chunkInfo.emplace_back(normals);
	this->chunkInfo.emplace_back(posA);
	this->chunkInfo.emplace_back(posB);
	this->chunkInfo.emplace_back(posC);

	this->chunkInfo.emplace_back(verticesL);
	this->chunkInfo.emplace_back(normalsL);
	this->chunkInfo.emplace_back(posAL);
	this->chunkInfo.emplace_back(posBL);
	this->chunkInfo.emplace_back(posCL);
	//this->chunkInfo.emplace_back(std::vector<float>());
	//std::vector<float> meta;
	//meta.emplace_back((double)std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());

	//std::cout << "loaded : " << ((double)std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() * 0.000000001) << "\n";
	//this->chunkInfo.emplace_back(meta);
	
	delete[] voxels;
	loaded = true;
	
	return data;
}


std::vector<std::vector<float>> Chunk::generateLiquid()
{
	return std::vector<std::vector<float>>();
}

void Chunk::generateTerrain(const std::shared_ptr<ChunkHeight>& heights)
{
	bool air = false;
	bool solid = false;
	for (int x = 0; x < BASE_SIZE; x++)
	{
		for (int z = 0; z < BASE_SIZE; z++)
		{
			float density = 0;
			float height = heights->getHeight(x, z);
			float excess = ((height)-(float)((int)(height)));
			excess *= 255;
			for (int y = 0; y < BASE_SIZE; y++)
			{
				setVoxel(x, y, z, 0, 0);

				if (y + this->y * CHUNK_SIZE < height)
				{
					/*if ((this->x % 2 == 0) == (this->z % 2 == 0))
					{
						setDensity(x, y, z, 7, excess);
					}
					else
					{
						setDensity(x, y, z, 5, excess);
					}
					if (abs(this->z % 4) == 3)
					{
						setDensity(x, y, z, 1, excess);
					}
					if (abs(this->x % 4) == 3)
					{
						setDensity(x, y, z, 12, excess);
					}
					if (abs(this->x % 3) == 2)
					{
						setDensity(x, y, z, 2, excess);
					}*/
					setVoxel(x, y, z, 18, excess);
				}
				if (y + this->y * CHUNK_SIZE < (int) (height))
				{
					//if ((this->x % 2 == 0) == (this->z % 2 == 0))
					{
					//	setDensity(x, y, z, 7, 255);
					}
					//else
					{
						setVoxel(x, y, z, 18, 255);
					}
					/*if (abs(this->z % 4) == 3)
					{
						setDensity(x, y, z, 1, 255);
					}
					if (abs(this->x % 4) == 3)
					{
						setDensity(x, y, z, 12, 255);
					}
					if (abs(this->x % 3) == 2)
					{
						setDensity(x, y, z, 2, 255);
					}*/
				}
				/*if (y + this->y * CHUNK_SIZE < height)
				{
					setDensity(x, y, z, 11);
					
					if (y + this->y * CHUNK_SIZE < 7)
					{
						setDensity(x, y, z, 6);
					}
					if (y + this->y * CHUNK_SIZE < -10)
					{
						setDensity(x, y, z, 8);
					}
					if (y + this->y * CHUNK_SIZE < 0)
					{
						setDensity(x, y, z, 4);
					}
					else if (y + this->y * CHUNK_SIZE > 170)
					{
						setDensity(x, y, z, 12);
					}
					if (y + this->y * CHUNK_SIZE <= -1)
					{
						setDensity(x, y, z, 13);
					}
				}
				if (y + this->y * CHUNK_SIZE < height - 1)
				{
					setDensity(x, y, z, 3);
				}
				float zz = z + this->z * CHUNK_SIZE;
				if (zz < -250)
				{
					zz = -250;
				}
				if (zz > 250)
				{
					zz = 250;
				}
				if (M::distance(Vec3f(x + this->x * CHUNK_SIZE, y + this->y * CHUNK_SIZE, z + this->z * CHUNK_SIZE), Vec3f(70, zz, zz)) < 9)
				{
					setDensity(x, y, z, 5);
				}
				if (M::distance(Vec3f(x + this->x * CHUNK_SIZE, y + this->y * CHUNK_SIZE, z + this->z * CHUNK_SIZE), Vec3f(60, (z + this->z * CHUNK_SIZE) / 3, z + this->z * CHUNK_SIZE)) < 7)
				{
					setDensity(x, y, z, 0);
				}*/

				if (getVoxelID(x,y,z) < 1)
				{
					air = true;
				}
				else if (getVoxelID(x, y, z))
				{
					solid = true;
				}
			}
		
		}
	}
	if (solid != air)
	{
		emptyChunk = true;

	}
	else
	{

	}
	dataLoaded = true;
}
void Chunk::render()
{
	if (liquidMesh.getVAO() != 0)
	{
		glDisable(GL_CULL_FACE);
		glBindVertexArray(liquidMesh.getVAO());
		glEnableVertexArrayAttrib(liquidMesh.getVAO(), 0);
		glEnableVertexArrayAttrib(liquidMesh.getVAO(), 1);
		glEnableVertexArrayAttrib(liquidMesh.getVAO(), 2);
		glEnableVertexArrayAttrib(liquidMesh.getVAO(), 3);
		glEnableVertexArrayAttrib(liquidMesh.getVAO(), 4);
		glEnableVertexArrayAttrib(liquidMesh.getVAO(), 5);
		glDrawArrays(GL_TRIANGLES, 0, liquidMesh.getCount());
	}

	if (mesh.getVAO() != 0)
	{
 		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glBindVertexArray(mesh.getVAO());
		glEnableVertexArrayAttrib(mesh.getVAO(), 0);
		glEnableVertexArrayAttrib(mesh.getVAO(), 1);
		glEnableVertexArrayAttrib(mesh.getVAO(), 2);
		glEnableVertexArrayAttrib(mesh.getVAO(), 3);
		glEnableVertexArrayAttrib(mesh.getVAO(), 4);
		glEnableVertexArrayAttrib(mesh.getVAO(), 5);
		glDrawArrays(GL_TRIANGLES, 0, mesh.getCount());
	}
}