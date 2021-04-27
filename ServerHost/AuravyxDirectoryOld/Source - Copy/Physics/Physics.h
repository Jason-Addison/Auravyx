#pragma once
#include <functional>
#include <vector>
#include <string>
class Physics
{
public:

	static void addCallback(const std::function<void(std::string)>& callback);

	static void messagePhysics(const std::string& message);

private:
	static std::vector<std::function<void(std::string)>> physicsCallbacks;
};

