#pragma once
#include "Engine/FBO.h"
#include "Utilities/Matrix4f.h"
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

