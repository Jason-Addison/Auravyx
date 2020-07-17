#include "pch.h"
#include "PacketChunk.h"
#include "Chunk.h"
#include "GameManager.h";
#include "M.h"
PacketChunk::PacketChunk()
{
	id = CHUNK;
}
PacketChunk::~PacketChunk()
{
}

std::string PacketChunk::get(std::shared_ptr<Chunk> c)
{
	int tile = 0;
	int floor = 0;
	int plop = 0;
	std::string cData;
	id = CHUNK;
	std::vector<char> x = M::intToBytes(c->x);
	std::vector<char> y = M::intToBytes(c->y);
	cData += x.at(0); cData += x.at(1); cData += x.at(2); cData += x.at(3);
	cData += y.at(0); cData += y.at(1); cData += y.at(2); cData += y.at(3);
	for (int i = 0; i < c->size; i++)
	{
		/* Tile */
		//cData += (c->tiles[i].tile & 0xFF00) >> 8;
		//cData += (c->tiles[i].tile & 0x00FF);
		/* Floor */
		//cData += (c->tiles[i].floor & 0xFF00) >> 8;
		//cData += (c->tiles[i].floor & 0x00FF);
		/* Plop */
		//cData += (c->tiles[i].plop & 0xFF00) >> 8;
		//cData += (c->tiles[i].plop & 0x00FF);



		///////////////////////////////
		//cData += (unsigned char)c->tiles[i].tile;
		//cData += (unsigned char)c->tiles[i].floor;
		//cData += (unsigned char) c->tiles[i].plop;
	}
	data = cData;
	return cData;
}

void PacketChunk::receive(int length, char* data)
{
	GameManager::loadChunk(length, data);
}
