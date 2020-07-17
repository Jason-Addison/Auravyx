#include "pch.h"
#include "Client.h"

Client::Client()
{
}


Client::~Client()
{
}

#include <chrono>
#include <cstdlib>
#include <WS2tcpip.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <windows.h>
#include <string>
#pragma comment (lib, "ws2_32.lib")
/*SOCKET out;
sockaddr_in server;
void u()
{

}

LONG running = 1;

BOOL WINAPI consoleHandler(DWORD signal)
{
	//if (signal == CTRL_C_EVENT)
	//{
	char r[1];
	r[0] = 1;
	std::cout << "Closing\n";
	int dc = sendto(out, r, 1, 0, (sockaddr*)& server, sizeof(server));
	if (dc == SOCKET_ERROR)
	{
		std::cout << "Error sending (" << WSAGetLastError() << ")\n";
	}
	return TRUE;
	//}
	//return FALSE;
}

void l()
{
	if (!SetConsoleCtrlHandler(consoleHandler, TRUE))
	{
		std::cerr << "Error: " << GetLastError() << '\n';
	}


	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int WSOK = WSAStartup(version, &data);
	if (WSOK != 0)
	{
		std::cerr << "Failed to create socket!\n";
	}

	server.sin_addr.S_un.S_addr = ADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(54000);

	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

	out = socket(AF_INET, SOCK_DGRAM, 0);

	std::cout << "Enter username : ";

	std::string username;
	getline(std::cin, username);
	username = (char)1 + username;

	int sendOK = sendto(out, username.c_str(), 256, 0, (sockaddr*)& server, sizeof(server));

	int nMode = 1;
	ioctlsocket(out, FIONBIO, (u_long*)& nMode);

	sockaddr_in sin;
	int addrlen = sizeof(sin);

	if (getsockname(out, (struct sockaddr*) & sin, &addrlen) == 0 &&
		sin.sin_family == AF_INET &&
		addrlen == sizeof(sin))
	{
		int local_port = ntohs(sin.sin_port);
	}


	//if (bind(in, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		//std::cout << "Couldn't bind!\n";
	}
	//connect(out, (sockaddr*)&sin, sizeof(sin));

	//while (true)
	{
	}
	sockaddr_in serverAddr;
	int serverLength = sizeof(serverAddr);
	ZeroMemory(&serverAddr, serverLength);
	char buf[1024];

	while (true)
	{
		std::string message = "lol";
		//getline(std::cin, message);
		if (GetKeyState('A') & 0x8000)
		{
			int sendOK = sendto(out, message.c_str(), 256, 0, (sockaddr*)& server, sizeof(server));
			if (sendOK == SOCKET_ERROR)
			{
				//std::cout << "Error sending (" << WSAGetLastError() << ")\n";
			}

		}

		ZeroMemory(&buf, 1024);
		int bytesIn = recvfrom(out, buf, 1024, 0, (sockaddr*)& sin, &addrlen);
		if (bytesIn == SOCKET_ERROR)
		{
			//std::cout << "Error\n";
		}
		else
		{
			std::cout << buf << "\n";
		}
	}

	closesocket(out);

	WSACleanup();
}*/