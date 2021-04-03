#pragma once
#include <string>
#include <vector>
#include <memory>
//#include "Player.h"
class Packet
{
public:
	Packet();
	~Packet();

	static std::vector<std::shared_ptr<Packet>> packets;
	enum Type
	{
		CONNECTION,
		MESSAGE,
		TILE_CHANGE,
		TEST,
		CHUNK,
		PLAYER_POSITION_CHANGE
	};

	bool outgoing = false;
	//std::vector <std::shared_ptr<Player>> recipients;

	std::string data;

	int id = 0;

	void seend();

	//virtual void send(char* data, int & length);

	//virtual void encode

	virtual void receive(const int length, const char * data);

	virtual void read(const int length, const char* data);

	void send();

	static void loadPackets();
private:
	static void addPacket(const std::shared_ptr<Packet>& p);
};

