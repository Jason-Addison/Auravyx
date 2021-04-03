#pragma once
#include <WS2tcpip.h>
#include "Player.h"
#include <vector>
#include <memory>
#include <string>
#include "Packet.h"
class Server
{
public:
	Server();
	~Server();

	static std::vector<std::shared_ptr<Player>> players;

	static std::vector<std::shared_ptr<Packet>> packets;

	static int id;

	static void sendToOthers(const std::string& data, const sockaddr_in& except);

	static void sendToAll(const char id, const std::string& data);

	static void sendToAll(const std::shared_ptr<Packet>& p);
	static void add(const std::shared_ptr<Packet>& c)
	{
		c->id = id++;
		packets.emplace_back(c);
	}
	template <typename T> static void add()
	{
		std::shared_ptr<T> p(new T());
		add(p);
	}

	static int nextID();
	static void loadPackets();
};

