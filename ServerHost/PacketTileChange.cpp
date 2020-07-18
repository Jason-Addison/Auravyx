#include "pch.h"
#include "PacketTileChange.h"
#include "M.h"
#include "GameManager.h"
#include "Server.h"
PacketTileChange::PacketTileChange()
{
	id = TILE_CHANGE;
}


PacketTileChange::~PacketTileChange()
{
}

void PacketTileChange::read(int length, char* data)
{
	int x = M::bytesToInt(data + 0);
	int y = M::bytesToInt(data + 4);
	int z = M::bytesToInt(data + 8);
	unsigned char pos = data[12];
	int id = M::bytesToInt(data + 13);
	for (int i = 0; i < 17; i++)
	{
		//ws << (int) data[i] << " ";
	}
	//if (outgoing)
	{
		std::shared_ptr<PacketTileChange> p(new PacketTileChange());
		p->write(x, y, z, pos, id);
		Server::sendToAll(p);

		//std::cout << x << " " << y << " " << id;
		GameManager::setTile(x, y, z, pos, id);
	}
}

void PacketTileChange::write(int x, int y, int z, int pos, int id)
{
	std::vector<char> xx = M::intToBytes(x);
	std::vector<char> yy = M::intToBytes(y);
	std::vector<char> zz = M::intToBytes(z);
	std::vector<char> idd = M::intToBytes(id);
	pData = "";
	//pData.clear();
	pData += xx.at(0); pData += xx.at(1); pData += xx.at(2); pData += xx.at(3);
	pData += yy.at(0); pData += yy.at(1); pData += yy.at(2); pData += yy.at(3);
	pData += zz.at(0); pData += zz.at(1); pData += zz.at(2); pData += zz.at(3);
	pData += (unsigned char)pos;
	pData += idd.at(0); pData += idd.at(1); pData += idd.at(2); pData += idd.at(3);
	data = pData;
}
