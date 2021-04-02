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
	Assets(Assets* a);
	~Assets();

	void addTexture(const std::string& name, const Texture& texture);

	void addModel(const std::string& name, const Model& model);

	void addFont(const std::string& name, const Font& font);

	void addAudio(const std::string& name, const WAVE& a);

	std::shared_ptr<Texture> getTexture(const std::string& name);

	std::shared_ptr<Model> getModel(const std::string& name);

	std::shared_ptr<Font> getFont(const std::string& name);

	std::shared_ptr<WAVE> getAudio(const std::string& name);

	void deleteAllAssets();

	void deleteAudio();

	void deleteTextures();

	void deleteFonts();

	void deleteModels();

	static Assets* getAssets();

private:

	static Assets* assets;

	std::map<std::string, std::shared_ptr<Model>> models;

	std::map<std::string, std::shared_ptr<Texture>> textures;

	std::map<std::string, std::shared_ptr<Font>> fonts;

	std::map<std::string, std::shared_ptr<WAVE>> audio;
};

