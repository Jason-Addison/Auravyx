#pragma once
#include <WS2tcpip.h>
#include <string>
class ServerManager
{
public:
	ServerManager();
	~ServerManager();

	static bool running;

	void setup();

	void updateServer();

	int detectPlayer(sockaddr_in player);

	void connectPlayer(sockaddr_in player, std::string data);

	void stop();

	void loopServer();

	void start();
};

