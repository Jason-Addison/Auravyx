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

Vec3f Voxel::getLiquidAverage()
{
    if (!avgCalcL)
    {
        if (avgLiq.size() != 0)
        {
            for (auto v : avgLiq)
            {
                averageLiquid.x += v->smoothAverage.x;
                averageLiquid.y += v->smoothAverage.y;
                averageLiquid.z += v->smoothAverage.z;
            }
            averageLiquid.x /= avgLiq.size();
            averageLiquid.y /= avgLiq.size();
            averageLiquid.z /= avgLiq.size();
            
            //averageLiquid.x /= countL;
            //averageLiquid.y /= countL;
            //averageLiquid.z /= countL;
        }
        avgCalcL = true;
    }
    return this->averageLiquid;
}

void Voxel::addAverage(const float x, const float y, const float z, const int id)
{
    addLiquidAverage(x, y, z);
   
    {
        average.x += x;
        average.y += y;
        average.z += z;
        count++;
    }
}

void Voxel::addMaterial(const int m)
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
#include <iostream>
void Voxel::addLiquidAverage(const float x, const float y, const float z)
{
    averageLiquid.x += x;
    averageLiquid.y += y;
    averageLiquid.z += z;
    countL++;
}

void Voxel::addLiquidAverage(Voxel* vox)
{
    avgLiq.emplace_back(vox);
}

void Voxel::addLiquidMaterial(const int m)
{
    for (int i = 0; i < totalLiquidMaterial.size(); i++)
    {
        if (totalLiquidMaterial.at(i) == m)
        {
            totalLiquidMaterialCounter.at(i)++;
        }
    }
    totalLiquidMaterial.emplace_back(m);
    totalLiquidMaterialCounter.emplace_back(1);
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

void Voxel::findMostCommonLiquid()
{
}

short Voxel::getCommonLiquid()
{
    return 100;
}
