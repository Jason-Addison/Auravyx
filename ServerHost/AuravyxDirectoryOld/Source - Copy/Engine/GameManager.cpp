#include "Engine/GameManager.h"
#include "Utilities/M.h"
World GameManager::world;

std::shared_ptr<State> GameManager::currentState;

double GameManager::UPS = 60;

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

void GameManager::setCurrentState(const std::shared_ptr<State>& s)
{
	currentState = s;
}

double GameManager::getUPS()
{
	return UPS;
}
