#pragma once
#include "FBO.h"
#include "Matrix4f.h"
class ShadowMap : public FBO
{
public:

	int width = 1024 * 2;
	int height = 1024 * 2;

	Matrix4f depthMVP;

	ShadowMap();
	ShadowMap(int width, int height);
	~ShadowMap();

	void create();
	void bind();
};

