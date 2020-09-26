#include "World/Voxel.h"

Voxel::Voxel()
{
    fn = Vec3f(0, 0, 0);
    average = Vec3f(0, 0, 0);
	smoothAverage = Vec3f(0, 0, 0);
    flipX = false;
    flipY = false;
    flipZ = false;
    count = 0;
}
 
Voxel::~Voxel()
{
}
 
Vec3f Voxel::getAverage()
{
    if (!avgCalc)
    {
        if (count != 0)
        {
            average.x /= count;
            average.y /= count;
            average.z /= count;
        }
        avgCalc = true;
    }
    return this->average;
}
 
void Voxel::addAverage(float x, float y, float z)
{
    average.x += x;
    average.y += y;
    average.z += z;
    count++;
}

void Voxel::addMaterial(int m)
{
	for (int i = 0; i < totalMaterial.size(); i++)
	{
		if (totalMaterial.at(i) == m)
		{
			totalMaterialCounter.at(i)++;
		}
	}
	totalMaterial.emplace_back(m);
	totalMaterialCounter.emplace_back(1);
}

void Voxel::findMostCommonMaterial()
{
	int max = -1;
	int maxMaterial = 0;
	for (int i = 0; i < totalMaterialCounter.size(); i++)
	{
		if (totalMaterialCounter.at(i) > max)
		{
			max = totalMaterialCounter.at(i);
			mat = totalMaterial.at(i);
		}
	}
}

short Voxel::getCommonMaterial()
{
	return mat;
}