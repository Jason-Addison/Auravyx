#include "stdafx.h"
#include "Player.h"
#include "Server.h"

Player::Player()
{

}


Player::~Player()
{
	disconnect();
}

void Player::update()
{
}

void Player::send(std::string data)
{
	//std::cout << address.sin_port << "\n";
	int sendOK = sendto(out, data.c_str(), data.length(), 0, (sockaddr*)& address, sizeof(address));
	//printf("%i", sendOK);
	/*while (true)
	{
		std::string message;
		getline(std::cin, message);

		int sendOK = sendto(out, message.c_str(), 256, 0, (sockaddr*)&server, sizeof(server));

		if (sendOK == SOCKET_ERROR)
		{
			std::cout << "Welp :( \n";
		}
	}*/
}

void Player::receive(std::string data)
{
	std::cout << name << " : " << data << "\n";
	Server::sendToOthers(data, address);
}

void Player::disconnect()
{
	std::cout << name << " disconnected\n";
	Server::sendToOthers(name + " disconnected\n", address);
	closesocket(out);
}

void Player::setup()
{
	//address.sin_addr.S_un.S_addr = ADDR_ANY;
	//address.sin_family = AF_INET;

	//inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
}


