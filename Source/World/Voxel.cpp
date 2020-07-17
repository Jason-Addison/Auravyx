#include "Voxel.h"

#include <vector>

Voxel::Voxel()
{
	smoothAverage = Vec3f(0, 0, 0);
}


Voxel::~Voxel()
{
}

Model Voxel::generate()
{
	std::vector<float> vertices;


	Model m = Model::load3DModel(vertices);
	return 0;
}
