#include "ServerManager.h"
#include "stdafx.h"
#include <chrono>
#include <cstdlib>

#include <iostream>
#include "Player.h"
#include <Vector>
#include <map>
#include <memory>
#include <thread>
#include "Server.h"
#include <atomic>
#include <time.h>

#define PORT 54000

#define CONNECTION 1
#define PLAYER_CONNECTION 2
#define COMMAND 3
#define MESSAGE 4
#define POSITION 5
#define SERVER_NAME "Test Server"

#pragma comment (lib, "ws2_32.lib")

ServerManager::ServerManager()
{
}


ServerManager::~ServerManager()
{
}

std::string title;

void ServerManager::updateServer()
{
	Server::players.size();
	std::string title = SERVER_NAME + std::string(" | Port : ");
	title.append(std::to_string(PORT));
	title.append(" | Player Count : ");
	//int succ = (int) std::to_string(Server::players.size()).at(0);
	//std::cout << succ << "\n";
	title.append(std::to_string(Server::players.size()));
	std::wstring stemp = std::wstring(title.begin(), title.end());
	LPCWSTR sw = stemp.c_str();

	//SetConsoleTitle(sw);
}
int ServerManager::detectPlayer(sockaddr_in player)
{
	int i = 0;
	for (auto& p : Server::players)
	{
		if (p->ip == player.sin_addr.S_un.S_addr && p->port == player.sin_port)
		{
			return i;
		}
		i++;
	}
	return -1;
}

void ServerManager::connectPlayer(sockaddr_in player, std::string data)
{
	char clientIP[256];
	ZeroMemory(clientIP, 256);
	inet_ntop(AF_INET, &(player.sin_addr), clientIP, 256);
	data = data.substr(1, data.length() - 1);
	std::string inc = "Player '" + data + " connecting from IP: " + clientIP + " PORT:  " + std::to_string(player.sin_port);
	std::cout << "[Server] " << inc << "\n";
	std::shared_ptr<Player> p(new Player());
	p->ip = player.sin_addr.S_un.S_addr;
	p->port = player.sin_port;
	p->name = data.c_str();
	p->address = player;

	std::string connection = (char)PLAYER_CONNECTION + p->name;
	connection = p->name + " has connected!\n";
	for (auto& u : Server::players)
	{
		u->send(connection);
	}

	Server::players.emplace_back(p);
	std::string message;
	message += "Connected to '";
	message += SERVER_NAME;
	message += "'";
	p->send(message);
	//updateServer();
}

void ServerManager::stop()
{
	running = false;
}

SOCKET in;
sockaddr_in serverHint;
void ServerManager::setup()
{
	std::wstring stemp = std::wstring(title.begin(), title.end());
	LPCWSTR sw = stemp.c_str();

	//SetConsoleTitle((LPCSTR) sw);
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int WSOK = WSAStartup(version, &data);
	if (WSOK != 0)
	{
		std::cerr << "Failed to create socket!\n";
	}
	in = socket(AF_INET, SOCK_DGRAM, 0);
	int nMode = 1;
	ioctlsocket(in, FIONBIO, (u_long*)& nMode);

	serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(PORT);

	if (bind(in, (sockaddr*)& serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		std::cout << "Couldn't bind! (" << WSAGetLastError() << ")\n";
	}
}

void updater(std::atomic<bool>& program_is_running, unsigned int update_interval_millisecs)
{
	const auto wait_duration = std::chrono::milliseconds(update_interval_millisecs);
	while (program_is_running)
	{
		printf("hi\n");
		std::this_thread::sleep_for(wait_duration);
	}
}
bool ServerManager::running = true;
double tps = 10;

auto start_time = std::chrono::high_resolution_clock::now();

void ServerManager::loopServer()
{
	sockaddr_in client;
	int clientLength = sizeof(client);
	ZeroMemory(&client, clientLength);
	char buf[1024];
	updateServer();
	double updatesPerSecond = 1 / tps;
	double lastTimeUPS = -1;
	double thisTimeUPS = 0;

	double tSinceS;

	while (running)
	{
		auto current_time = std::chrono::high_resolution_clock::now();
		tSinceS = (double) std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - start_time).count() / (double) 1000000000;

		thisTimeUPS = tSinceS;
		double deltaUpdate = thisTimeUPS - lastTimeUPS;
		if (deltaUpdate >= updatesPerSecond)
		{
			lastTimeUPS = thisTimeUPS;
			for (auto& p : Server::players)
			{
				//p->send("hi");
				//std::cout << p->ip << "   " << p->port << "\n";
			}
			//update_thread.join();
			ZeroMemory(&buf, 1024);
			int bytesIn = recvfrom(in, buf, 1024, 0, (sockaddr*)& client, &clientLength);
			if (bytesIn == SOCKET_ERROR)
			{
				//std::cout << "Error\n";
			}
			else
			{
				int clientID = detectPlayer(client);
				if (clientID == -1)
				{
					if (buf[0] == CONNECTION)
					{
						connectPlayer(client, buf);
					}
					else
					{
						std::cerr << "Incorrect connection format!";
					}
				}
				else
				{
					if (buf[0] == CONNECTION)
					{
						Server::players.at(clientID)->disconnect();
					}
					else
					{
						Server::players.at(clientID)->receive(buf);
					}
				}
				for (auto& p : Server::players)
				{
					//p->send("HI");
				}

			}
			for (auto& p : Server::players)
			{
				p->update();
			}
		}
	}
	closesocket(in);

	WSACleanup();
	for (auto p : Server::players)
	{
		//p->disconnect();
	}
	std::cout << "[Server]" << " Stopping..." << "\n";
}

void ServerManager::start()
{
	updateServer();
	setup();
	char clientIP[256];
	ZeroMemory(clientIP, 256);
	inet_ntop(AF_INET, &serverHint.sin_addr, clientIP, 256);
	std::string start =  "Started, Port : " + std::to_string(PORT) + ", IP : " + std::to_string(serverHint.sin_addr.S_un.S_addr) + ", Running at " + std::to_string((int) tps) + " TPS";
	std::cout << "[Server] " << start << "\n";
	loopServer();
}


