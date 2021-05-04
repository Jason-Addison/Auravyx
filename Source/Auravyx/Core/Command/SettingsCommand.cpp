#include "Auravyx/Core/Command/SettingsCommand.h"
#include <Auravyx/Graphics/GFX.h>
#include <Logger/Log.h>
#include <AL/al.h>

void SettingsCommand::load()
{
	set("settings")
		.condition(Command::literal("fov")
			.condition(Command::argument("fov", Command::integer())
				.otherwise
				(
					[](std::map<std::string, std::vector<std::string>>& s)
					{
						GFX::getOverlay()->CAM.setFOV(Command::getInteger("fov", s));
						return 1;
					}
				)
			)
		)
		.condition(Command::literal("fps")
			.condition(Command::argument("fps", Command::integer())
				.otherwise
				(
					[](std::map<std::string, std::vector<std::string>>& s)
					{
						int fps = Command::getInteger("fps", s);
						if (fps < 15 && (int)fps != -1)
						{
							Log::out("Command", "FPS can not be lower than 5!", RED);
						}
						else
						{
							Log::out("Command", "FPS set to " + std::to_string(fps), RED);
							GFX::getOverlay()->setFPS(fps);
						}
						return 1;
					}
				)
			)
		)
		.condition(Command::literal("brightness")
			.condition(Command::argument("brightness", Command::integer())
				.otherwise
				(
					[](std::map<std::string, std::vector<std::string>>& s)
					{
						//TODO//
						return 1;
					}
				)
			)
		)
		.condition(Command::literal("sound")
			.condition(Command::literal("volume")
				.condition(Command::argument("volume", Command::doubleArgument())
					.otherwise
					(
						[](std::map<std::string, std::vector<std::string>>& s)
						{
							double volume = Command::getDouble("volume", s);
							alListenerf(AL_GAIN, volume);
							return 1;
						}
					)
				)
			)
		);
}
