#include "PacketMsg.h"

#include <iostream>
//#include "ClientManager.h"
PacketMsg::PacketMsg()
{
}

PacketMsg::~PacketMsg()
{
}

int PacketMsg::id = 0;
void PacketMsg::sendMessage(std::string msg)
{
	//ClientManager::send(id, msg);
}

void PacketMsg::receive(char* data)
{
	int len = data[0];

	std::string msg = data + 1;
	msg[len] = 0;

	std::cout << msg << "\n";
}

void PacketMsg::set(int i)
{
	id = i;
}
