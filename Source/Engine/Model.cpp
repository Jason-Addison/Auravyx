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
Model::Model(const GLuint vao, const int count)
{
	this->vao = vao;
	this->count = count;
	this->indexCount = 0;
}
Model::Model(const GLuint vao, const GLuint elementBuffer, const int count)
{
	this->vao = vao;
	this->count = count;
	this->elementBuffer = elementBuffer;
	this->indexCount = 0;
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
void Model::setMaterials(const std::vector<ModelMaterial>& materials)
{
	this->materials = materials;
}

std::vector<ModelMaterial> Model::getMaterials()
{
	return materials;
}

void Model::destroy()
{
	if (attributes.size() > 0)
	{
		glDeleteBuffers(attributes.size(), &attributes.front());
		glDeleteVertexArrays(1, &vao);
	}
}

void Model::setTexture(const int index, const Texture& texture)
{
	/////////////////materials.at(index).texture = texture;
}

Model Model::load2DModel(const std::vector<float>& vertices)
{
	GLuint vao = generateVAO();
	Model m(vao, vertices.size());
	addVertexAttribute(0, 2, m, vertices);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return m;
}
Model Model::load2DModel(const std::vector<float>& vertices, const std::vector<float>& texCoords)
{
	GLuint vao = generateVAO();
	Model m(vao, vertices.size());

	addVertexAttribute(0, 2, m, vertices);
	addVertexAttribute(1, 2, m, texCoords);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return m;
}

Model Model::loadIndexed3DModel(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<unsigned int>& indices)
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
Model Model::loadIndexed3DModel(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& textures,
	const std::vector<float>& colors, const std::vector<unsigned int>& indices)
{
	GLuint vao = generateVAO();
	GLuint elementBuffer = 0;
	Model m(vao, vertices.size());

	addVertexAttribute(0, 3, m, vertices);
	addVertexAttribute(1, 3, m, normals);
	addVertexAttribute(2, 2, m, textures);
	addVertexAttribute(3, 3, m, colors);
	elementBuffer = addIndicesBuffer(indices);

	m.indexCount = indices.size();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return m;
}
Model Model::load3DModel(const std::vector<float>& vertices, const std::vector<float>& normals)
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
Model Model::load3DLineModel(const std::vector<float>& vertices, const std::vector<float>& colours)
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
Model Model::load3DModelT(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& uv)
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
Model Model::load3DModel(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& materials)
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
Model Model::load3DModel(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& tex, const std::vector<float>& materials)
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
Model Model::load3DModel(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& posA,
	const std::vector<float>& posB, const std::vector<float>& posC, const std::vector<unsigned int>& materials)
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
Model Model::load3DModel(const std::vector<float>& vertices)
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

GLuint Model::addVertexAttribute(const int attrib, const int size, Model& m, const std::vector<float>& attribs)
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
GLuint Model::addVertexAttribute(const int attrib, const int size, Model& m, const std::vector<unsigned int>& attribs)
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
GLuint Model::addIndicesBuffer(const std::vector<unsigned int>& indices)
{
	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	return elementBuffer;
}