#include "ClientManager.h"
#include <chrono>
#include <cstdlib>
#include <WS2tcpip.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <windows.h>
#include <string>
#include "Controller.h"
#include "GLFW/glfw3.h"
#include "FileIO.h"
#include "Resource.h"
#include <chrono>
#include <thread>
#include "Settings.h"
#include "Packet.h"
//#include "PacketChunk.h"
#pragma comment (lib, "ws2_32.lib")

bool host = false;
ClientManager::ClientManager()
{
	host = Settings::getBool("host");
	init();
}


ClientManager::~ClientManager()
{
}
SOCKET out;
sockaddr_in server;
int addrlen;
sockaddr_in serverin;
sockaddr_in serverAddr;
int serverLength = sizeof(serverin);
char buf[1024 * 2];
std::string port;
std::string ip;
void ClientManager::init()
{
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int WSOK = WSAStartup(version, &data);
	if (WSOK != 0)
	{
		std::cerr << "Failed to create socket!\n";
	}
	std::string username;

	ip = Settings::getSetting(S_IP);
	port = Settings::getSetting(S_PORT);
	username = Settings::getSetting(S_USERNAME);
	

	server.sin_addr.S_un.S_addr = ADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(std::stoi(port));
	inet_pton(AF_INET, ip.c_str(), &server.sin_addr);
	//inet_pton(AF_INET, "52.15.110.244", &server.sin_addr);
	out = socket(AF_INET, SOCK_DGRAM, 0);
	//std::cout << "Enter username : ";
	//std::cin >> username;
	//Sleep(5000);

	//username =  + username;
	//int sendOK = sendto(out, username.c_str(), 256, 0, (sockaddr*)& server, sizeof(server));
	send(1, username);
	int nMode = 1;
	ioctlsocket(out, FIONBIO, (u_long*)& nMode);
	//listen(out, nMode);
	addrlen = sizeof(serverin);

	if (getsockname(out, (struct sockaddr*) & serverin, &addrlen) == 0 &&
		serverin.sin_family == AF_INET &&
		addrlen == sizeof(serverin))
	{
		int local_port = ntohs(serverin.sin_port);
	}
	else
	{
		printf("Error\n");
	}
	//char* inet_ntoa(struct in_addr addr);
	char clientIP[256];
	ZeroMemory(clientIP, 256);
	serverin.sin_addr.S_un.S_addr = server.sin_addr.S_un.S_addr;// inet_addr(ip.c_str());
	inet_ntop(AF_INET, &(serverin.sin_addr), clientIP, 256);
//	ZeroMemory(&serverin, serverLength);

	int serverLength = sizeof(serverAddr);
	//ZeroMemory(&serverAddr, serverLength);
}
void ClientManager::update()
{
	std::string msg = "hello";
	if (Controller::isKeyDown(GLFW_KEY_0))
	{
		int sendOK = sendto(out, msg.c_str(), 256, 0, (sockaddr*)&server, sizeof(server));
		if (sendOK == SOCKET_ERROR)
		{
			std::cout << "Error sending (" << WSAGetLastError() << ")\n";
		}
	}
	std::vector<Packet> packets;
	//packets.emplace_back(PacketChunk());
	//PacketChunk pc = PacketChunk();
	//if (!host)
	{
		//std::cout << htons(serverin.sin_port) << "\n";
		//std::cout << htons(serverin.sin_port) << "\n";
		ZeroMemory(&buf, 1024 * 2);
		int bytesIn = recvfrom(out, buf, 1024 * 2, 0, (sockaddr*)& serverin, &addrlen);
		
		if (bytesIn == SOCKET_ERROR)
		{
			//std::cout << "Error\n";
		}
		else
		{
			short id = buf[0];
			short len = buf[1];
			len <<= 8;
			len |= buf[2];

			if (id == Packet::Type::CHUNK)
			{
				Packet::packets.at(id)->receive(len, buf + 3);
			}
			else if (id == Packet::Type::TILE_CHANGE)
			{
				//std::cout << "LOL\n";
				Packet::packets.at(id)->read(len, buf + 3);
			}
			else
			{
				std::cout << "UIP Error : [Server] " << buf << "\n";
			}
			/*switch (id)
			{
				case(Packet::Type::CHUNK):
				{
					//printf("LOL");
					pc.receive(len, buf + 3);
					break;
				};
				default:
				{
					std::cout << "[Server] " << buf << "\n";
					break;
				};
			}*/
		}
	}
	//send(4, "sadopkdsakoidsakopisdakpoisdakpodsaksapdpkodsakodpsakpodsapoksdakopdsakopsdakopdskapokopdsakposadkpodsakdsapkodspaokkpdosakpdosakpodaskpodsakskadpodakop");
}

void ClientManager::stop()
{
	send((char)1, "");
	closesocket(out);

	WSACleanup();
}
void ClientManager::send(char id, std::string s)
{
	s = id + s;
	int sendOK = sendto(out, s.c_str(), s.length(), 0, (sockaddr*)& server, sizeof(server));
	if (sendOK == SOCKET_ERROR)
	{
		std::cout << "Error sending (" << WSAGetLastError() << ")\n";
	}
}
