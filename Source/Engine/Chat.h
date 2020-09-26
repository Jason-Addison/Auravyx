#pragma once
#include "World/World.h"
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

	void message(std::string msg);

	void command(std::string cmd);
};

