#include "stdafx.h"
#include "RandomNoise.h"
#include <math.h>
#include <iostream>

#define PI 3.1415956

RandomNoise::RandomNoise()
{
	std::random_device rd;
	random.seed(rd());
	seed = random();
}

RandomNoise::RandomNoise(int seed)
{
	setSeed(seed);
}


RandomNoise::~RandomNoise()
{
}

void RandomNoise::setSeed(int seed)
{
	this->seed = seed;
}

float RandomNoise::getSmoothNoise(int x, int y, int z)
{
	float corners = (createNoise(x - 1, y, z - 1) + createNoise(x + 1, y, z - 1) + createNoise(x - 1, y, z + 1) + createNoise(x + 1, y, z + 1)) / 16;
	float sides = (createNoise(x - 1, y, z) + createNoise(x, y, z - 1) + createNoise(x, y, z + 1) + createNoise(x + 1, y, z)) / 8;
	float center = createNoise(x, y, z) / 4;
	return corners + sides + center;
}

float RandomNoise::costerp(float a, float b, float blend)
{
	double theta = blend * (double) PI;
	float f = (float)(1.0 - cos(theta)) * 0.5;
	return a * (1 - f) + b * f;
}

float RandomNoise::createNoise(int x, int y, int z)
{
	random.seed(x * 32 + y * 35 + z * 33 + seed);
	return (((double) random() / (double) 0xfffffffffffffff) * 2 - 1) / 32.0;
}

float interp(float a, float b, float amt)
{
	return a * (1 - amt) + b * amt;
}

float RandomNoise::getInterpolatedNoise(float x, float y, float z)
{
	int intX = (int)x;
	int intY = (int)y;
	int intZ = (int)z;

	float xFrac = x - intX;
	float yFrac = y - intY;
	float zFrac = z - intZ;
	float v1 = getSmoothNoise(x, y, z);
	//float v1 = getSmoothNoise(intX, intY, intZ);
	float v2 = getSmoothNoise(intX + 1, intY, intZ);
	float v3 = getSmoothNoise(intX, intY, intZ + 1);
	float v4 = getSmoothNoise(intX + 1, intY, intZ + 1);

	float i1 = costerp(v1, v2, xFrac);
	float i2 = costerp(v3, v4, xFrac);
	return costerp(i1, i2, zFrac);
	//return v1;
	/*float v1 = getSmoothNoise(intX, intY, intZ);
	float v2 = getSmoothNoise(intX + 1, intY, intZ);
	float v3 = getSmoothNoise(intX, intY, intZ + 1);
	float v4 = getSmoothNoise(intX + 1, intY, intZ + 1);

	float i1 = costerp(v1, v2, xFrac);
	float i2 = costerp(v3, v4, zFrac);
	return costerp(i1, i2, yFrac);*/
}

float RandomNoise::noise(float x, float y, float z, int octaves, float roughness)
{
	return getInterpolatedNoise(x, y, z);
}

float RandomNoise::noise(float x, float y, float z)
{
	return noise(x, y, z, 0, 0);
}
