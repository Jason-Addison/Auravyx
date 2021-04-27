#include "Shaders.h"

Shaders::Shaders()
{
}


Shaders::~Shaders()
{
}

void Shaders::init(const std::map<std::string, std::string>& shaders)
{
	voxelShader->createShader(shaders);
	shadowShader->createShader(shaders);
	deferredShader->createShader(shaders);
	skyShader->createShader(shaders);
	lineShader->createShader(shaders);
	modelShader->createShader(shaders);
}

void Shaders::initBootShaders(const std::map<std::string, std::string>& shaders)
{
	imageShader->createShader(shaders);
	rectShader->createShader(shaders);
	fontShader->createShader(shaders);
}
