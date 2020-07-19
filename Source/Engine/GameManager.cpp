#include "GameManager.h"
#include "M.h"
World GameManager::world;

std::shared_ptr<State> GameManager::currentState;

GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}

std::shared_ptr<State> GameManager::getCurrentState()
{
	return currentState;
}

void GameManager::setCurrentState(std::shared_ptr<State> s)
{
	currentState = s;
}
