#pragma once
#include "Packet.h"
class PacketPlayerPosition : public Packet
{
public:
	PacketPlayerPosition();
	~PacketPlayerPosition();

	std::string pData;

	void read(const int length, const char * data);

	void write(const int x, const int y, const int z);
};

