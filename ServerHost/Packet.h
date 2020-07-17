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
	static enum Type
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

	virtual void receive(int length, char * data);

	virtual void read(int length, char* data);

	void send();

	static void loadPackets();
private:
	static void addPacket(std::shared_ptr<Packet> p);
};

