#pragma once
#include <string>
class StringUtil
{
public:
	StringUtil();
	~StringUtil();

	static int startsWith(std::string string, std::string start);

	static int endsWith(std::string string, std::string end);
};

