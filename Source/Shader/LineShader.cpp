#include "Shader/LineShader.h"
#include "World/Chunk.h"
#include <Graphics/GFX.h>
#include <Auravyx.h>

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

void LineShader::loadProjectionMatrix(const Matrix4f&  matrix)
{
	loadMatrix4f(projectionMatrix, matrix);
}

void LineShader::loadOffset(const float x, const float y, const float z)
{
	loadVec3f(offset, x, y, z);
}

void LineShader::loadCamera(const Matrix4f& matrix)
{
	loadMatrix4f(viewMatrix, matrix);
}

void LineShader::render()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(model.getVAO());
	glEnableVertexArrayAttrib(model.getVAO(), 0);
	glEnableVertexArrayAttrib(model.getVAO(), 1);
	glLineWidth(2);
	glDrawArrays(GL_LINES, 0, model.getCount());
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void LineShader::renderPoint(const float x, const float y, const float z, const Matrix4f& projectionMatrix)
{
	start();
	loadCamera(GFX::getOverlay()->CAM.getViewMatrix());
	loadProjectionMatrix(projectionMatrix);
	loadOffset(x / 128, y / 128, z / 128);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(point.getVAO());
	glEnableVertexArrayAttrib(point.getVAO(), 0);
	glEnableVertexArrayAttrib(point.getVAO(), 1);
	glPointSize(20);
	glDrawArrays(GL_POINTS, 0, point.getCount());
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void addSegments(std::vector<float>& vertices, std::vector<float>& colours, const float r, const float g, const float b, const float a, const int segments)
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

	std::vector<float> pVertices;
	std::vector<float> pColours;

	pVertices.emplace_back(0);
	pVertices.emplace_back(0);
	pVertices.emplace_back(0);

	pVertices.emplace_back(0);
	pVertices.emplace_back(0.0001);
	pVertices.emplace_back(0);

	pColours.emplace_back(1);
	pColours.emplace_back(0);
	pColours.emplace_back(0);
	pColours.emplace_back(0);

	pColours.emplace_back(1);
	pColours.emplace_back(0);
	pColours.emplace_back(0);
	pColours.emplace_back(0);

	point = Model::load3DLineModel(pVertices, pColours);
}

void LineShader::loadAllUniformLocations()
{
	projectionMatrix = getUniformLocation("projectionMatrix");
	offset = getUniformLocation("offset");
	viewMatrix = getUniformLocation("viewMatrix");
}
