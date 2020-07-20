#pragma once
#include <functional>
#include <vector>
class Physics
{
public:

	static void addCallback(std::function<void(std::string)> callback);

	static void messagePhysics(std::string message);

private:
	static std::vector<std::function<void(std::string)>> physicsCallbacks;
};

