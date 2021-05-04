#pragma once
#include <map>
#include <string>
#include "Auravyx/Core/Command/Command.h"
#include <memory>
class CommandRegistry
{
public:
	
	static std::map<std::string, Command> commands;
	static std::shared_ptr<std::vector<std::string>> commandStrings;

	static void addCommand(Command& cmd);

	static void loadAllStrings();

	static void loadBaseCommands();
};