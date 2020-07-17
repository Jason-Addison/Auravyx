#include "stdafx.h"
#include "StringUtil.h"


StringUtil::StringUtil()
{
}


StringUtil::~StringUtil()
{
}

int StringUtil::startsWith(std::string string, std::string start)
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

int StringUtil::endsWith(std::string string, std::string end)
{
	bool flag = true;
	int j = 0;
	for (int i = string.length() - end.length(); i < string.length(); i++)
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
