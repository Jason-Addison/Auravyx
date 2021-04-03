#include "pch.h"
#include "ServerManager.h"

#include <thread>
#include "Server.h"
#include "Utilities/Log.h"
#include <ctime>
#include "LogType.h"
#include "World/World.h"
#include "PacketTileChange.h"
#include <Engine/GameManager.h>

#define PORT 54000

#define CONNECTION 1
#define PLAYER_CONNECTION 2
#define COMMAND 3
#define MESSAGE 4
#define POSITION 5
#define SERVER_NAME "Test Server"

#pragma comment(lib, "ws2_32.lib")

std::atomic<bool> ServerManager::ready = false;
std::atomic<bool> ServerManager::done = false;
ServerManager::ServerManager()
{
	
}


ServerManager::~ServerManager()
{
}

std::string title;

std::string ServerManager::logData;

std::wstringstream wss;
void ServerManager::log(const std::wstring& msg)
{
	//wss.clear();
	//Log::out(msg);
}

std::string ServerManager::getTime(const std::string& format)
{
	std::tm bt{};
	auto t = std::time(nullptr);
	auto tm = localtime_s(&bt, &t);
	char buf[64];
	std::strftime(buf, sizeof(buf), format.c_str(), &bt);
	return std::string(buf);
}

void ServerManager::updateServer()
{
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
int ServerManager::detectPlayer(const sockaddr_in& player)
{
	int i = 0;
	for (auto& p : Server::players)
	{
		if (p->ip == player.sin_addr.S_un.S_addr && p->port == htons(player.sin_port))
		{
			return i;
		}
		i++;
	}
	return -1;
}
SOCKET in;
sockaddr_in serverHint;

void ServerManager::connectPlayer(const sockaddr_in& player, const std::string& data)
{
	std::wstringstream wss;
	char clientIP[256];
	ZeroMemory(clientIP, 256);
	inet_ntop(AF_INET, &(player.sin_addr), clientIP, 256);
	std::string dataTrim = data.substr(1, data.length() - 1);
	std::string inc = "Player '" + dataTrim + "' connecting [IP: " + clientIP + "] [PORT:  " + std::to_string(htons(player.sin_port)) + "]";
	wss << inc.c_str();
	Log::out(SERVER, wss, OKAY);
	std::shared_ptr<Player> p(new Player());
	p->ip = player.sin_addr.S_un.S_addr;
	p->port = htons(player.sin_port);
	p->name = dataTrim.c_str();
	p->address = player;
	//p->address.sin_port = htons(PORT);
	p->out = in;

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
std::atomic<bool> ServerManager::running = true;
double tps = 20;

auto start_time = std::chrono::high_resolution_clock::now();

void ServerManager::loopServer()
{
	sockaddr_in client;
	int clientLength = sizeof(client);
	ZeroMemory(&client, clientLength);
	char buf[1024 * 4];
	updateServer();
	double updatesPerSecond = 1 / tps;
	double lastTimeUPS = -1;
	double thisTimeUPS = 0;

	double tSinceS;
	//PacketChunk pc;
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			std::shared_ptr<Chunk> c(new Chunk());
			//c->generate();
			//c->x = x;
			//c->y = y;
			/////////////////////////////////////////////////////////////////////GameManager::world.overworld.emplace_back(c);
		}
	}
	while (running)
	{
		//OutputDebugString(L"A");
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
			ZeroMemory(&buf, 1024 * 4);
			int bytesIn = recvfrom(in, buf, 1024 * 4, 0, (sockaddr*)& client, &clientLength);
			int id = buf[0];
			if (bytesIn == SOCKET_ERROR)
			{
				//std::cout << "Error\n";
			}
			else
			{
				int clientID = detectPlayer(client);
				if (clientID == -1)
				{
					if (id == CONNECTION)
					{
						connectPlayer(client, buf);
						std::shared_ptr<Player> p = Server::players.at(Server::players.size() - 1);
						Log::update();
						for (int x = 0; x < 5; x++)
						{
							for (int y = 0; y < 5; y++)
							{
								//PacketChunk pc;
								
								//pc.get(GameManager::world.getChunk(x, y));
								//printf("LOL");
								//p->send(std::make_shared<PacketChunk>(pc));
								p->sendAll();
								//p->send("hi");
								p->update();
								p->updateConnection();
							}
						}
					}
					else
					{
						std::cerr << "Incorrect connection format!";
					}
				}
				else
				{
					if (id == CONNECTION)
					{
						Server::players.at(clientID)->disconnect();
						Server::players.erase(Server::players.begin() + clientID);
						Log::update();
					}
					else if (id == Packet::TILE_CHANGE)
					{
						PacketTileChange pt;
						pt.read(1000, (buf + 1));
					}
					else
					{
						Server::players.at(clientID)->receive(buf);
					}
				}
				for (auto& p : Server::players)
				{
					//p->send("HI");
					///std::shared_ptr<PacketChunk> pc(new PacketChunk(p));
					//p->send(std::make_shared<PacketChunk>(pc));
				}

			}
			for (auto& p : Server::players)
			{
				//PacketChunk pc;
				//pc.get(w.getChunk(0, 0));
				//printf("LOL");
				//p->send(std::make_shared<PacketChunk>(pc));
				//p->sendAll();
				//p->send("hi");
				//p->update();
				//p->updateConnection();
			}
		}
		else
		{
			int time = (updatesPerSecond - deltaUpdate) * 1000000;
			std::this_thread::sleep_for(std::chrono::nanoseconds(time));
		}
		ready = true;
	}
	closesocket(in);

	WSACleanup();
	for (auto p : Server::players)
	{
		//p->disconnect();
	}
	wss.clear();
	wss << "[Server]" << " Stopping..." << "\n";
	wss.clear();
}

void ServerManager::start()
{
	//OutputDebugString(L"eeeeeeeeeeeeee");
	//system("pause");
	Packet::loadPackets();
	wss.clear();
	COLORREF cf_white = RGB(255, 255, 255);
	char ip[100];
	gethostname(ip, 100);
	std::string start = "Starting on port [" + std::to_string(PORT) + "]. Server running at " + std::to_string((int)tps) + " ticks per second.";
	std::wstringstream ws;
	ws << start.c_str();
	Log::out(SERVER, ws);
	updateServer();
	setup();
	char clientIP[256];
	ZeroMemory(clientIP, 256);
	inet_ntop(AF_INET, &serverHint.sin_addr, clientIP, 256);
	//OutputDebugString(".");
	//while (!ready)
	{
		//OutputDebugString(L".");
	}
	//OutputDebugString(L".");
	ws.str(L"");
	ws.clear();
	ws << "Setup complete." << L"";
	Log::out(SERVER, ws);
	loopServer();
	done = true;
}


