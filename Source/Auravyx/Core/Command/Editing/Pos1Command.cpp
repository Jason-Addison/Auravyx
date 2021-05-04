#include "Auravyx/Core/Command/Editing/Pos1Command.h"
#include <Auravyx/Graphics/GFX.h>
#include <Auravyx/Core/Chat.h>

void Pos1Command::load()
{
	set("/pos1")
		.otherwise
		(
			[](std::map<std::string, std::vector<std::string>>& s)
			{
				GFX::getOverlay()->pos1x = GFX::getOverlay()->CAM.xPos;
				GFX::getOverlay()->pos1y = GFX::getOverlay()->CAM.yPos;
				GFX::getOverlay()->pos1z = GFX::getOverlay()->CAM.zPos;
				std::string msg = "Position 1 set: [" + std::to_string(GFX::getOverlay()->pos1x) + std::string(", ")
					+ std::to_string(GFX::getOverlay()->pos1y) + std::string(", ")
					+ std::to_string(GFX::getOverlay()->pos1z) + std::string("]");
				Chat::message(msg);
				return 1;
			}
	);
}
