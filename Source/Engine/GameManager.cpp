#include "GameManager.h"
#include "M.h"

World GameManager::world;
GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}

void GameManager::loadChunk(int length, char* data)
{
	int tile = 0;
	int floor = 0;
	int plop = 0;
	std::shared_ptr<Chunk> c(new Chunk());
	int i = 0;
	//c->x = M::bytesToInt(data);
	//c->y = M::bytesToInt(data + 4);
	//printf("%i %i \n", c->x, c->y);
	data += 8;
	//for (int p = 0; p < Chunk::size; p++)
	{
		//i = p * 3;
		/*tile = data[i + 0];
		tile <<= 8;
		tile |= data[i + 1];

		floor = data[i + 2];
		floor <<= 8;
		floor |= data[i + 3];

		plop = data[i + 4];
		plop <<= 8;
		plop |= data[i + 5];*/

		//////c->tiles[p].tile = data[i + 0];
		//////c->tiles[p].floor = data[i + 1];
		//////c->tiles[p].plop = data[i + 2];
	}

	world.addChunk(c);
}

void GameManager::setTile(int x, int y, int z, int pos, int id)
{
	//world.getChunk(x, y)->setTile(pos % 16, pos / 16, id);
}
