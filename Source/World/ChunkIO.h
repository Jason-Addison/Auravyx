#pragma once
#include "Chunk.h"
#include <memory>
class ChunkIO
{
public:
	ChunkIO();
	~ChunkIO();

	void saveArea();

	void saveChunk(std::shared_ptr<Chunk> chunk, std::string name);

	std::shared_ptr<Chunk> readChunk(int x, int y, int z, std::string name);

private:


};

