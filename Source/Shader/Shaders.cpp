#include "stdafx.h"
#include "Shaders.h"

Shaders::Shaders()
{
}


Shaders::~Shaders()
{
}

ImageShader* Shaders::imageShader = new ImageShader();
RectShader* Shaders::rectShader = new RectShader();
FontShader* Shaders::fontShader = new FontShader();
VoxelShader* Shaders::voxelShader = new VoxelShader();
ShadowShader* Shaders::shadowShader = new ShadowShader();
DeferredShader* Shaders::deferredShader = new DeferredShader();
SkyShader* Shaders::skyShader = new SkyShader();
LineShader* Shaders::lineShader = new LineShader();
void Shaders::init(std::map<std::string, std::string> shaders)
{
	voxelShader->createShader(shaders);
	shadowShader->createShader(shaders);
	deferredShader->createShader(shaders);
	skyShader->createShader(shaders);
	lineShader->createShader(shaders);
}

void Shaders::initBootShaders(std::map<std::string, std::string> shaders)
{
	imageShader->createShader(shaders);
	rectShader->createShader(shaders);
	fontShader->createShader(shaders);
}
