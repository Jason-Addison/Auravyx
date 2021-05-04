#include "Auravyx/Core/Command/Editing/HSphereCommand.h"

void HSphereCommand::load()
{
	set("/hsphere")
		.condition(Command::argument("x", Command::integer())
			.otherwise
			(
				[](std::map<std::string, std::vector<std::string>>& s)
				{
					//TODO
					return 1;
				}
			)
		);
}
