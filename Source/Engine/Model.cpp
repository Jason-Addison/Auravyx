#ifdef __linux__ 
#elif _WIN32
#include <Windows.h>
#else
#endif
#include "Engine/Model.h"
#include "Engine/Model.h"
#include <iostream>
GLuint vao;
int count;
GLuint elementBuffer;

Model::Model()
{

}
Model::Model(GLuint vao, int count)
{
	this->vao = vao;
	this->count = count;
}
Model::Model(GLuint vao, GLuint elementBuffer, int count)
{
	this->vao = vao;
	this->count = count;
	this->elementBuffer = elementBuffer;
}

Model::~Model()
{

}

GLuint Model::getVAO()
{
	return vao;
}

int Model::getCount()
{
	return count;
}

GLuint Model::getElementBuffer()
{
	return elementBuffer;
}
void Model::setMaterials(std::vector<OBJMaterial> materials)
{
	this->materials = materials;
}

std::vector<OBJMaterial> Model::getMaterials()
{
	return materials;
}

void Model::destroy()
{
	glDeleteBuffers(attributes.size(), &attributes.front());
	glDeleteVertexArrays(1, &vao);
}

void Model::setTexture(int index, Texture texture)
{
	materials.at(index).texture = texture;
}

Model Model::load2DModel(std::vector<float> vertices)
{
	GLuint vao = generateVAO();
	Model m(vao, vertices.size());
	addVertexAttribute(0, 2, m, vertices);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return m;
}
Model Model::load2DModel(std::vector<float> vertices, std::vector<float> texCoords)
{
	GLuint vao = generateVAO();
	Model m(vao, vertices.size());

	addVertexAttribute(0, 2, m, vertices);
	addVertexAttribute(1, 2, m, texCoords);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return m;
}

Model Model::loadIndexed3DModel(std::vector<float> vertices, std::vector<float> normals, std::vector<unsigned int> indices)
{
	GLuint vao = generateVAO();
	GLuint elementBuffer = 0;
	Model m(vao, vertices.size());

	addVertexAttribute(0, 3, m, vertices);
	addVertexAttribute(1, 3, m, normals);
	elementBuffer = addIndicesBuffer(indices);

	m.indexCount = indices.size();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return m;
}
Model Model::load3DModel(std::vector<float> vertices, std::vector<float> normals)
{
	GLuint vao = generateVAO();
	Model m(vao, vertices.size());

	if (vertices.size() != 0)
	{
		addVertexAttribute(0, 3, m, vertices);
		addVertexAttribute(1, 3, m, normals);
	}
	else
	{
		std::cerr << "Model without data!\n";
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return m;
}
Model Model::load3DLineModel(std::vector<float> vertices, std::vector<float> colours)
{
	GLuint vao = generateVAO();
	Model m(vao, vertices.size());

	if (vertices.size() != 0)
	{
		addVertexAttribute(0, 3, m, vertices);
		addVertexAttribute(1, 4, m, colours);
	}
	else
	{
		std::cerr << "Model without data!\n";
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return m;
}
Model Model::load3DModelT(std::vector<float> vertices, std::vector<float> normals, std::vector<float> uv)
{
	GLuint vao = generateVAO();
	Model m(vao, vertices.size());

	if (vertices.size() != 0)
	{
		addVertexAttribute(0, 3, m, vertices);
		addVertexAttribute(1, 3, m, normals);
		addVertexAttribute(2, 2, m, uv);
	}
	else
	{
		std::cerr << "Model without data!\n";
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return m;
}
Model Model::load3DModel(std::vector<float> vertices, std::vector<float> normals, std::vector<float> materials)
{
	GLuint vao = generateVAO();
	Model m(vao, vertices.size());

	if (vertices.size() != 0)
	{
		addVertexAttribute(0, 3, m, vertices);
		addVertexAttribute(1, 3, m, normals);
		addVertexAttribute(2, 3, m, materials);
	}
	else
	{
		//std::cerr << "Model without data!\n";
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return m;
}
/*Model Model::load3DModel(std::vector<float> vertices, std::vector<float> normals, std::vector<float> tex)
{
	GLuint vao = generateVAO();

	addVertexAttribute(0, 3, vertices);
	addVertexAttribute(1, 3, normals);
	addVertexAttribute(2, 2, tex);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return Model(vao, vertices.size());
}*/
Model Model::load3DModel(std::vector<float> vertices, std::vector<float> normals, std::vector<float> tex, std::vector<float> materials)
{
	GLuint vao = generateVAO();
	Model m(vao, vertices.size());

	addVertexAttribute(0, 3, m, vertices);
	addVertexAttribute(1, 3, m, normals);
	addVertexAttribute(2, 2, m, tex);
	addVertexAttribute(3, 3, m, materials);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return m;
}
Model Model::load3DModel(std::vector<float> vertices, std::vector<float> normals, std::vector<float> posA,
	std::vector<float> posB, std::vector<float> posC, std::vector<unsigned int> materials)
{
	GLuint vao = generateVAO();
	Model m(vao, vertices.size());
	if (vertices.size() != 0)
	{
		addVertexAttribute(0, 3, m, vertices);
		addVertexAttribute(1, 3, m, normals);
		addVertexAttribute(2, 3, m, posA);
		addVertexAttribute(3, 3, m, posB);
		addVertexAttribute(4, 3, m, posC);
		addVertexAttribute(5, 3, m, materials);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return m;
}
Model Model::load3DModel(std::vector<float> vertices)
{
	GLuint vao = generateVAO();
	Model m(vao, vertices.size());

	addVertexAttribute(0, 3, m, vertices);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return m;
}

GLuint Model::generateVAO()
{
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	return vao;
}

GLuint Model::addVertexAttribute(int attrib, int size, Model& m, std::vector<float> attribs)
{
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, attribs.size() * sizeof(float), &attribs[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(attrib, size, GL_FLOAT, GL_FALSE, 0, NULL);
	m.attributes.emplace_back(vbo);
	return vbo;
}
GLuint Model::addVertexAttribute(int attrib, int size, Model& m, std::vector<unsigned int> attribs)
{
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, attribs.size() * sizeof(unsigned int), &attribs[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribIPointer(attrib, size, GL_UNSIGNED_INT, 0, NULL);
	m.attributes.emplace_back(vbo);
	return vbo;
}
GLuint Model::addIndicesBuffer(std::vector<unsigned int> indices)
{
	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	return elementBuffer;
}