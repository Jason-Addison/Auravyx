#include "LineShader.h"
#include "Assets.h"
#include <GFX.h>
#include "Chunk.h"


void addColour(std::vector<float> &colours, float r, float g, float b, float a)
{
	colours.emplace_back(r);
	colours.emplace_back(g);
	colours.emplace_back(b);
	colours.emplace_back(a);
}

void addVertex(std::vector<float> &vertices, std::vector<float> &colours, int x, int y, int z, int axis)
{
	vertices.emplace_back(x);
	vertices.emplace_back(y);
	vertices.emplace_back(z);

	int val = x;
	if (axis == 1)
	{
		val = y;
	}
	else if(axis == 2)
	{
		val = z;
	}

	if (val % 128 == 0)
	{
		addColour(colours, 1, 0, 0, 1);
	}
	else if(val % 64 == 0)
	{
		addColour(colours, 1, 0.64, 0, 1);
	}
	else if (val % 32 == 0)
	{
		addColour(colours, 1, 1, 0, 1);
	}
	else if (val % 16 == 0)
	{
		addColour(colours, 1, 1, 0, 1);
	}
	else if (val % 8 == 0)
	{
		addColour(colours, 0.5, 0.5, 1, 1);
	}
	else
	{
		addColour(colours, 0, 0, 1, 1);
	}
	
}

LineShader::LineShader()
{
	shader = "3DLine";
}


LineShader::~LineShader()
{
}

void LineShader::loadProjectionMatrix(Matrix4f matrix)
{
	loadMatrix4f(projectionMatrix, matrix);
}

void LineShader::loadOffset(float x, float y, float z)
{
	loadVec3f(offset, x, y, z);
}

void LineShader::loadCamera(Matrix4f matrix)
{
	loadMatrix4f(viewMatrix, matrix);
}

void LineShader::render(FBO fbo)
{
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(model.getVAO());
	glEnableVertexArrayAttrib(model.getVAO(), 0);
	glEnableVertexArrayAttrib(model.getVAO(), 1);
	glDrawArrays(GL_LINES, 0, model.getCount());
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}


void addSegments(std::vector<float>& vertices, std::vector<float>& colours, float r, float g, float b, float a, int segments)
{
	for (int i = 0; i < Chunk::CHUNK_SIZE / segments + 1; i++)
	{
		addVertex(vertices, colours, 0 * Chunk::CHUNK_SIZE, i * segments, 0 * Chunk::CHUNK_SIZE, 1);
		addVertex(vertices, colours, 0 * Chunk::CHUNK_SIZE, i * segments, 1 * Chunk::CHUNK_SIZE, 1);

		addVertex(vertices, colours, 0 * Chunk::CHUNK_SIZE, i * segments, 0 * Chunk::CHUNK_SIZE, 1);
		addVertex(vertices, colours, 1 * Chunk::CHUNK_SIZE, i * segments, 0 * Chunk::CHUNK_SIZE, 1);

		addVertex(vertices, colours, Chunk::CHUNK_SIZE, i * segments, 0 * Chunk::CHUNK_SIZE, 1);
		addVertex(vertices, colours, Chunk::CHUNK_SIZE, i * segments, 1 * Chunk::CHUNK_SIZE, 1);

		addVertex(vertices, colours, Chunk::CHUNK_SIZE, i * segments, Chunk::CHUNK_SIZE, 1);
		addVertex(vertices, colours, 0, i * segments, Chunk::CHUNK_SIZE, 1);

		addVertex(vertices, colours, i * segments, 0 * Chunk::CHUNK_SIZE, 0 * Chunk::CHUNK_SIZE, 0);
		addVertex(vertices, colours, i * segments, 0 * Chunk::CHUNK_SIZE, 1 * Chunk::CHUNK_SIZE, 0);

		addVertex(vertices, colours, i * segments, 0 * Chunk::CHUNK_SIZE, 0 * Chunk::CHUNK_SIZE, 0);
		addVertex(vertices, colours, i * segments, 1 * Chunk::CHUNK_SIZE, 0 * Chunk::CHUNK_SIZE, 0);

		addVertex(vertices, colours, i * segments, 0 * Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, 0);
		addVertex(vertices, colours, i * segments, 1 * Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, 0);

		addVertex(vertices, colours, i * segments, 1 * Chunk::CHUNK_SIZE, 0, 0);
		addVertex(vertices, colours, i * segments, 1 * Chunk::CHUNK_SIZE, Chunk::CHUNK_SIZE, 0);

		addVertex(vertices, colours, Chunk::CHUNK_SIZE, 0 * Chunk::CHUNK_SIZE, i * segments, 2);
		addVertex(vertices, colours, Chunk::CHUNK_SIZE, 1 * Chunk::CHUNK_SIZE, i * segments, 2);

		addVertex(vertices, colours, 0, Chunk::CHUNK_SIZE, i * segments, 2);
		addVertex(vertices, colours, Chunk::CHUNK_SIZE, 1 * Chunk::CHUNK_SIZE, i * segments, 2);

		addVertex(vertices, colours, 0 * Chunk::CHUNK_SIZE, 0 * Chunk::CHUNK_SIZE, i * segments, 2);
		addVertex(vertices, colours, 1 * Chunk::CHUNK_SIZE, 0 * Chunk::CHUNK_SIZE, i * segments, 2);

		addVertex(vertices, colours, 0 * Chunk::CHUNK_SIZE, 0 * Chunk::CHUNK_SIZE, i * segments, 2);
		addVertex(vertices, colours, 0 * Chunk::CHUNK_SIZE, 1 * Chunk::CHUNK_SIZE, i * segments, 2);



	}
}

void LineShader::init()
{
	std::vector<float> vertices;

	std::vector<float> colours;

	float segments = 128;

	addSegments(vertices, colours, 1, 0, 0, 1, 2);

	model = Model::load3DLineModel(vertices, colours);
}

void LineShader::loadAllUniformLocations()
{
	projectionMatrix = getUniformLocation("projectionMatrix");
	offset = getUniformLocation("offset");
	viewMatrix = getUniformLocation("viewMatrix");
}
