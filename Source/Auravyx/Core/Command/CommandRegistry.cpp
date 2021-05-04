#include "Auravyx/Core/Command/CommandRegistry.h"

#include "Auravyx/Core/Command/SettingsCommand.h"
#include "Auravyx/Core/Command/HelpCommand.h"
#include "Auravyx/Core/Command/Editing/SetCommand.h"
#include "Auravyx/Core/Command/Editing/ReplaceCommand.h"
#include "Auravyx/Core/Command/Editing/SphereCommand.h"
#include "Auravyx/Core/Command/Editing/HSphereCommand.h"
#include "Auravyx/Core/Command/Editing/Pos1Command.h"
#include "Auravyx/Core/Command/Editing/Pos2Command.h"
#include "Auravyx/Core/Command/TeleportCommand.h"
#include "Auravyx/Core/Command/SaveCommand.h"
#include "Auravyx/Core/Command/MessageCommand.h"

std::map<std::string, Command> CommandRegistry::commands;
std::shared_ptr<std::vector<std::string>> CommandRegistry::commandStrings(new std::vector<std::string>());

void CommandRegistry::addCommand(Command& cmd)
{
	cmd.load();
	commands.emplace(cmd.argumentValue, cmd);
}

void CommandRegistry::loadAllStrings()
{
	std::map<std::string, Command>::iterator itr;
	for (itr = commands.begin(); itr != commands.end(); itr++)
	{
		commandStrings->emplace_back(itr->second.argumentValue);
	}
}

void CommandRegistry::loadBaseCommands()
{
	CommandRegistry::addCommand(SaveCommand());
	CommandRegistry::addCommand(MessageCommand());
	CommandRegistry::addCommand(Pos1Command());
	CommandRegistry::addCommand(Pos2Command());
	CommandRegistry::addCommand(SphereCommand());
	CommandRegistry::addCommand(HSphereCommand());
	CommandRegistry::addCommand(SetCommand());
	CommandRegistry::addCommand(HelpCommand());
	CommandRegistry::addCommand(SettingsCommand());
	CommandRegistry::addCommand(TeleportCommand());
}


