#pragma once
#include <map>
#include "Auravyx/Graphics/Shader/ImageShader.h"
#include "Auravyx/Graphics/Shader/RectShader.h"
#include "Auravyx/Graphics/Shader/FontShader.h"
#include "Auravyx/Graphics/Shader/VoxelShader.h"
#include "Auravyx/Graphics/Shader/DeferredShader.h"
#include "Auravyx/Graphics/Shader/ShadowShader.h"
#include "Auravyx/Graphics/Shader/SkyShader.h"
#include "Auravyx/Graphics/Shader/LineShader.h"
#include "Auravyx/Graphics/Shader/ModelShader.h"
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

	void init(const std::map<std::string, std::string>& shaders);

	void initBootShaders(const std::map<std::string, std::string>& shaders);
};

