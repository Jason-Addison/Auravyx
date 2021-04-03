#pragma once
#include <map>
#include <string>

#define S_IP "ip"
#define S_PORT "port"
#define S_USERNAME "username"

class Settings
{
public:
	Settings();
	~Settings();

	static std::map<std::string, std::string> settings;

	static std::string getSetting(const std::string& setting);

	static bool getBool(const std::string& setting);
};

