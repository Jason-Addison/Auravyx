#pragma once
#include "PhysicsObject.h"
class PhysicsSphere : public PhysicsObject
{
public:

	PhysicsSphere();
	~PhysicsSphere();

	void setDiameter(double d);

	double getDiameter();

protected:

	bool sphereCollision(PhysicsObject* sphere);

	double diameter = 1;

	PhysicsSphere* downcast(PhysicsObject* obj);
};

