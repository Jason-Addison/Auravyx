#pragma once
#include <Library\GL\glew.h>
#include <vector>
#include <string>
#include "Graphics/Model/ModelMaterial.h"
class AnimatedMesh
{
public:

	std::vector<GLuint> indices;
	std::vector<float> vertices;
	std::vector<float> textureCoords;
	std::vector<float> normals;
	std::vector<float> colors;

	std::string id;

	std::vector<ModelMaterial> materials;

	float r;
	float g;
	float b;
	float a;
};

