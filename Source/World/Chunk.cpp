#include "stdafx.h"
#include "Chunk.h"
#include <iostream>
#include "M.h"
#include <chrono>
#include "RandomNoise.h"
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

int Chunk::CHUNK_SIZE = 128;

void addN(Vec3f n, std::vector<float>* vec)
{
	vec->emplace_back(n.x);
	vec->emplace_back(n.y);
	vec->emplace_back(n.z);
}
void addN(Voxel*n, std::vector<float>* vec)
{
	vec->emplace_back(n->fn.x);
	vec->emplace_back(n->fn.y);
	vec->emplace_back(n->fn.z);
}

void addMaterial(Voxel* n, int material, std::vector<unsigned int>* mat)
{
	mat->emplace_back(material);
}
void addNormalFace(Voxel* n1, Voxel* n2, Voxel* n3, Voxel* n4, std::vector<float>* vec, char type, bool flipXLock, bool flipYLock, bool flipZLock, Vec3f a, Vec3f b)
{
	bool flipX = n3->flipX || n2->flipX;
	bool flipY = n3->flipY || n1->flipY;
	bool flipZ = n3->flipZ || n2->flipZ;

	bool flip = false;

	if (type == 0)
	{
		if (flipXLock && flipX)
		{
			flip = true;
		}
	}
	if (type == 1)
	{
		if (flipYLock && flipY)
		{
			flip = true;
		}
	}
	if (type == 2)
	{
		if (flipZLock && flipZ)
		{
			flip = true;
		}
	}
	if (flip)
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
bool Chunk::isValidNormal(Voxel* vert)
{
	if (vert->x >= -1 && vert->y >= -1 && vert->z >= -1 &&
		vert->x < BASE_SIZE + 3 && vert->y < BASE_SIZE + 3 && vert->z < BASE_SIZE + 3)
	{
		return true;
	}
	return true;
}
bool Chunk::isValid(Voxel* vert)
{
	if (vert->x > 0 && vert->y > 0 && vert->z > 0 &&
		(float) vert->x < (float) size - 4.9 && (float) vert->y < (float) size - 4.9 && (float) vert->z < (float) size - 4.9)
	{
		return true;
	}
	return false;
}
void Chunk::addVertex(Vec3f pos, std::vector<float>* vec, float x, float y, float z)
{
	vec->emplace_back((pos.x * lod + CHUNK_SIZE * x) * 1);
	vec->emplace_back((pos.y * lod + CHUNK_SIZE * y) * 1 - 0.5f);
	vec->emplace_back((pos.z * lod + CHUNK_SIZE * z) * 1);
}

void Chunk::addTriangle(Voxel * v1, Voxel * v2, Voxel * v3, Voxel * v4, std::vector<float> * vec, float x, float y, float z)
{
	addVertex(v1->smoothAverage, vec, x, y, z);
	addVertex(v2->smoothAverage, vec, x, y, z);
	addVertex(v3->smoothAverage, vec, x, y, z);

	addVertex(v4->smoothAverage, vec, x, y, z);
	addVertex(v3->smoothAverage, vec, x, y, z);
	addVertex(v2->smoothAverage, vec, x, y, z);
}
Voxel* v;
Voxel* Chunk::at(int x, int y, int z)
{
	return &v[x + size * (y + size * z)];
}

float Chunk::relativeDensity(int x, int y, int z)
{
	x -= 1;
	y -= 1;
	z -= 1;
	int cX = x >> 7;
	int cY = y >> 7;
	int cZ = z >> 7;

	if (cX == 0 && cY == 0 && cZ == 0)
	{
		int rX = x - (cX << 7);
		int rY = y - (cY << 7);
		int rZ = z - (cZ << 7);
		return getDensity(rX, rY, rZ);
	}
	if (neighbours[cX + 1][cY + 1][cZ + 1] != nullptr && !neighbours[cX + 1][cY + 1][cZ + 1]->emptyChunk)
	{
		int rX = x - (cX << 7);
		int rY = y - (cY << 7);
		int rZ = z - (cZ << 7);
		return neighbours[cX + 1][cY + 1][cZ + 1]->getDensity(rX, rY, rZ);
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

int positionInArray = 0;

float Chunk::getDensity(int x, int y, int z)
{
	positionInArray = x + CHUNK_SIZE * (y + CHUNK_SIZE * z);
	if (positionInArray >= 0 && positionInArray < 2097152)
	{
		return density[positionInArray];
	}
	return 0;
}
float Chunk::calcDensity(int x, int y, int z, int xX, int yY, int zZ)
{
	//////CODE FOR EVEN SMOOTHER TERRAIN///////

	/*float d1 = 0;
	float d2 = 0;
	x *= lod;
	y *= lod;
	z *= lod;
	
	xX *= lod;
	yY *= lod;
	zZ *= lod;
	if (x >= 0 && y >= 0 && z >= 0 &&
		x < BASE_SIZE&& y < BASE_SIZE && z < BASE_SIZE)
	{
		d1 = relativeDensity(x, y, z);
	}
	if (xX >= 0 && yY >= 0 && zZ >= 0 &&
		xX < BASE_SIZE&& yY < BASE_SIZE && zZ < BASE_SIZE)
	{
		d2 = relativeDensity(xX, yY, zZ);
	}
	float interp = (d1 + d2) / 2;
	if (d1 < d2)
	{
		interp = 1 - interp;
	}
	interp = 0.5;*/
	return 0.5;
}
int offX = 0;
int offY = 0;
int offZ = 0;

float fdA = 0;
float fdB = 0;

int Chunk::scan(int x, int y, int z, int axis, int axisA, int axisB, float &density, int &material)
{
	offX = (axis == 0);
	offY = (axis == 1);
	offZ = (axis == 2);
	x -= 1;
	y -= 1;
	z -= 1;
	x *= lod;
	y *= lod;
	z *= lod;

	offX *= lod;
	offY *= lod;
	offZ *= lod;
	if (axisA >= 0 && axisB >= 0)
	{
		fdA = relativeDensity(x, y, z);
		fdB = relativeDensity(x + offX, y + offY, z + offZ);
		if (fdA < 0 && fdB < 0)
		{
			return -1;
		}
		if ((fdA == 0 && fdB) != 0)
		{
			material = fdB;
			return (int) fdB;
		}
		if (fdA != 0 && fdB == 0)
		{
			material = fdA;
			return (int) fdA;
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
		//std::cout << chunkInfo.size() << "\n";
		mesh = Model::load3DModel(chunkInfo.at(0), chunkInfo.at(1), chunkInfo.at(2), chunkInfo.at(3), chunkInfo.at(4), allMaterials);
		ready = true;
		loaded = false;
		chunkInfo = std::vector<std::vector<float>>();
		allMaterials = std::vector<unsigned int>();
	}
	
}
void Chunk::sphere(float xP, float yP, float zP, float radius, float power)
{
	for (int x = (int) floor(-radius / 2); x < (int) ceil(radius / 2); x++)
	{
		for (int z = (int) floor(-radius / 2); z < (int) ceil(radius / 2); z++)
		{
			for (int y = (int) floor(-radius / 2); y < (int) ceil(radius / 2); y++)
			{
				int nX = x + xP;
				int nY = y + yP;
				int nZ = z + zP;
				float distance = abs(M::distance(Vec3f(xP, yP, zP), Vec3f(nX, nY, nZ)));

				if (distance < radius / 2)
				{
					if (nX >= 0 && nY >= 0 && nZ >= 0 &&
						nX < CHUNK_SIZE && nY < CHUNK_SIZE && nZ < CHUNK_SIZE)
					{
						this->density[nX + CHUNK_SIZE * (nY + CHUNK_SIZE * nZ)] += power * 0.01 * (((((radius - distance) / radius))));
						if (this->density[nX + CHUNK_SIZE * (nY + CHUNK_SIZE * nZ)] > 1)
						{
							this->density[nX + CHUNK_SIZE * (nY + CHUNK_SIZE * nZ)] = 1;
						}
						else if (this->density[nX + CHUNK_SIZE * (nY + CHUNK_SIZE * nZ)] < 0)
						{
							this->density[nX + CHUNK_SIZE * (nY + CHUNK_SIZE * nZ)] = 0;
						}
						if (this->density[nX + CHUNK_SIZE * (nY + CHUNK_SIZE * nZ)] < 0.1 && power < 0)
						{
							this->density[nX + CHUNK_SIZE * (nY + CHUNK_SIZE * nZ)] = 0;
						}
					}
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
void Chunk::addNeighbour(std::shared_ptr<Chunk> c, std::shared_ptr<Chunk> localChunk)
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
				if (!(x == 1 && y == 1 && z == 1) && neighbours[x][y][z] == nullptr)
				{
					return false;
				}
			}
		}
	}
	return true;
}
void Chunk::setDensity(int x, int y, int z, float density)
{
	if (x + CHUNK_SIZE * (y + CHUNK_SIZE * z) >= 0 && x + CHUNK_SIZE * (y + CHUNK_SIZE * z) < 2097152)
	{
		this->density[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)] = density;
	}
}
void Chunk::clearDensity()
{
	delete[] density;
}
RandomNoise ran;
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

	lod = 1;
	
	size = 128.0 / lod + 6;
	//lod = 2;
	int s = size;

	std::vector<char> flips;
	Voxel* voxels = new Voxel[s * s * s];
	v = voxels;

	std::vector<char> axis;
	int yyy = 0;
	int xn = 0;
	int zn = 0;
	int yn = 0;
	bool swag = rand() % 15 == 0;
	
	Vec3f edge;
	float rX;
	float rY;
	float rZ;
	int material = 0;
	for (int x = 0; x < s; x++)
	{
		for (int y = 0; y < s; y++)
		{
			for (int z = 0; z < s; z++)
			{
				rX = x - 1;
				rY = y - 1;
				rZ = z - 1;
				at(x, y, z)->x = rX;
				at(x, y, z)->y = rY;
				at(x, y, z)->z = rZ;
				float density = 0.5;
				if (scan(x, y, z, 0, rY, rZ, density, material) > 0)
				{
					float interp = calcDensity(rX, rY, rZ, rX + 1, rY, rZ);
					edge = Vec3f(rX+ interp, rY, rZ);
					at(x, y - 1, z - 1)->addAverage(edge);
					at(x, y, z - 1)->addAverage(edge);
					at(x, y - 1, z)->addAverage(edge);
					at(x, y, z)->addAverage(edge);

					at(x, y - 1, z - 1)->addMaterial(material);
					at(x, y, z - 1)->addMaterial(material);
					at(x, y - 1, z)->addMaterial(material);
					at(x, y, z)->addMaterial(material);

					axis.emplace_back(0);

					if (relativeDensity(rX, rY, rZ) == 0)
					{
						at(x, y, z)->flipX = true;
					}

					if (at(x, y, z)->flipX)
					{
						flips.emplace_back(0);
						edges.emplace_back(at(x, y, z));
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
						edges.emplace_back(at(x, y, z));
					}
				}
				if (scan(x, y, z, 1, rX, rZ, density, material) > 0)
				{
					float interp = calcDensity(rX, rY, rZ, rX, rY + 1, rZ);
					edge = Vec3f(rX, rY + interp, rZ);
					at(x - 1, y, z - 1)->addAverage(edge);
					at(x, y, z - 1)->addAverage(edge);
					at(x - 1, y, z)->addAverage(edge);
					at(x, y, z)->addAverage(edge);

					at(x - 1, y, z - 1)->addMaterial(material);
					at(x, y, z - 1)->addMaterial(material);
					at(x - 1, y, z)->addMaterial(material);
					at(x, y, z)->addMaterial(material);

					axis.emplace_back(1);

					if (relativeDensity(rX, rY, rZ) != 0)
					{
						at(x, y, z)->flipY = true;
					}

					if (at(x, y, z)->flipY)
					{
						flips.emplace_back(2);
						edges.emplace_back(at(x, y, z));
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
						edges.emplace_back(at(x, y, z));
					}
				}
				if (scan(x, y, z, 2, rX, rY, density, material) > 0)
				{
					float interp = calcDensity(rX, rY, rZ, rX, rY, rZ + 1);
					edge = Vec3f(rX, rY, rZ + interp);
					at(x - 1, y - 1, z)->addAverage(edge);
					at(x - 1, y, z)->addAverage(edge);
					at(x, y - 1, z)->addAverage(edge);
					at(x, y, z)->addAverage(edge);

					at(x - 1, y - 1, z)->addMaterial(material);
					at(x - 1, y, z)->addMaterial(material);
					at(x, y - 1, z)->addMaterial(material);
					at(x, y, z)->addMaterial(material);

					axis.emplace_back(2);

					if (relativeDensity(rX, rY, rZ) != 0)
					{
						at(x, y, z)->flipZ = true;
					}

					if (at(x, y, z)->flipZ)
					{
						flips.emplace_back(4);
						edges.emplace_back(at(x, y, z));
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
						edges.emplace_back(at(x, y, z));
					}
						
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

	int axisType = 0;
	for (auto e : edges)
	{
		e->smoothAverage = e->average;
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
			addTriangle(aV, bV, cV, dV, &vertices, x, y, z);

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

	for (int i = 0; i < edges.size(); i++)
	{
		edges.at(i)->fn.normalize();
	}
	
	for (int i = 0; i < edges.size() / 4; i++)
	{
		Voxel* aV = edges.at(i * 4 + 0);
		Voxel * bV = edges.at(i * 4 + 1);
		Voxel * cV = edges.at(i * 4 + 2);
		Voxel * dV = edges.at(i * 4 + 3);

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
			else if(aV->flipY)
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

	delete[] voxels;
	auto finish = std::chrono::high_resolution_clock::now();

	std::vector<std::vector<float>> data;
	this->chunkInfo.emplace_back(vertices);
	this->chunkInfo.emplace_back(normals);
	this->chunkInfo.emplace_back(posA);
	this->chunkInfo.emplace_back(posB);
	this->chunkInfo.emplace_back(posC);
	this->chunkInfo.emplace_back(std::vector<float>());
	std::vector<float> meta;
	meta.emplace_back((double)std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
	this->chunkInfo.emplace_back(meta);
	loaded = true;
	return data;
}
float lx = 0;
float lz = 0;
void Chunk::generateTerrain(std::shared_ptr<ChunkHeight> heights)
{
	bool air = false;
	bool solid = false;
	for (int x = 0; x < BASE_SIZE; x++)
	{
		for (int z = 0; z < BASE_SIZE; z++)
		{
			float density = 0;
			float height = (int) heights->getHeight(x, z);
			if (height < -1)
			{
				height = -1;
			}
			for (int y = 0; y < BASE_SIZE; y++)
			{
				this->density[x + BASE_SIZE * (y + BASE_SIZE * z)] = 0;

				if (y + this->y * CHUNK_SIZE < height)
				{
					this->density[x + BASE_SIZE * (y + BASE_SIZE * z)] = 11;
					
					if (y + this->y * CHUNK_SIZE < 7)
					{
						this->density[x + BASE_SIZE * (y + BASE_SIZE * z)] = 6;
					}
					if (y + this->y * CHUNK_SIZE < -10)
					{
						this->density[x + BASE_SIZE * (y + BASE_SIZE * z)] = 8;
					}
					if (y + this->y * CHUNK_SIZE < 0)
					{
						this->density[x + BASE_SIZE * (y + BASE_SIZE * z)] = 4;
					}
					
					else if (y + this->y * CHUNK_SIZE > 170)
					{
						this->density[x + BASE_SIZE * (y + BASE_SIZE * z)] = 12;
					}
					if (y + this->y * CHUNK_SIZE <= -1)
					{
						this->density[x + BASE_SIZE * (y + BASE_SIZE * z)] = 13;
					}
				}
				if (y + this->y * CHUNK_SIZE < height - 1)
				{
					this->density[x + BASE_SIZE * (y + BASE_SIZE * z)] = 3;
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
					this->density[x + BASE_SIZE * (y + BASE_SIZE * z)] = 5;
				}
				if (M::distance(Vec3f(x + this->x * CHUNK_SIZE, y + this->y * CHUNK_SIZE, z + this->z * CHUNK_SIZE), Vec3f(60, (z + this->z * CHUNK_SIZE) / 3, z + this->z * CHUNK_SIZE)) < 7)
				{
					this->density[x + BASE_SIZE * (y + BASE_SIZE * z)] = 0;
				}

				if (this->density[x + BASE_SIZE * (y + BASE_SIZE * z)] < 1)
				{
					air = true;
				}
				else if (this->density[x + BASE_SIZE * (y + size * z)])
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
}
void Chunk::render(Camera camera, Matrix4f projectionMatrix)
{
	glBindVertexArray(mesh.getVAO());
	glEnableVertexArrayAttrib(mesh.getVAO(), 0);
	glEnableVertexArrayAttrib(mesh.getVAO(), 1);
	glEnableVertexArrayAttrib(mesh.getVAO(), 2);
	glEnableVertexArrayAttrib(mesh.getVAO(), 3);
	glEnableVertexArrayAttrib(mesh.getVAO(), 4);
	glEnableVertexArrayAttrib(mesh.getVAO(), 5);
	glDrawArrays(GL_TRIANGLES, 0, mesh.getCount());
}