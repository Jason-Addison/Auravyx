#include "pch.h"
#include "Player.h"
#include "Server.h"
#include "Log.h"
#include "LogType.h"
Player::Player()
{

}


Player::~Player()
{
	//disconnect();
}

void Player::send(std::shared_ptr<Packet> p)
{
	outPackets.emplace_back(p);
}

void Player::sendAll()
{
	std::string pData;
	int len;
	for (auto p : outPackets)
	{
		len = p->data.length();
		pData += p->id;
		pData += (len & 0xFF00) >> 8;
		pData += (len & 0x00FF);
		pData += p->data;
		send(pData);
		//std::string lol = p->data;
		//send(lol);
		pData = "";
	}
	outPackets.clear();
}

void Player::update()
{
	
}

void Player::send(std::string data)
{
	std::cout << "ERROR should not send!\n";
	sockaddr_in client;
	int clientLength = sizeof(client);
	ZeroMemory(&client, clientLength);
	int sendOK = sendto(out, data.c_str(), data.length(), 0, (sockaddr*)& address, sizeof(address));
}

void Player::send(int id, std::string data)
{
	send((char)id + data);
}

void Player::receive(std::string data)
{
	std::wstringstream wss;
	wss.clear();
	wss << " [" << name.c_str() << "] " << data.c_str() + 1 << "";
	wss.clear();
	Log::out(CHAT, wss);
	std::string g = name.c_str();
	g += " : ";
	g += data;
	Server::sendToOthers(g, address);
}

void Player::disconnect()
{
	std::wstringstream w;
	w << name.c_str() << " disconnected.";
	Log::out(SERVER, w, WARNING);
	Server::sendToOthers(name + " disconnected", address);
	//closesocket(out);
}

void Player::updateConnection()
{
	struct sockaddr_in c;
	int len = sizeof(c);
	char buf[1024];
	ZeroMemory(&buf, 1024);
	
	int bytesIn = 3;// recvfrom(out, buf, 1024, 0, (sockaddr*)& address, sizeof(address));
	std::wstringstream wss;
	//wss << bytesIn;
	int l = strlen(buf);
	if (l != 0)
	{
	//printf("LOL");
		receive(buf);
	}
}

void Player::setup()
{
	//address.sin_addr.S_un.S_addr = ADDR_ANY;
	//address.sin_family = AF_INET;

	//inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
}


