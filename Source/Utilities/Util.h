#pragma once
#pragma comment(lib, "rpcrt4.lib")
#include <string>
#include <vector>
#include "shtypes.h"    // for COMDLG_FILTERSPEC
#include "xstring"      // for string
#include <minwindef.h>  // for LPCWSTR
#include <stddef.h>     // for size_t

#define FOLDER 0

class Util
{
public:

	static void logNNL(int colour, std::string text);

	static void logNNL(int colour, std::string who, std::string message);

	static void log(int colour, std::string text);

	static void log(std::string text);

	static void log(int out);

	static void log(float out);

	static void log(int colour, std::string who, std::string message);

	static int typeOfFile(std::string path);

	static std::vector<std::string> listDirectory(std::string dir);

	static std::string readTextFile(std::string location);

	static void writeToFile(std::string dir, std::string data);

	static int createFolder(std::string dir);

	static std::string removeFromString(std::string string, std::string remove, std::string replace);

	static bool endsWith(std::string& fullString, std::string& ending);

	static bool startsWith(std::string string, std::string startsWith);

	static std::vector<std::string> splitString(std::string string, std::string splitter);

	static std::vector<std::string> splitString(std::string string, char splitter);

	static std::string createUUID();

	static std::string getFolder(LPCWSTR title, LPCWSTR dir);

	static std::string getFile(COMDLG_FILTERSPEC pFilter, LPCWSTR title, LPCWSTR defaultExtension, LPCWSTR dir);

	static std::string getFile(COMDLG_FILTERSPEC pFilter, LPCWSTR title, LPCWSTR defaultExtension, LPCWSTR dir, int mode);

	static bool saveFile();

	static std::string replaceAll(std::string str, std::string a, std::string b);

	static std::string removeAll(std::string str, char a);

	static size_t countCharsInString(std::string string, char c);

	static bool isCommentedLine(std::string line);

	static std::string getFileNameNoEXT(std::string path, std::string ext);

	static bool pointAABB(float px, float py, float x, float y, float width, float height);

	static std::string removeDecimal(double x, int precision);
};