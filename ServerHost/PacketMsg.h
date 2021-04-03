#pragma once
#include <string>
class PacketMsg
{
public:
	PacketMsg();
	~PacketMsg();

	static int id;
	static void sendMessage(const std::string& msg);

	void receive(const char* data);

	static void set(const int i);
};

