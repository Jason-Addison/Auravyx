#pragma once
class Packet
{
public:
	Packet();
	~Packet();

	int id = 0;

	void seend();

	//virtual void send(char* data, int & length);

	virtual void receive(char * data);
};

