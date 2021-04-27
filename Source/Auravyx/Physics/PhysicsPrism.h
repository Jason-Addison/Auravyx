#pragma once
#include "Auravyx/Physics/PhysicsObject.h"
class PhysicsPrism : public PhysicsObject
{
public:
	PhysicsPrism();
	~PhysicsPrism();
protected:

	double xScale = 1, yScale = 1, zScale = 1;
};

