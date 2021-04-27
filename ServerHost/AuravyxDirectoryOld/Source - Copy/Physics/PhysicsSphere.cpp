#include "Physics/PhysicsSphere.h"
#include "Physics/Physics.h"
PhysicsSphere::PhysicsSphere()
{
	type = SPHERE;
}

PhysicsSphere::~PhysicsSphere()
{
}

void PhysicsSphere::setDiameter(const double d)
{
	this->diameter = d;
}

double PhysicsSphere::getDiameter()
{
	return this->diameter;
}

bool PhysicsSphere::sphereCollision(PhysicsObject* sphere)
{
	double distance = sqrt(pow(x - sphere->getX(), 2) + pow(y - sphere->getY(), 2) + pow(z - sphere->getZ(), 2));
	if (distance <= getDiameter() + downcast(sphere)->getDiameter())
	{
		return true;
	}
	return false;
}

PhysicsSphere* PhysicsSphere::downcast(PhysicsObject* obj)
{
	PhysicsSphere* s = dynamic_cast<PhysicsSphere*>(obj);
	if (s)
	{
		return s;
	}
	Physics::messagePhysics("Sphere downcast failed");
	return nullptr;
}
