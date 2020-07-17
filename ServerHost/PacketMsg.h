#pragma once
#include <string>
class PacketMsg
{
public:
	PacketMsg();
	~PacketMsg();

	static int id;
	static void sendMessage(std::string msg);

	void receive(char* data);

	static void set(int i);
};

