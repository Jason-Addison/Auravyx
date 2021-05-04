#include "Auravyx/Core/Command/Editing/Pos2Command.h"
#include <Auravyx/Graphics/GFX.h>
#include <Auravyx/Core/Chat.h>

void Pos2Command::load()
{
	set("/pos2")
		.otherwise
		(
			[](std::map<std::string, std::vector<std::string>>& s)
			{
				GFX::getOverlay()->pos2x = GFX::getOverlay()->CAM.xPos;
				GFX::getOverlay()->pos2y = GFX::getOverlay()->CAM.yPos;
				GFX::getOverlay()->pos2z = GFX::getOverlay()->CAM.zPos;
				std::string msg = "Position 2 set: [" + std::to_string(GFX::getOverlay()->pos2x) + std::string(", ")
					+ std::to_string(GFX::getOverlay()->pos2y) + std::string(", ")
					+ std::to_string(GFX::getOverlay()->pos2z) + std::string("]");
				Chat::message(msg);
				return 1;
			}
	);
}
