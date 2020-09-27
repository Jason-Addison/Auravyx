#pragma once
#include "Shader/Shaders.h"
class Renderer
{
public:

	Renderer();

	Renderer(Renderer* r);

	Shaders shaders;

	Shaders* getShaders();

	static Renderer* getRenderer();

private:

	static Renderer* renderer;
};

