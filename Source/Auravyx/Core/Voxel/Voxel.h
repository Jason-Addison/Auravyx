#pragma once
#include <string>
class Voxel
{
public:

	std::string name;

protected:

	Voxel() = default;
	virtual ~Voxel() = 0;
};