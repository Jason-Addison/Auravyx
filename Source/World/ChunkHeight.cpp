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
	for (int x = 0; x < 128; x++)
	{
		for (int z = 0; z < 128; z++)
		{
			heightmap[x * 128 + z] = (int)(n.noise(((double)x + (double)this->x * (double)128) * 0.002 + 100, 0, ((double)z + (double)this->z * (double)128) * 0.001 + 100, 0, 0) * 1400
				+ (int) (n.noise(((double)x + (double)this->x * (double)128) * 0.02 + 100, 0, ((double)z + (double)this->z * (double)128) * 0.02 + 100, 0, 0) * 140 +
				n.noise(((double)x + (double)this->x * (double)128) * 0.2 + 100, 0, ((double)z + (double)this->z * (double)128) * 0.2 + 100, 0, 0) * 10 - 30)) - 1024 + 460;
		}
	}
}
