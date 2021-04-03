#pragma once
#include "World/Chunk.h"
#include <memory>
class ChunkIO
{
public:
	ChunkIO();
	~ChunkIO();

	void saveArea();

	void saveChunk(const Chunk* chunk, const std::string& name);

	std::shared_ptr<Chunk> readChunk(const int x, const int y, const int z, const std::string& name);
};

