#include "Auravyx/Core/Command/Editing/SetCommand.h"
#include <Auravyx/Graphics/GFX.h>
#include <cmath>
#include <Auravyx/Core/Chat.h>
#include <Auravyx/Core/GameManager.h>

void setRegion(int id, int density)
{
	std::string msg = "Set ID: " + std::to_string(id) + " from [" + std::to_string(GFX::getOverlay()->pos1x) + std::string(", ")
		+ std::to_string(GFX::getOverlay()->pos1y) + std::string(", ")
		+ std::to_string(GFX::getOverlay()->pos1z) + std::string("] to [")
		+ std::to_string(GFX::getOverlay()->pos2x) + std::string(", ")
		+ std::to_string(GFX::getOverlay()->pos2y) + std::string(", ")
		+ std::to_string(GFX::getOverlay()->pos2z) + std::string("] (")
		+ std::to_string((abs(GFX::getOverlay()->pos1x - GFX::getOverlay()->pos2x))
			* (abs(GFX::getOverlay()->pos1y - GFX::getOverlay()->pos2y))
			* (abs(GFX::getOverlay()->pos1z - GFX::getOverlay()->pos2z))) + std::string(")");

	Chat::message(msg);

	int xC1 = floor((GFX::getOverlay()->pos1x * 1) / Chunk::CHUNK_SIZE);
	int yC1 = floor((GFX::getOverlay()->pos1y * 1) / Chunk::CHUNK_SIZE);
	int zC1 = floor((GFX::getOverlay()->pos1z * 1) / Chunk::CHUNK_SIZE);

	int xC2 = floor((GFX::getOverlay()->pos2x * 1) / Chunk::CHUNK_SIZE);
	int yC2 = floor((GFX::getOverlay()->pos2y * 1) / Chunk::CHUNK_SIZE);
	int zC2 = floor((GFX::getOverlay()->pos2z * 1) / Chunk::CHUNK_SIZE);

	for (int xC = std::min(xC1, xC2); xC <= std::max(xC1, xC2); xC++)
	{
		for (int yC = std::min(yC1, yC2); yC <= std::max(yC1, yC2); yC++)
		{
			for (int zC = std::min(zC1, zC2); zC <= std::max(zC1, zC2); zC++)
			{
				Chunk* c = GameManager::world.getChunk(xC, yC, zC);
				if (c)
				{
					int x1 = ((int)floor(GFX::getOverlay()->pos1x)) % 64;
					int y1 = ((int)floor(GFX::getOverlay()->pos1y)) % 64;
					int z1 = ((int)floor(GFX::getOverlay()->pos1z)) % 64;

					/*if (x < 0)
					{
						x = Chunk::CHUNK_SIZE + x;
					}
					if (y < 0)
					{
						y = Chunk::CHUNK_SIZE + y;
					}
					if (z < 0)
					{
						z = Chunk::CHUNK_SIZE + z;
					}*/
					c->chunkUpdate = true;
					c->priorityLoad = true;
					c->loaded = false;
					for (int x = std::max(0, std::min((int)floor(GFX::getOverlay()->pos1x), (int)floor(GFX::getOverlay()->pos2x)) - xC * Chunk::CHUNK_SIZE);
						x <= std::min(Chunk::CHUNK_SIZE - 1, std::max((int)floor(GFX::getOverlay()->pos1x), (int)floor(GFX::getOverlay()->pos2x)) - xC * Chunk::CHUNK_SIZE); x++)
					{
						for (int y = std::max(0, std::min((int)floor(GFX::getOverlay()->pos1y), (int)floor(GFX::getOverlay()->pos2y)) - yC * Chunk::CHUNK_SIZE);
							y <= std::min(Chunk::CHUNK_SIZE - 1, std::max((int)floor(GFX::getOverlay()->pos1y), (int)floor(GFX::getOverlay()->pos2y)) - yC * Chunk::CHUNK_SIZE); y++)
						{
							for (int z = std::max(0, std::min((int)floor(GFX::getOverlay()->pos1z), (int)floor(GFX::getOverlay()->pos2z)) - zC * Chunk::CHUNK_SIZE);
								z <= std::min(Chunk::CHUNK_SIZE - 1, std::max((int)floor(GFX::getOverlay()->pos1z), (int)floor(GFX::getOverlay()->pos2z)) - zC * Chunk::CHUNK_SIZE); z++)
							{
								c->setVoxel(x, y, z, id, density);
							}
						}
					}
				}
			}
		}
	}
}

void SetCommand::load()
{
	set("/set")
		.condition(Command::argument("id", Command::integer())
			.condition(Command::argument("density", Command::integer())
				.otherwise
				(
					[&](std::map<std::string, std::vector<std::string>>& s)
					{
						int id = Command::getInteger("id", s);
						int density = Command::getInteger("density", s);
						setRegion(id, density);

						return 1;
					}
				)
			)
			.otherwise
			(
				[&](std::map<std::string, std::vector<std::string>>& s)
				{
					int id = Command::getInteger("id", s);

					if (id == 0)
					{
						setRegion(id, 0);
					}
					else
					{
						setRegion(id, 255);
					}

					return 1;
				}
			)
		);
}
