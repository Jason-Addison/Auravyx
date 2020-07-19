#pragma once
#include "World.h"
#include "State.h"
#include <memory>
#include "GameState.h"
class GameManager
{
public:
	GameManager();
	~GameManager();

	static World world;

	static std::shared_ptr<State> getCurrentState();

	static void setCurrentState(std::shared_ptr<State> s);

private:

	static std::shared_ptr<State> currentState;
};

