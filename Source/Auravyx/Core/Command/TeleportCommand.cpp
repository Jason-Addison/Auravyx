#include "Auravyx/Core/Command/TeleportCommand.h"
#include <Auravyx/Graphics/GFX.h>

TeleportCommand::TeleportCommand()
{
	//setCommandName("tp");
}

void TeleportCommand::load()
{
	set("tp")
		.condition(Command::argument("x", Command::doubleArgument())
			.condition(Command::argument("y", Command::doubleArgument())
				.condition(Command::argument("z", Command::doubleArgument())
					.otherwise
					(
						[](std::map<std::string, std::vector<std::string>>& s)
						{
							GFX::getOverlay()->CAM.xPos = Command::getDouble("x", s);
							GFX::getOverlay()->CAM.yPos = Command::getDouble("y", s);
							GFX::getOverlay()->CAM.zPos = Command::getDouble("z", s);
							return 1;
						}
					)
				)
			)
		);
}
