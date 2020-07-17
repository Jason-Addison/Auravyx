#pragma once
#include "TileData.h"
class HostChunk
{
public:
	HostChunk();
	~HostChunk();

	const static int chunkSize = 16;
	const static int chunkHeight = 1;
	int x = 0, y = 0;
	const static int size = chunkSize * chunkHeight * chunkSize;
	//GLuint vbo;
	TileData tiles[size];

	void generate();

	void setTile(int x, int y, int id);
};

