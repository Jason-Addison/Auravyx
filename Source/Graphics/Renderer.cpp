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

std::vector<std::function<void()>>* Renderer::getRenderFunctions()
{
	return &renderFunctions;
}

void Renderer::addRenderFunction(void(*f)())
{
	std::function<void()> func = f;
	renderFunctions.emplace_back(func);
}
