#pragma once
#include "World/World.h"
#include "Engine/State.h"
#include <memory>
#include "Engine/State.h"
class GameManager
{
public:
	GameManager();
	~GameManager();

	static World world;

	static double UPS;

	static std::shared_ptr<State> getCurrentState();

	static void setCurrentState(std::shared_ptr<State> s);

	static double getUPS();

private:

	static std::shared_ptr<State> currentState;
};

