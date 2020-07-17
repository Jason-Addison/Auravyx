#pragma once
#include "Chunk.h"
#include <memory>
class ChunkIO
{
public:
	ChunkIO();
	~ChunkIO();

	void saveChunk(std::shared_ptr<Chunk> chunk);

	std::shared_ptr<Chunk> readChunk(int x, int y, int z);

private:


};

