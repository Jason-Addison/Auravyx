#include "Auravyx/Core/Command/CommandRegistry.h"

std::map<std::string, Command> CommandRegistry::commands;

void CommandRegistry::addCommand(const Command& cmd)
{
	commands.emplace(cmd.argumentValue, cmd);
}
