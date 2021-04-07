#pragma once
/**
 * @brief 2D heightmap of density
*/
class ChunkHeight
{
public:
	ChunkHeight() = default;
	~ChunkHeight() = default;

	int x;
	int z;
	float *heightmap = new float[16384];

	float getHeight(const int x, const int z);

	void generate(const int xC, const int zC);
};

