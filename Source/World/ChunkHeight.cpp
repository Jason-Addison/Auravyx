#include "ChunkHeight.h"
#include <RandomNoise.h>

ChunkHeight::ChunkHeight()
{
}

ChunkHeight::~ChunkHeight()
{
}

int ChunkHeight::getHeight(int x, int z)
{
	return heightmap[x * 128 + z];
}

void ChunkHeight::generate(int xC, int zC)
{
	this->x = xC;
	this->z = zC;
	RandomNoise n;
	n.setSeed(54);

	int x = 0;
	int y = 0;
	for (int i = 0; i < 128 * 128; i++)
	{
		x = i / 128;
		y = i % 128;
		heightmap[x * 128 + z] = (int)
			((n.noise(((double)x + (double)this->x * (double)128) * 0.001 + 100.0, 0, ((double)z + (double)this->z * (double)128) * 0.001 + 100.0, 0, 0) * 1400.0
			+ (n.noise(((double)x + (double)this->x * (double)128) * 0.02 + 100.0, 0, ((double)z + (double)this->z * (double)128) * 0.02 + 100.0, 0, 0) * 140.0 +
			n.noise(((double)x + (double)this->x * (double)128) * 0.2 + 100.0, 0, ((double)z + (double)this->z * (double)128) * 0.2 + 100.0, 0, 0) * 10.0 - 30.0)) - 1024.0 + 460.0);
	}
}
