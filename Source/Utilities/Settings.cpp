#pragma once
#include "Utilities/Settings.h"
#include <iostream>
Settings::Settings()
{
}


Settings::~Settings()
{
}
std::string Settings::getSetting(std::string setting)
{
	try
	{
		return settings.at(setting);
	}
	catch (std::out_of_range e)
	{
		std::cout << "Setting not found\n";
	}
	return std::string("Error : Setting not found");
}
bool Settings::getBool(std::string setting)
{
	std::string b = getSetting(setting);
	std::string bc;
	for (int i = 0; i < b.size(); i++)
	{
		bc += toupper(b.at(i));
	}
	if (bc.compare("TRUE") == 0)
	{
		return true;
	}
	else if (bc.compare("FALSE") == 0)
	{
		return false;
	}
	std::cout << "Get Bool setting error!\n";
	return false;
}
std::map<std::string, std::string> Settings::settings;