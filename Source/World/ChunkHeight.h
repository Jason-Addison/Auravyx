#pragma once
class ChunkHeight
{
public:
	ChunkHeight();
	~ChunkHeight();

	int x;
	int z;
	float *heightmap = new float[16384];

	float getHeight(const int x, const int z);

	void generate(const int xC, const int zC);
};

