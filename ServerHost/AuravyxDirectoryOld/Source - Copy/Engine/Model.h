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
	Model(const GLuint vao, const int vertexCount);
	Model(const GLuint vao, const GLuint elementBuffer, const int vertexCount);
	~Model();

	std::vector<ModelMaterial> materials;

	std::string name;

	std::vector<GLuint> attributes;

	GLuint vao = 0;
	int count; 
	int indexCount;
	GLuint getVAO();
	int getCount();

	GLuint elementBuffer;
	GLuint getElementBuffer();

	void setTexture(const int index, const Texture& texture);

	static Model load2DModel(const std::vector<float>& vertices);

	static Model load2DModel(const std::vector<float>& vertices, const std::vector<float>& texCoords);

	static Model loadIndexed3DModel(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<unsigned int>& indices);

	static Model loadIndexed3DModel(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& textures,
		const std::vector<float>& colors, const std::vector<unsigned int>& indices);

	static Model load3DModel(const std::vector<float>& vertices, const std::vector<float>& normals);

	static Model load3DLineModel(const std::vector<float>& vertices, const std::vector<float>& colours);

	static Model load3DModelT(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& uv);

	static Model load3DModel(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& materials);

	static Model load3DModel(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& posA,
		const std::vector<float>& posB, const std::vector<float>& posC, const std::vector<unsigned int>& materials);
	//static Model load3DModel(std::vector<float> vertices, std::vector<float> normals, std::vector<float> tex);

	static Model load3DModel(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& tex, const std::vector<float>& materials);

	static Model load3DModel(const std::vector<float>& vertices);

	void setMaterials(const std::vector<ModelMaterial>& materials);

	std::vector<ModelMaterial> getMaterials();

	void destroy();

private:

	static GLuint generateVAO();

	static GLuint addVertexAttribute(const int attrib, const int size, Model& m, const std::vector<float>& attribs);
	static GLuint addVertexAttribute(const int attrib, const int size, Model& m, const std::vector<unsigned int>& attribs);
	static GLuint addIndicesBuffer(const std::vector<unsigned int>& indices);
};

