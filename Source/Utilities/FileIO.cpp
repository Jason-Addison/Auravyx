#include "stdafx.h"
#include "FileIO.h"
#include <filesystem>
#include <iostream>
#include "FileIO.h"
#include <Windows.h>
#include "StringUtil.h"
#include <sstream>
#include "Log.h"
#define FOLDER 0
FileIO::FileIO()
{
}


FileIO::~FileIO()
{
}

std::vector<std::string> FileIO::listDirectory(std::string directory, std::string fileType)
{
	std::vector<std::string> list;
	std::vector<std::string> files = listDirectory(directory);
	for (auto & i : files)
	{
		std::string fileEnding = ".";
		fileEnding += fileType;
		if (StringUtil::endsWith(i, fileEnding))
		{
			list.emplace_back(i);
		}
		if (FileIO::typeOfFile(i) == FOLDER)
		{
			std::vector<std::string> subfolder = listDirectory(i, fileType);
			list.insert(list.begin(), subfolder.begin(), subfolder.end());
		}
	}

	return list;
}

std::vector<std::string> FileIO::listDirectory(std::string directory)
{
	std::vector<std::string> items;
	for (auto & p : std::filesystem::directory_iterator(directory))
	{
		std::ostringstream oss;
		oss << absolute(p.path()).string();
		items.emplace_back(oss.str());
	}
	return items;
}

int FileIO::typeOfFile(std::string path)
{
	struct stat s;
	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
		{
			return 0;
		}
		else if (s.st_mode & S_IFREG)
		{
			return 1;
		}
		else
		{
			return 3;
		}
	}
	else
	{
		return -1;
	}
}

std::string FileIO::getFileName(std::string file)
{
	char sep = '\\';

	int os = 0;
	if (os)
	{
		sep = '\\';
	}

	unsigned long long k = file.rfind(sep, file.length());
	if (k != std::string::npos)
	{
		file = file.substr(k + 1, file.length() - k);
	}
	return file;
}

std::string FileIO::getFileNameNoEXT(std::string file)
{
	std::string s = getFileName(file);
	s = s.substr(0, s.length() - 4);
	return s;
}

std::string FileIO::readTextFile(std::string location)
{
	std::ifstream inStream(location, std::ifstream::in);
	std::stringstream stream;
	stream << inStream.rdbuf();

	return stream.str();
}

void FileIO::writeToFile(std::string dir, std::string data)
{
	std::ofstream file;
	file.open(dir);
	file << data;
	file.close();
}

std::map<std::string, std::string> FileIO::readConfig(std::string dir)
{
	std::map<std::string, std::string> config;
	std::vector<std::string> lines = readLines(dir);
	for (auto l : lines)
	{
		if (l.size() > 0 && l.at(0) != '#')
		{
			std::vector<std::string> line = Util::splitString(l, "=");
			config.emplace(line.at(0), line.at(1));
		}
	}
	return config;
}

std::vector<std::string> FileIO::readLines(std::string location)
{
	std::string file = readTextFile(location);
	std::vector<std::string> lines = Util::splitString(file, "\n");
	return lines;
}
