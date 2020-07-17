#include "stdafx.h"
#include "Server.h"
#include "PacketMsg.h"
std::vector<std::shared_ptr<Player>> Server::players;
std::vector<std::shared_ptr<Packet>> Server::packets;
Server::Server()
{
}


Server::~Server()
{
}
int Server::id = 0;
void Server::sendToOthers(std::string data, sockaddr_in address)
{
	for (auto& p : Server::players)
	{
		if (p->ip == address.sin_addr.S_un.S_addr && p->port == address.sin_port)
		{

		}
		else
		{
			p->send(data);
		}
	}
}

int Server::nextID()
{
	return id++;
}

void Server::loadPackets()
{
	//PacketMsg::set(nextID());
}
