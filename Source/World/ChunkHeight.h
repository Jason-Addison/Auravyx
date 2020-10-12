#pragma once
class ChunkHeight
{
public:
	ChunkHeight();
	~ChunkHeight();

	int x;
	int z;
	float *heightmap = new float[16384];

	float getHeight(int x, int z);

	void generate(int xC, int zC);
};

