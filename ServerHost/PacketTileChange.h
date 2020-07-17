#pragma once
#include "Packet.h"
class PacketTileChange : public Packet
{
public:
	PacketTileChange();
	~PacketTileChange();

	std::string pData;

	virtual void read(int length, char* data) override;

	void write(int x, int y, int z, int pos, int id);
};

