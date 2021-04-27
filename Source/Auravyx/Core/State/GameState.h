#pragma once
#include "Auravyx/Core/State/State.h"
class GameState : public State
{
public:

	GameState();
	~GameState();

	void update();

	void render();

	void start();

	void stop();
};

