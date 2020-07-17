#pragma once
#include <string>
#include <sstream>
#include <strstream>

class Log
{
public:
	Log();
	~Log();
	static COLORREF white;
	static COLORREF red;
	static COLORREF orange;
	static COLORREF yellow;
	static COLORREF green;
	static COLORREF lgreen;
	static void out(std::string who, std::wstringstream& msg);

	static void out(std::string who, std::wstringstream& msg, int lvl);

	static void out(std::wstringstream& msg);

	static void out(std::wstringstream& msg, COLORREF colour);

	static void serverStatus(int status);

	//static void out(std::wstringstream& msg);

	//static void out(std::wstringstream& msg);

	static void update();
};

