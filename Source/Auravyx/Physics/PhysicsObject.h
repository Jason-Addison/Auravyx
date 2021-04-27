#pragma once
#include <memory>
#include <vector>
/*
	Physics Object - Base class for any physics objects
	
	A world will be made up of physics objects. These objects can interact with one an another


*/
class PhysicsObject
{
public:

	void getBoundsAABB(double& x, double& y, double& z);

	bool checkCollision(const PhysicsObject& object);

	void setPosition(const double x, const double y, const double z);

	void addImpulse(const double x, const double y, const double z);

	double getX() const;

	double getY() const;

	double getZ() const;

	double getXVelocity() const;

	double getYVelocity() const;

	double getZVelocity() const;

	void setXVelocity(const double vel);

	void setYVelocity(const double vel);

	void setZVelocity(const double vel);

	void update();

protected:

	PhysicsObject();
	~PhysicsObject();

	struct Impulse
	{
		double xi, yi, zi;
		Impulse(const double x, const  double y, const double z)
		{
			xi = x;
			yi = y;
			zi = z;
		}
	};

	std::vector<struct Impulse> impulses;

	int type = 0;
	/*
		x / y / z : Position of a physics object globally

		(x/y/z)Scale : Scale modifiers (default 1)

		(x/y/z)Bounds : Scale of AABB bounding box, from xyz coords (axis locked)

		(x/y/z)Velocity : Global velocity

		(x/y/z)Rotation : Relative rotation

		(x/y/z)Angular : Relative angular velocity
	*/
	double x = 0, y = 0, z = 0;

	double xBounds = 1, yBounds = 1, zBounds = 1;

	double xVelocity = 0, yVelocity = 0, zVelocity = 0;

	double xRotation = 0, yRotation = 0, zRotation = 0;

	double xAngularVelocity = 0, yAngularVelocity = 0, zAngularVelocity = 0;

	virtual void updateAABB();

	virtual bool sphereCollision(const PhysicsObject& sphere);

	virtual bool prismCollision(const PhysicsObject& prism);

	virtual bool trianglesCollision(const PhysicsObject& triangles);

	enum Type
	{
		OBJECT,
		PRISM,
		SPHERE,
		TRIANGLES
	};
};

