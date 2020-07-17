#pragma once
#include <vector>
#include <memory>
#include "Chunk.h"

class HostWorld
{
public:
	HostWorld();
	~HostWorld();

	std::vector<std::shared_ptr<Chunk>> overworld;

	//void setTile(int x, int y, int id);

	std::shared_ptr<Chunk> getChunk(int x, int y);
};

