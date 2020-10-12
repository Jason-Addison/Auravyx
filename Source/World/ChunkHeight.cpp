#include "World/ChunkHeight.h"
#include <Utilities/RandomNoise.h>
#include <iostream>
ChunkHeight::ChunkHeight()
{
}

ChunkHeight::~ChunkHeight()
{
}

float ChunkHeight::getHeight(int x, int z)
{
	return heightmap[x * 64 + z];
}

void ChunkHeight::generate(int xC, int zC)
{
	this->x = xC;
	this->z = zC;
	RandomNoise n;
	n.setSeed(54);

	int x = 0;
	int y = 0;
	for (int i = 0; i < 4096; i++)
	{
		x = i / 64;
		y = i % 64;
		heightmap[x * 64 + y] = //100 - 0.2 * (abs((double)x + (double)this->x * (double)128 * (double)x + (double)this->x * (double)128 * 0.2) + abs((double)y + (double)this->z * (double)128));// n.noise((this->x * (double)128) * 17 + this->z * 128.0 * 12, 0, 0, 0, 0) * 10;

			(float)(cos(0.06*((double)x + (double)this->x * (double)64))*5) + 30 + (float)(sin(0.06 * ((double) y + (double) this->z * 64)) * 13)
			+ abs((double)x + (double)this->x * (double)64) * 0.07 + abs((double)y + (double)this->z * (double)64) * 0.07 - 10;

			//((n.noise(((double)x + (double)this->x * (double)128) * 0.01 + 100.0, 0, ((double)y + (double)this->z * (double)128) * 0.01 + 100.0, 0, 0) * 1400.0
			//+ (n.noise(((double)x + (double)this->x * (double)128) * 0.02 + 100.0, 0, ((double)y + (double)this->z * (double)128) * 0.02 + 100.0, 0, 0) * 140.0 +
			//n.noise(((double)x + (double)this->x * (double)128) * 0.2 + 100.0, 0, ((double)y + (double)this->z * (double)128) * 0.2 + 100.0, 0, 0) * 10.0 - 30.0)) - 1024.0 + 460.0) + 400;
		//std::cout << heightmap[x * 128 + y] << "\n";
	}
}
