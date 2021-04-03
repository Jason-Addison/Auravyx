#pragma once
#include <string>
#include <iostream>
#include <WS2tcpip.h>
#include "Packet.h"
class Player
{
public:

	std::vector< std::shared_ptr<Packet>> outPackets;

	sockaddr_in address;

	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in in;
	std::string name;
	long ip;
	long port;
	Player();
	~Player();

	void send(const std::shared_ptr<Packet>& p);

	void sendAll();

	void update();

	void send(const std::string& data);

	void send(const int id, const std::string& data);

	void receive(const std::string& data);

	void disconnect();

	void updateConnection();

	void setup();

	void ping();
};

