#pragma once
#include <vector>
#include <sstream>
#include <fstream>
#include <map>
#include "Utilities/Util.h"
class FileIO
{
public:
	FileIO();
	~FileIO();
	static std::vector<std::string> listDirectory(std::string directory);

	static std::vector<std::string> listDirectory(std::string directory, std::string fileType);

	static int typeOfFile(std::string path);

	static std::string getFileName(std::string file);

	static std::string getFileNameNoEXT(std::string file);

	static std::string readTextFile(std::string location);

	static void writeToFile(std::string dir, std::string data);

	static std::map<std::string, std::string> readConfig(std::string dir);

	static std::vector<std::string> readLines(std::string location);
};

