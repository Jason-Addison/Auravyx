#pragma once
#include <map>
#include "Engine/Texture.h"
#include "Engine/Model.h"
#include <memory>
#include "Graphics/Font.h"
#include "Audio/WAVE.h"
class Assets
{
public:
	Assets();
	~Assets();

	std::map<std::string, std::shared_ptr<Model>> models;

	std::map<std::string, std::shared_ptr<Texture>> textures;

	std::map<std::string, std::shared_ptr<Font>> fonts;

	std::map<std::string, std::shared_ptr<WAVE>> audio;

	void addTexture(std::string name, Texture texture);

	void addModel(std::string name, Model model);

	void addFont(std::string name, Font font);

	void addAudio(std::string name, WAVE a);

	std::shared_ptr<Texture> getTexture(std::string name);

	std::shared_ptr<Model> getModel(std::string name);

	std::shared_ptr<Font> getFont(std::string name);

	std::shared_ptr<WAVE> getAudio(std::string name);

	void deleteAllAssets();

	void deleteAudio();

	void deleteTextures();

	void deleteFonts();

	void deleteModels();
};

