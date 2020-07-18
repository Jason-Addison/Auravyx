#pragma once
#include <random>
class RandomNoise
{
public:
	//Test
	unsigned long long seed;
	std::mt19937_64 random;
	RandomNoise();
	RandomNoise(int seed);
	~RandomNoise();

	void setSeed(int seed);

	float costerp(float a, float b, float blend);

	float createNoise(int x, int y, int z);

	float noise(float x, float y, float z, int octaves, float roughness);

	float noise(float x, float y, float z);

	float getSmoothNoise(int x, int y, int z);

	float getInterpolatedNoise(float x, float y, float z);
};

