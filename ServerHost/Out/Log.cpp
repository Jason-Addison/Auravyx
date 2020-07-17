#include "pch.h"
#include "Log.h"
#include <strstream>
#include "ServerHostDlg.h"
#include "ServerManager.h"
#include <time.h>
#include <iomanip>
#include <ctime>
#include "LogType.h"
Log::Log()
{
}


Log::~Log()
{
}
COLORREF Log::white = RGB(255, 255, 255);
COLORREF Log::red = RGB(255, 0, 0);
COLORREF Log::orange = RGB(255, 127, 0);
COLORREF Log::yellow = RGB(255, 255, 0);
COLORREF Log::green = RGB(0, 255, 0);
COLORREF Log::lgreen = RGB(170, 255, 0);
void Log::out(std::string who, std::wstringstream& msg)
{
	Log::out(who, msg, NORMAL);
}

void Log::out(std::string who, std::wstringstream& msg, int lvl)
{
	std::wstringstream op;
	op << " [" << who.c_str() << "] ";
	op << msg.str();
	std::wstringstream time;
	time << ServerManager::getTime("%H:%M:%S").c_str();
	CServerHostDlg::logNNL(time.str(), RGB(200, 200, 200), false, false);
	switch (lvl)
	{
		case(NORMAL):
		{
			out(op, white);
			break;
		}
		case(WARNING):
		{
			out(op, yellow);
			break;
		}
		case(ERROR):
		{
			out(op, red);
			break;
		}
		case(GOOD):
		{
			out(op, green);
			break;
		}
		case(OKAY):
		{
			out(op, lgreen);
			break;
		}
	}
}

void Log::out(std::wstringstream& msg)
{
	CServerHostDlg::log(msg.str());
}

void Log::out(std::wstringstream& msg, COLORREF colour)
{
	CServerHostDlg::log(msg.str(), colour, false, false);
}

void Log::serverStatus(int status)
{
	CServerHostDlg::serverStatus = status;
	CServerHostDlg::renderInfo();
}

void Log::update()
{
	CServerHostDlg::renderInfo();
}

