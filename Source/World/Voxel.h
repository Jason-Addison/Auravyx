#pragma once
#include "Vec3f.h"
#include <vector>

class Voxel
{
public:
	Voxel();
	~Voxel();

	Vec3f average = Vec3f(0, 0, 0);
	Vec3f smoothAverage = Vec3f(0, 0, 0);
	float count;
	Vec3f fn = Vec3f(0, 0, 0);
	bool flipX = false;
	bool flipY = false;
	bool flipZ = false;
	bool avgCalc = false;
	int total = 0;

	Vec3f material;
	int materialCounter = 0;
	int mat = 0;

	std::vector<short> totalMaterial;
	std::vector<short> totalMaterialCounter;

	float x = 0;
	float y = 0;
	float z = 0;

	Vec3f getAverage();

	void addAverage(float x, float y, float z);

	void addMaterial(int m);

	void findMostCommonMaterial();

	short getCommonMaterial();
};

