#pragma once
class ChunkHeight
{
public:
	ChunkHeight();
	~ChunkHeight();

	int x;
	int z;
	int *heightmap = new int[16384];

	int getHeight(int x, int z);

	void generate(int xC, int zC);
};

