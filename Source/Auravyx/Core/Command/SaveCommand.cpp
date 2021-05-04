#include "Auravyx/Core/Command/SaveCommand.h"
#include <iostream>
#include <Logger/Log.h>
#include <Auravyx/Core/GameManager.h>
#include <Auravyx/Core/World/ChunkIO.h>

void SaveCommand::load()
{
	set("save")
		.otherwise
		(
			[](std::map<std::string, std::vector<std::string>>& s)
			{
				std::cout << "aaa";
				Log::out("Command", "Saving...", RED);
				for (auto& c : GameManager::world.overworld)
				{
					Log::out("Command", std::to_string(c->x), RED);
					ChunkIO::saveChunk(c.get(), "bigworld");
				}
				Log::out("Command", "Save complete.", RED);
				return 1;
			}
	);
}
