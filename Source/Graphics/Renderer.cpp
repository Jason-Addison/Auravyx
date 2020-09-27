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
