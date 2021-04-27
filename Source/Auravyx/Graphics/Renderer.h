#pragma once
#include "Auravyx/Graphics/Shader/Shaders.h"
#include <vector>
#include <functional>
#include "Auravyx/Graphics/GL/GLContext.h"
class Renderer
{
public:

	Renderer();

	Renderer(Renderer* r);

	Shaders shaders;

	Shaders* getShaders();

	std::vector<std::pair<std::function<void()>, GLContext*>> renderFunctions;

	static Renderer* getRenderer();

	std::vector<std::pair<std::function<void()>, GLContext*>>* getRenderFunctions();

	void addRenderFunction(void (*f)(), GLContext* c);

private:

	static Renderer* renderer;
};

