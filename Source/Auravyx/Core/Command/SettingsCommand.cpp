#include "Auravyx/Core/Command/SettingsCommand.h"
#include <Auravyx/Graphics/GFX.h>
#include <Logger/Log.h>
#include <AL/al.h>
#include <Auravyx/Audio/Sound.h>
#include <Auravyx/Core/Assets.h>
#include <iostream>

Sound sound;
void SettingsCommand::load()
{
	sound.create();
	//sound.play(Assets::getAssets()->getAudio("world"));
	//sound.setGain(1);
	//sound.setPitch(1);
	//sound.setTime(0);

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
			.condition(Command::literal("pitch")
				.condition(Command::argument("pitch", Command::doubleArgument())
					.otherwise
					(
						[=](std::map<std::string, std::vector<std::string>>& s)  mutable
						{
							double pitch = Command::getDouble("pitch", s);
							sound.setPitch(pitch);
							return 1;
						}
					)
				)
			)
			.condition(Command::literal("time")
				.condition(Command::argument("time", Command::doubleArgument())
					.otherwise
					(
						[=](std::map<std::string, std::vector<std::string>>& s) mutable
						{
							double time = Command::getDouble("time", s);
							sound.setTime(time);
							return 1;
						}
					)
				)
			)
			.condition(Command::literal("play")
				.condition(Command::argument("audio", Command::string())
					.otherwise
					(
						[=](std::map<std::string, std::vector<std::string>>& s) mutable
						{
							sound.stop();
							std::cout << s.at("audio").at(0) << "\n";
							sound.play(Assets::getAssets()->getAudio(s.at("audio").at(0)));
							return 1;
						}
					)
				)
			)
		);
}
