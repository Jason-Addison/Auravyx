#pragma once
#include <map>
#include "ImageShader.h"
#include "RectShader.h"
#include "FontShader.h"
#include "VoxelShader.h"
#include "DeferredShader.h"
#include "ShadowShader.h"
#include "SkyShader.h"
#include "LineShader.h"
class Shaders
{
public:

	static ImageShader* imageShader;
	static RectShader* rectShader;
	static FontShader* fontShader;
	static VoxelShader* voxelShader;
	static ShadowShader* shadowShader;
	static DeferredShader* deferredShader;
	static SkyShader* skyShader;
	static LineShader* lineShader;
	Shaders();
	~Shaders();

	void init(std::map<std::string, std::string> shaders);

	void initBootShaders(std::map<std::string, std::string> shaders);
};

