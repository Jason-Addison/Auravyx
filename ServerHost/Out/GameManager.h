#pragma once
#include "HostWorld.h"
class GameManager
{
public:
	GameManager();
	~GameManager();

	static HostWorld world;

	static void loadChunk(int length, char* data);

	static void setTile(int x, int y, int z, int pos, int id);
};

