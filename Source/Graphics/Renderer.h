#pragma once
#include "Shader/Shaders.h"
#include <vector>
#include <functional>
class Renderer
{
public:

	Renderer();

	Renderer(Renderer* r);

	Shaders shaders;

	Shaders* getShaders();

	std::vector<std::function<void()>> renderFunctions;

	static Renderer* getRenderer();

	std::vector<std::function<void()>>* getRenderFunctions();

	void addRenderFunction(void (*f)());

private:

	static Renderer* renderer;
};

