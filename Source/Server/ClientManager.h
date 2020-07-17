#pragma once
#include <string>

class ClientManager
{
public:
	ClientManager();
	~ClientManager();
	void init();

	void update();

	void stop();

	static void send(char id, std::string s);
};

