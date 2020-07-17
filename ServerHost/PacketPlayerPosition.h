#pragma once
#include "Packet.h"
class PacketPlayerPosition : public Packet
{
public:
	PacketPlayerPosition();
	~PacketPlayerPosition();

	std::string pData;

	void read(int length, char * data);

	void write(int x, int y, int z);
};

