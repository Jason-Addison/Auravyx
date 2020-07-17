#include "pch.h"
#include "GameManager.h"
#include "M.h"
GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}
HostWorld GameManager::world;

void GameManager::loadChunk(int length, char* data)
{

}

void GameManager::setTile(int x, int y, int z, int pos, int id)
{
	world.getChunk(x, y)->setTile(pos / 16, pos % 16, id);
}
