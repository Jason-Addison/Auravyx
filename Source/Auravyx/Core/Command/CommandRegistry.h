#pragma once
#include <map>
#include <string>
#include "Auravyx/Core/Command/Command.h"
class CommandRegistry
{
public:
	
	static std::map<std::string, Command> commands;

	static void addCommand(const Command& cmd);
};