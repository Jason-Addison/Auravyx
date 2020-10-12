#include "stdafx.h"
#include "Utilities/Log.h"
#include <iostream>
#include <Engine\OutputConsole.h>

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
	OutputConsole::getConsole()->message(converted_str, 0);
}
void Log::out(std::string msg, int colour)
{
	OutputConsole::getConsole()->message(msg, 0);
}
void Log::warn(std::string msg)
{
	OutputConsole::getConsole()->message(msg, ConsoleMessage::Type::WARNING_MESSAGE);
}
void Log::outNNL(std::string msg)
{
	OutputConsole::getConsole()->message(msg, 0);
}
void Log::error(std::string msg)
{
	OutputConsole::getConsole()->message(msg, ConsoleMessage::Type::ERROR_MESSAGE);
}
void Log::criticalError(std::string msg)
{
	OutputConsole::getConsole()->message(msg, ConsoleMessage::Type::CRITICAL_ERROR_MESSAGE);
}
void Log::outNNL(std::string msg, int colour)
{
	OutputConsole::getConsole()->message(msg, 0);
}
void Log::out(std::string src, std::string msg)
{
	out("[" + src + "] : " + msg);
}
void Log::out(std::string src, std::string msg, int colour)
{
	outNNL("[" + src + "] : " + msg + "\n");
}

void Log::out(std::string src, std::string msg, int colourA, int colourB)
{
	outNNL("[" + src + "] : " + msg + "\n", colourB);
}

void Log::out(std::string msg)
{
	OutputConsole::getConsole()->message(msg, 0);
}

void Log::update()
{
}
