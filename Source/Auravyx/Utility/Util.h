#pragma once
#pragma comment(lib, "rpcrt4.lib")
#include <string>
#include <vector>
#include <string>
#include <stddef.h>     // for size_t

#define FOLDER 0

class Util
{
public:

	static void logNNL(const int colour, const std::string& text);

	static void logNNL(const int colour, const std::string& who, const std::string& message);

	static void log(const int colour, const std::string& text);

	static void log(const std::string& text);

	static void log(const int out);

	static void log(const float out);

	static void log(const int colour, const std::string& who, const std::string& message);

	static int typeOfFile(const std::string& path);

	static std::vector<std::string> listDirectory(const std::string& dir);

	static std::string readTextFile(const std::string& location);

	static void writeToFile(const std::string& dir, const std::string& data);

	static int createFolder(std::string& dir);

	static std::string removeFromString(std::string& string, const std::string& remove, const std::string& replace);

	static bool endsWith(const std::string& fullString, const std::string& ending);

	static bool startsWith(const std::string& string, const std::string& startsWith);

	static std::vector<std::string> splitString(const std::string& string, const std::string& splitter);

	static std::vector<std::string> splitString(const std::string& string, const char splitter);

	static std::string createUUID();
/*
	static std::string getFolder(const LPCWSTR& title, const LPCWSTR& dir);

	static std::string getFile(const COMDLG_FILTERSPEC& pFilter, const LPCWSTR& title, const LPCWSTR& defaultExtension, const LPCWSTR& dir);

	static std::string getFile(const COMDLG_FILTERSPEC& pFilter, const LPCWSTR& title, const LPCWSTR& defaultExtension, const LPCWSTR& dir, const int mode);
*/
	//static bool saveFile();

	static std::string replaceAll(std::string& str, const std::string& a, const std::string& b);

	static std::string removeAll(std::string str, const char a);

	static size_t countCharsInString(const std::string& string, const char c);

	static bool isCommentedLine(const std::string& line);

	static std::string getFileNameNoEXT(const std::string& path, const std::string& ext);

	static bool pointAABB(const float px, const float py, const float x, const float y, const float width, const float height);

	static std::string removeDecimal(const double x, const int precision);
};