#pragma once
#include "Packet.h"
#include "PacketMsg.h"
#include "Chunk.h";
#include <memory>
class PacketChunk : public Packet
{
public:
	PacketChunk();
	~PacketChunk();

	std::string get(std::shared_ptr<Chunk> c);

	virtual void receive(int length, char* data) override;
};

