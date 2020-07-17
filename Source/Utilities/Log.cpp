#include "stdafx.h"
#include "Log.h"
#include <iostream>
#include <Windows.h>

Log::Log()
{
}


Log::~Log()
{
}
void Log::out(std::string who, std::wstringstream& msg)
{
	out(who, msg, 0);
}
void Log::out(std::string who, std::wstringstream& msg, int lvl)
{
	std::wstringstream ws;
	ws << "[" << who.c_str() << "] " << msg.str();
	out(ws);
}
void Log::out(std::wstringstream &msg)
{
	std::string converted_str = std::string(msg.str().begin(), msg.str().end());
	std::cout << converted_str << "\n";
}
static HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

void Log::out(std::string msg, int colour)
{
	if (1)
	{
		SetConsoleTextAttribute(console, colour);
		std::cout << msg << std::endl;
	}
	else
	{
		std::cout << "Unsupported OS!";
	}
}
void Log::outNNL(std::string msg)
{
	SetConsoleTextAttribute(console, WHITE);
	std::cout << msg;
}
void Log::outNNL(std::string msg, int colour)
{
	SetConsoleTextAttribute(console, colour);
	std::cout << msg;
}
void Log::out(std::string src, std::string msg)
{
	out("[" + src + "] : " + msg);
}
void Log::out(std::string src, std::string msg, int colour)
{
	outNNL("[");
	outNNL(src, colour);
	outNNL("] : " + msg + "\n");
}

void Log::out(std::string src, std::string msg, int colourA, int colourB)
{
	outNNL("[");
	outNNL(src, colourA);
	outNNL("] : ");
	outNNL(msg + "\n", colourB);
}

void Log::out(std::string msg)
{
	std::cout << msg << '\n';
}

void Log::update()
{
}
