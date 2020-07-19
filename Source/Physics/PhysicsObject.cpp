#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
{

}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::getBoundsAABB(double& x, double& y, double& z)
{
	x = this->xBounds;
	y = this->yBounds;
	z = this->zBounds;
}

void PhysicsObject::updateAABB()
{
	this->xBounds = 1;
	this->yBounds = 1;
	this->zBounds = 1;
}
