#pragma once
#include <Library\GL\glew.h>
#include "Library\GLFW\glfw3.h"
#include <vector>
#include "Engine/OBJMaterial.h"
#include "Graphics/Model/ModelMaterial.h"
class Model
{
public:

	Model();
	Model(GLuint vao, int vertexCount);
	Model(GLuint vao, GLuint elementBuffer, int vertexCount);
	~Model();

	std::vector<ModelMaterial> materials;

	std::string name;

	std::vector<GLuint> attributes;

	GLuint vao;
	int count; 
	int indexCount;
	GLuint getVAO();
	int getCount();

	GLuint elementBuffer;
	GLuint getElementBuffer();

	void setTexture(int index, Texture texture);

	static Model load2DModel(std::vector<float> vertices);

	static Model load2DModel(std::vector<float> vertices, std::vector<float> texCoords);

	static Model loadIndexed3DModel(std::vector<float> vertices, std::vector<float> normals, std::vector<unsigned int> indices);

	static Model loadIndexed3DModel(std::vector<float> vertices, std::vector<float> normals, std::vector<float> textures, 
		std::vector<float> colors, std::vector<unsigned int> indices);

	static Model load3DModel(std::vector<float> vertices, std::vector<float> normals);

	static Model load3DLineModel(std::vector<float> vertices, std::vector<float> colours);

	static Model load3DModelT(std::vector<float> vertices, std::vector<float> normals, std::vector<float> uv);

	static Model load3DModel(std::vector<float> vertices, std::vector<float> normals, std::vector<float> materials);

	static Model load3DModel(std::vector<float> vertices, std::vector<float> normals, std::vector<float> posA,
		std::vector<float> posB, std::vector<float> posC, std::vector<unsigned int> materials);
	//static Model load3DModel(std::vector<float> vertices, std::vector<float> normals, std::vector<float> tex);

	static Model load3DModel(std::vector<float> vertices, std::vector<float> normals, std::vector<float> tex, std::vector<float> materials);

	static Model load3DModel(std::vector<float> vertices);

	void setMaterials(std::vector<ModelMaterial> materials);

	std::vector<ModelMaterial> getMaterials();

	void destroy();

private:

	static GLuint generateVAO();

	static GLuint addVertexAttribute(int attrib, int size, Model& m, std::vector<float> attribs);
	static GLuint addVertexAttribute(int attrib, int size, Model& m, std::vector<unsigned int> attribs);
	static GLuint addIndicesBuffer(std::vector<unsigned int> indices);
};

