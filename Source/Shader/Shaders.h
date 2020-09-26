#pragma once
#include <map>
#include "Shader/ImageShader.h"
#include "Shader/RectShader.h"
#include "Graphics/FontShader.h"
#include "World/VoxelShader.h"
#include "Shader/DeferredShader.h"
#include "Shader/ShadowShader.h"
#include "Shader/SkyShader.h"
#include "Shader/LineShader.h"
#include "Shader/ModelShader.h"
class Shaders
{
public:

	ImageShader* imageShader = new ImageShader();
	RectShader* rectShader = new RectShader();
	FontShader* fontShader = new FontShader();
	VoxelShader* voxelShader = new VoxelShader();
	ShadowShader* shadowShader = new ShadowShader();
	DeferredShader* deferredShader = new DeferredShader();
	SkyShader* skyShader = new SkyShader();
	LineShader* lineShader = new LineShader();
	ModelShader* modelShader = new ModelShader();
	Shaders();
	~Shaders();

	void init(std::map<std::string, std::string> shaders);

	void initBootShaders(std::map<std::string, std::string> shaders);
};

