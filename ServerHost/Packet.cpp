#include "pch.h"
#include "Packet.h"
#include "PacketTileChange.h"
Packet::Packet()
{
}


Packet::~Packet()
{
}

//void Packet::seend(char* data, int& length)

void Packet::receive(const int length, const char* data)
{
	printf("%i\nerror", length);
}

void Packet::read(const int length, const char* data)
{
	printf("%i\nerror", length);
}

void Packet::send()
{
	//for (auto r : recipients)
	{
		//r->send(data);
	}
}
std::vector<std::shared_ptr<Packet>> Packet::packets;
void Packet::loadPackets()
{
	for (int i = 0; i < 20; i++)
	{
		packets.emplace_back(std::shared_ptr<Packet>(new Packet()));
	}
	//addPacket(std::shared_ptr<Packet>(new PacketChunk()));
	addPacket(std::shared_ptr<Packet>(new PacketTileChange()));
}

void Packet::addPacket(const std::shared_ptr<Packet>& p)
{
	packets.at(p->id) = p;
}

