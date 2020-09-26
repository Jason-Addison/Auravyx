#include "stdafx.h"
#include "Utilities/RandomNoise.h"

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

double RandomNoise::getSmoothNoise(int x, int y, int z)
{
	double corners = (createNoise(x - 1, y, z - 1) + createNoise(x + 1, y, z - 1) + createNoise(x - 1, y, z + 1) + createNoise(x + 1, y, z + 1)) / 16;
	double sides = (createNoise(x - 1, y, z) + createNoise(x, y, z - 1) + createNoise(x, y, z + 1) + createNoise(x + 1, y, z)) / 8;
	double center = createNoise(x, y, z) / 4;
	return corners + sides + center;
}

double RandomNoise::costerp(double a, double b, double blend)
{
	double theta = blend * (double) PI;
	double f = (double)(1.0 - cos(theta)) * 0.5;
	return a * (1 - f) + b * f;
}

double RandomNoise::createNoise(int x, int y, int z)
{
	random.seed(x * 32 + y * 35 + z * 33 + seed);
	return (((double) random() / (double) 0xfffffffffffffff) * 2 - 1) / 32.0;
}

double interp(double a, double b, double amt)
{
	return a * (1 - amt) + b * amt;
}

double RandomNoise::getInterpolatedNoise(double x, double y, double z)
{
	int intX = (int)x;
	int intY = (int)y;
	int intZ = (int)z;

	double xFrac = x - intX;
	double yFrac = y - intY;
	double zFrac = z - intZ;
	double v1 = getSmoothNoise(x, y, z);
	//double v1 = getSmoothNoise(intX, intY, intZ);
	double v2 = getSmoothNoise(x + 1, y, z);
	double v3 = getSmoothNoise(x, y, z + 1);
	double v4 = getSmoothNoise(x + 1, y, z + 1);

	double i1 = costerp(v1, v2, xFrac);
	double i2 = costerp(v3, v4, xFrac);
	return costerp(i1, i2, zFrac);
	//return v1;
	/*double v1 = getSmoothNoise(intX, intY, intZ);
	double v2 = getSmoothNoise(intX + 1, intY, intZ);
	double v3 = getSmoothNoise(intX, intY, intZ + 1);
	double v4 = getSmoothNoise(intX + 1, intY, intZ + 1);

	double i1 = costerp(v1, v2, xFrac);
	double i2 = costerp(v3, v4, zFrac);
	return costerp(i1, i2, yFrac);*/
}

double RandomNoise::noise(double x, double y, double z, int octaves, double roughness)
{
	return getInterpolatedNoise(x, y, z);
}

double RandomNoise::noise(double x, double y, double z)
{
	return getInterpolatedNoise(x, y, z);
}
