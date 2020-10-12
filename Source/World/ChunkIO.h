#pragma once
#include "World/Chunk.h"
#include <memory>
class ChunkIO
{
public:
	ChunkIO();
	~ChunkIO();

	void saveArea();

	void saveChunk(Chunk* chunk, std::string name);

	std::shared_ptr<Chunk> readChunk(int x, int y, int z, std::string name);

private:


};

