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

	PhysicsObject();
	~PhysicsObject();


	void getBoundsAABB(double& x, double& y, double& z);

	bool checkCollision(PhysicsObject* object);

	void setPosition(double x, double y, double z);

	void addImpulse(double x, double y, double z);

	double getX();

	double getY();

	double getZ();

	double getXVelocity();

	double getYVelocity();

	double getZVelocity();

	void setXVelocity(double vel);

	void setYVelocity(double vel);

	void setZVelocity(double vel);

	void update();

protected:

	struct Impulse
	{
		double xi, yi, zi;
		Impulse(double x, double y, double z)
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

	virtual bool sphereCollision(PhysicsObject* sphere);

	virtual bool prismCollision(PhysicsObject* prism);

	virtual bool trianglesCollision(PhysicsObject* triangles);

	enum Type
	{
		OBJECT,
		PRISM,
		SPHERE,
		TRIANGLES
	};
};

