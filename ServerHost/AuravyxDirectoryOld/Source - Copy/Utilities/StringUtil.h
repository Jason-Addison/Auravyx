#pragma once
#include <string>
#include <string>
class StringUtil
{
public:
	StringUtil();
	~StringUtil();

	static int startsWith(const std::string& string, const std::string& start);

	static int endsWith(const std::string& string, const std::string& end);
};

