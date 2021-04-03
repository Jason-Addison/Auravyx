#include "pch.h"
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
void Server::sendToOthers(const std::string& data, const sockaddr_in& address)
{
	for (auto& p : Server::players)
	{
		if (p->ip == address.sin_addr.S_un.S_addr && p->port == htons(address.sin_port))
		{

		}
		else
		{
			p->send(data);
		}
	}
}

void Server::sendToAll(const char id, const std::string& data)
{
	for (auto p : players)
	{
		p->send((char)id + data);
	}
}

void Server::sendToAll(const std::shared_ptr<Packet>& p)
{
	for (auto pl : players)
	{
		pl->send(p);
		pl->sendAll();
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
