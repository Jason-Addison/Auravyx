#pragma once
#include "Utilities/Vec3f.h"
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
	Vec3f fnL = Vec3f(0, 0, 0);
	bool flipX = false;
	bool flipY = false;
	bool flipZ = false;

	bool flipLX = false;
	bool flipLY = false;
	bool flipLZ = false;

	bool avgCalc = false;

	bool avgCalcL = false;
	int total = 0;

	Vec3f material;
	int materialCounter = 0;
	int mat = 0;
	int liq = 0;

	Vec3f averageLiquid = Vec3f(0, 0, 0);
	Vec3f smoothLiquidAverage = Vec3f(0, 0, 0);
	float countL;


	std::vector<short> totalMaterial;
	std::vector<short> totalMaterialCounter;

	std::vector<short> totalLiquidMaterial;
	std::vector<short> totalLiquidMaterialCounter;

	std::vector<Voxel*> avgLiq;

	float x = 0;
	float y = 0;
	float z = 0;

	Vec3f getAverage();

	Vec3f getLiquidAverage();

	void addAverage(float x, float y, float z, int id);

	void addMaterial(int m);

	void addLiquidAverage(float x, float y, float z);

	void addLiquidAverage(Voxel* vox);

	void addLiquidMaterial(int m);

	void findMostCommonMaterial();

	short getCommonMaterial();

	void findMostCommonLiquid();

	short getCommonLiquid();
};

