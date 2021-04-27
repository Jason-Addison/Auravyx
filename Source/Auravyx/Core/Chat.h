#pragma once
#include "Auravyx/Core/World/World.h"
#include <string>
class Chat
{
public:
	Chat();
	~Chat();

	World *w;

	static bool isChatting;

	void update();

	void render();


private:

	void message(const std::string& msg);

	void command(const std::string& cmd);
};

