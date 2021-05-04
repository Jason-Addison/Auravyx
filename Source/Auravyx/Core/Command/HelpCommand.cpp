#include "Auravyx/Core/Command/HelpCommand.h"
#include "Auravyx/Core/Chat.h"
#include <Auravyx/Core/Command/CommandRegistry.h>
#include <iostream>

int pageSize = 8;
void help(int page)
{
	std::map<std::string, Command>::iterator itr = CommandRegistry::commands.begin();

	if (CommandRegistry::commands.size() / pageSize < page - 1)
	{
		Chat::message("Page " + std::to_string(page) + " does not exist!");
		return;
	}
	int offset = (pageSize * (page - 1));
	std::advance(itr, offset);
	bool firstMessage = true;
	for (int i = 0; i < pageSize; i++)
	{
		if (offset + i >= CommandRegistry::commands.size())
		{
			break;
		}
		if (firstMessage)
		{
			firstMessage = false;
			Chat::message("------- Help (" + std::to_string(page) + "/" + std::to_string((int) std::ceil((float) CommandRegistry::commands.size() / (float) pageSize)) + ") -------");
		}
		Chat::message(itr->first);
		itr++;
	}
}
void help(int page, std::string start)
{
	int foundCount = 0;
	int totalCount = 0;
	std::map<std::string, Command>::iterator itr;
	for (itr = CommandRegistry::commands.begin(); itr != CommandRegistry::commands.end(); itr++)
	{
		if (itr->first.rfind(start, 0) == 0)
		{
			totalCount++;
		}
	}
	bool firstMessage = true;
	for (itr = CommandRegistry::commands.begin(); itr != CommandRegistry::commands.end(); itr++)
	{
		if (foundCount >= (page - 1) * pageSize + pageSize)
		{
			return;
		}
		if (itr->first.rfind(start, 0) == 0)
		{
			if (foundCount >= (page - 1) * pageSize)
			{
				if (firstMessage)
				{
					firstMessage = false;
					Chat::message("------- Help (" + std::to_string(page) + "/" + std::to_string((int)std::ceil((float)totalCount / (float)pageSize)) + ") -------");
				}
				Chat::message(itr->first);
			}
			foundCount++;
		}
	}
	if (firstMessage && totalCount != 0)
	{
		Chat::message("Page " + std::to_string(page) + " does not exist for '" + start + "'!");
	}
	else if (firstMessage)
	{
		Chat::message("Command '" + start + "' does not exist!");
	}
}

void HelpCommand::load()
{
	set("help")
		.condition(Command::argument("page", Command::integer())
			.otherwise
			(
				[&](std::map<std::string, std::vector<std::string>>& s)
				{
					help(getInteger("page", s));

					return 1;
				}
			)
		)
		.condition(Command::argument("command", Command::string())
			.limits(CommandRegistry::commandStrings)
			.condition(Command::argument("page", Command::integer())
				.otherwise
				(
					[&](std::map<std::string, std::vector<std::string>>& s)
					{
						help(getInteger("page", s), s.at("command").at(0));

						return 1;
					}
				)
			)
			.otherwise
			(
				[&](std::map<std::string, std::vector<std::string>>& s)
				{
					help(1, s.at("command").at(0));

					return 1;
				}
			)
		)
		.otherwise
		(
			[&](std::map<std::string, std::vector<std::string>>& s)
			{
				if (s.size() <= 0)
				{
					help(1);
				}
				else
				{
					std::string excess = "";
					std::map<std::string, std::vector<std::string>>::iterator sItr;
					for (sItr = s.begin(); sItr != s.end(); sItr++)
					{
						excess += sItr->second.at(0);
					}

					help(2, excess);
				}
				return 1;
			}
		);
}
