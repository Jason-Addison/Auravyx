#include "pch.h"
#include "PacketPlayerPosition.h"
#include "M.h"
#include <sstream>
#include "Log.h"
PacketPlayerPosition::PacketPlayerPosition()
{
	id = PLAYER_POSITION_CHANGE;
}


PacketPlayerPosition::~PacketPlayerPosition()
{
}
void PacketPlayerPosition::write(int x, int y, int z)
{
	int tile = 0;
	int floor = 0;
	int plop = 0;
	std::vector<char> xx = M::intToBytes(x);
	std::vector<char> yy = M::intToBytes(y);
	std::vector<char> zz = M::intToBytes(y);
	pData = "";
	pData += xx.at(0); pData += xx.at(1); pData += xx.at(2); pData += xx.at(3);
	pData += yy.at(0); pData += yy.at(1); pData += yy.at(2); pData += yy.at(3);
	pData += zz.at(0); pData += zz.at(1); pData += zz.at(2); pData += zz.at(3);
}

void PacketPlayerPosition::read(int length, char* data)
{
	int x = M::bytesToInt(data);
	int y = M::bytesToInt(data + 4);
	int z = M::bytesToInt(data + 8);

	std::wstringstream ws;
	ws << x << " " << y << " " << z;
	Log::out(ws);
}
