#pragma once
#include <string>
#include <iostream>
#include <WS2tcpip.h>

class Player
{
public:

	sockaddr_in address;

	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	std::string name;
	long ip;
	long port;
	Player();
	~Player();

	void update();

	void send(std::string data);

	void receive(std::string data);

	void disconnect();

	void setup();

	void ping();
};

