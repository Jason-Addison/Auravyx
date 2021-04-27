#pragma once
#include <vector>
#include <sstream>
#include <fstream>
#include <map>
#include "Utilities/Util.h"
class FileIO
{
public:

	static std::vector<std::string> listDirectory(const std::string& directory);

	static std::vector<std::string> listDirectory(const std::string& directory, const std::string& fileType);

	static int typeOfFile(const std::string& path);

	static std::string getFileName(const std::string& file);

	static std::string getFileNameNoEXT(const std::string& file);

	static std::string readTextFile(const std::string& location);

	static void writeToFile(const std::string& dir, const std::string& data);

	static std::map<std::string, std::string> readConfig(const std::string& dir);

	static std::vector<std::string> readLines(const std::string& location);

protected:

	FileIO() = default;
	virtual ~FileIO() = 0;
};

