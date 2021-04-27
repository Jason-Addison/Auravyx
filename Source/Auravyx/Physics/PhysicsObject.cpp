#include "Auravyx/Physics/PhysicsObject.h"
#include "Auravyx/Physics/Physics.h"
PhysicsObject::PhysicsObject()
{
	type = OBJECT;
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

bool PhysicsObject::checkCollision(const PhysicsObject& object)
{
	
	switch (object.type)
	{
		case(OBJECT): 
		{
			Physics::messagePhysics("Object collision with no type.");
			return false;
		}
		case(PRISM):
		{
			return prismCollision(object);
		}
		case(SPHERE):
		{
			return sphereCollision(object);
		}
		case(TRIANGLES):
		{
			return trianglesCollision(object);
		}
	}
	return false;
}

void PhysicsObject::setPosition(const double x, const double y, const double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void PhysicsObject::addImpulse(const double x, const double y, const double z)
{
	impulses.emplace_back(struct Impulse(x, y, z));
}

double PhysicsObject::getX() const
{
	return x;
}

double PhysicsObject::getY() const
{
	return y;
}

double PhysicsObject::getZ() const
{
	return z;
}

double PhysicsObject::getXVelocity() const
{
	return xVelocity;
}

double PhysicsObject::getYVelocity() const
{
	return yVelocity;
}

double PhysicsObject::getZVelocity() const
{
	return zVelocity;
}

void PhysicsObject::setXVelocity(const double vel)
{
	this->xVelocity = vel;
}

void PhysicsObject::setYVelocity(const double vel)
{
	this->yVelocity = vel;
}

void PhysicsObject::setZVelocity(const double vel)
{
	this->zVelocity = vel;
}

void PhysicsObject::update()
{
	for (auto i : impulses)
	{
		xVelocity += i.xi;
		yVelocity += i.yi;
		zVelocity += i.zi;
	}
	impulses.clear();

	x += xVelocity;
	y += yVelocity;
	z += zVelocity;
}

bool PhysicsObject::sphereCollision(const PhysicsObject& sphere)
{
	Physics::messagePhysics("Sphere collision using base class.");
	return false;
}

bool PhysicsObject::prismCollision(const PhysicsObject& prism)
{
	Physics::messagePhysics("Prism collision using base class.");
	return false;
}

bool PhysicsObject::trianglesCollision(const PhysicsObject& triangles)
{
	Physics::messagePhysics("Triangles collision using base class.");
	return false;
}

