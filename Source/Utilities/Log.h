#pragma once
#include <sstream>

#include <string>
#define BLACK 0
#define DARK_BLUE 1
#define DARK_GREEN 2
#define DARK_CYAN 3
#define DARK_RED 4
#define DARK_MAGENTA 5
#define DARK_YELLOW 6
#define LIGHT_GRAY 7
#define DARK_GRAY 8
#define BLUE 9
#define GREEN 10
#define LBLUE 11
#define RED 12
#define MAGENTA 13
#define YELLOW 14
#define WHITE 15

class Log
{
public:
	Log();
	~Log();

	static void out(const std::string& who, const std::wstringstream& msg);

	static void out(const std::string& who, const std::wstringstream& msg, const int lvl);

	static void out(const std::wstringstream& msg);

	static void out(const std::string& src, const std::string& msg, const int colour);

	static void out(const std::string& src, const std::string& msg, const int colourA, const int colourB);

	static void out(const std::string& src, const std::string& msg);

	static void out(const std::string& msg, const int colour);

	static void debug(const std::string& msg);

	static void warn(const std::string& msg);

	static void error(const std::string& msg);

	static void criticalError(const std::string& msg);

	static void outNNL(const std::string& msg, const int colour);

	static void outNNL(const std::string& msg);

	static void out(const std::string& msg);

	static void update();

	static void print(const std::string& msg);

	Log& operator <<(const std::string& msg)
	{
		print(msg);
		//*stream << msg;
		return *this;
	}
	static Log& print(Log& l, const int n);

};

