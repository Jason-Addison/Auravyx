#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::Renderer(Renderer* r)
{
	renderer = r;
}

Renderer* Renderer::renderer;

Shaders* Renderer::getShaders()
{
	return &shaders;
}

Renderer* Renderer::getRenderer()
{
	return renderer;
}

std::vector<std::pair<std::function<void()>, GLContext*>>* Renderer::getRenderFunctions()
{
	return &renderFunctions;
}

void Renderer::addRenderFunction(void(*f)(), GLContext *c)
{
	std::function<void()> func = f;
	std::pair<std::function<void()>, GLContext*> pair;
	pair.first = f;
	pair.second = c;
	renderFunctions.emplace_back(pair);
}
