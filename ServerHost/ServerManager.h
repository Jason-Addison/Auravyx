#pragma once
#include <WS2tcpip.h>
#include <string>
#include <atomic>

#define SERVER "Server"
#define NORMAL 1
#define GOOD 4

class ServerManager
{
public:
	ServerManager();
	~ServerManager();

	static std::atomic<bool> running;

	static std::atomic<bool> ready;

	static std::atomic<bool> done;

	static std::string logData;

	static void log(const std::wstring& msg);

	static std::string getTime(const std::string& format);

	void setup();

	void updateServer();

	int detectPlayer(const sockaddr_in& player);

	void connectPlayer(const sockaddr_in& player, const std::string& data);

	void stop();

	void loopServer();

	void start();
void begin();
};

