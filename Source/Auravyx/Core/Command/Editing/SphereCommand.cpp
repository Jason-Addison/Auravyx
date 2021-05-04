#include "Auravyx/Core/Command/Editing/SphereCommand.h"

void SphereCommand::load()
{
	set("/sphere")
		.condition(Command::argument("x", Command::integer())
			.otherwise
			(
				[](std::map<std::string, std::vector<std::string>>& s)
				{
					//TODO
					return 1;
				}
			)
		)
		.condition(Command::literal("asdf")
			.condition(Command::literal("xz")
				.otherwise
				(
					[](std::map<std::string, std::vector<std::string>>& s)
					{
						//TODO
						return 1;
					}
				)
			)
		);
}
