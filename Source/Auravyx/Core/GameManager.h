#pragma once
#include "Auravyx/Core/World/World.h"
#include "Auravyx/Core/State/State.h"
#include <memory>
class GameManager
{
public:
	GameManager();
	~GameManager();

	static World world;

	static double UPS;

	static std::shared_ptr<State> getCurrentState();

	static void setCurrentState(const std::shared_ptr<State>& s);

	static double getUPS();

private:

	static std::shared_ptr<State> currentState;
};

