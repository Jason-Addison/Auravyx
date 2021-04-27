#pragma once
#include "Physics/PhysicsObject.h"
#include <vector>
class PhysicsTriangles : public PhysicsObject
{
public:

	PhysicsTriangles();
	~PhysicsTriangles();
private:

	std::vector<float> triangles;
};

