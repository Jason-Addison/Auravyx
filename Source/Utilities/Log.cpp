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

void Log::out(const std::string& who, const std::wstringstream& msg)
{
	out(who, msg, 0);
}
void Log::out(const std::string& who, const std::wstringstream& msg, const int lvl)
{
	std::wstringstream ws;
	ws << "[" << who.c_str() << "] " << msg.str();
	out(ws);
}
void Log::out(const std::wstringstream &msg)
{
	std::string converted_str = std::string(msg.str().begin(), msg.str().end());
	OutputConsole::getConsole()->message(converted_str, 0);
}
void Log::out(const std::string& msg, const int colour)
{
	OutputConsole::getConsole()->message(msg, 0);
}
void Log::debug(const std::string& msg)
{
	OutputConsole::getConsole()->message(msg, ConsoleMessage::Type::DEBUG_MESSAGE);
}
void Log::warn(const std::string& msg)
{
	OutputConsole::getConsole()->message(msg, ConsoleMessage::Type::WARNING_MESSAGE);
}
void Log::outNNL(const std::string& msg)
{
	OutputConsole::getConsole()->message(msg, 0);
}
void Log::error(const std::string& msg)
{
	OutputConsole::getConsole()->message(msg, ConsoleMessage::Type::ERROR_MESSAGE);
}
void Log::criticalError(const std::string& msg)
{
	OutputConsole::getConsole()->message(msg, ConsoleMessage::Type::CRITICAL_ERROR_MESSAGE);
}
void Log::outNNL(const std::string& msg, const int colour)
{
	OutputConsole::getConsole()->message(msg, 0);
}
void Log::out(const std::string& src, const std::string& msg)
{
	out("[" + src + "] : " + msg);
}
void Log::out(const std::string& src, const std::string& msg, const int colour)
{
	outNNL("[" + src + "] : " + msg + "\n");
}

void Log::out(const std::string& src, const std::string& msg, const int colourA, const int colourB)
{
	outNNL("[" + src + "] : " + msg + "\n", colourB);
}

void Log::out(const std::string& msg)
{
	OutputConsole::getConsole()->message(msg, 0);
}

void Log::update()
{
}

void Log::print(const std::string& msg)
{
	out(msg);
}

Log& Log::print(Log& l, const int n)
{
	return l;
}
