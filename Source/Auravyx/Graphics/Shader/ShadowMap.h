#pragma once
#include "Auravyx/Graphics/FBO.h"
#include "Auravyx/Utility/Math/Matrix4f.h"
class ShadowMap : public FBO
{
public:

	int width = 1024 * 8;
	int height = 1024 * 8;

	Matrix4f depthMVP;

	ShadowMap();
	ShadowMap(const int width, const int height);
	~ShadowMap();

	void create();
	void bind();
};

