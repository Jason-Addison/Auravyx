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

	static void send(const char id, const std::string& s);
};

