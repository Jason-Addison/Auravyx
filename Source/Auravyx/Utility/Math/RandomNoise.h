#pragma once
#include <random>
class RandomNoise
{
public:
	RandomNoise();
	RandomNoise(const int seed);
	~RandomNoise();

	void setSeed(const int seed);

	double costerp(const double a, const double b, const double blend);

	double createNoise(const double x, const double y, const double z);

	double noise(const double x, const double y, const double z, const int octaves, const double roughness);

	double noise(const double x, const double y, const double z);

	double getSmoothNoise(const double x, const double y, const double z);

	double getInterpolatedNoise(const double x, const double y, const double z);

private:

	unsigned long long seed;
	std::mt19937_64 random;
};

