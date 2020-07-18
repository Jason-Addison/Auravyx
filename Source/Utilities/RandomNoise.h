#pragma once
#include <random>
class RandomNoise
{
public:
	RandomNoise();
	RandomNoise(int seed);
	~RandomNoise();

	void setSeed(int seed);

	double costerp(double a, double b, double blend);

	double createNoise(int x, int y, int z);

	double noise(double x, double y, double z, int octaves, double roughness);

	double noise(double x, double y, double z);

	double getSmoothNoise(int x, int y, int z);

	double getInterpolatedNoise(double x, double y, double z);

private:

	unsigned long long seed;
	std::mt19937_64 random;
};

