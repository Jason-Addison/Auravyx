#include "Physics/PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
{
	
}

PhysicsWorld::~PhysicsWorld()
{
}

void PhysicsWorld::addObject(const std::shared_ptr<PhysicsObject>& object)
{
	objects.emplace_back(object);
}

void PhysicsWorld::update()
{
	for (auto o : objects)
	{
		if (o)
		{
			o->addImpulse(0, -9.81 / 300.0, 0);
			o->update();
			if (o->getY() < 0)
			{
				o->setYVelocity(abs(o->getYVelocity()) + 0.1);
				o->setPosition(o->getX(), 0, o->getZ());
			}
		}
	}
}
