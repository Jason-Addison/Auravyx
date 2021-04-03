#pragma once
#include "Packet.h"
class PacketTileChange : public Packet
{
public:
	PacketTileChange();
	~PacketTileChange();

	std::string pData;

	virtual void read(const int length, const char* data) override;

	void write(const int x, const int y, const int z, const int pos, const int id);
};

