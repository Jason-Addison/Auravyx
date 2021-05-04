
#include "MessageCommand.h"

#include "Auravyx/Core/Command/MessageCommand.h"

void MessageCommand::load()
{
	set("msg")
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
