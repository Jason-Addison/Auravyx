#include "MSVC/pch.h"
#include "Auravyx/Utility/StringUtil.h"


StringUtil::StringUtil()
{
}


StringUtil::~StringUtil()
{
}

int StringUtil::startsWith(const std::string& string, const std::string& start)
{
	bool flag = true;
	for (int i = 0; i < string.length(); i++)
	{
		if (i < start.length())
		{
			if (string.at(i) != start.at(i))
			{
				flag = false;
				return flag;
			}
		}
	}
	return flag;
}

int StringUtil::endsWith(const std::string& string, const std::string& end)
{
	bool flag = true;
	int j = 0;
	for (int i = string.length() - end.length(); i < (int) string.length(); i++)
	{
		if (string.at(i) != end.at(j))
		{
			flag = false;
			return flag;
		}
		j++;
	}
	return flag;
}
