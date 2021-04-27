#include "Physics/Physics.h"

void Physics::addCallback(const std::function<void(std::string)>& callback)
{
	physicsCallbacks.emplace_back(callback);
}

void Physics::messagePhysics(const std::string& message)
{
	for (auto &c : physicsCallbacks)
	{
		c(message);
	}
}

std::vector<std::function<void(std::string)>> Physics::physicsCallbacks;