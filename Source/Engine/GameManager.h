#pragma once
#include "World.h"
class GameManager
{
public:
	GameManager();
	~GameManager();

	static World world;

	static void loadChunk(int length, char* data);

	static void setTile(int x, int y, int z, int pos, int id);

	//static void sendTile(int x, int y, int z, int pos, int id);
};

