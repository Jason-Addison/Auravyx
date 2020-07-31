#pragma once
#include <map>
#include "Texture.h"
#include "Model.h"
#include <memory>
#include "Font.h"
#include "WAVE.h"
class Assets
{
public:
	Assets();
	~Assets();

	static std::map<std::string, std::shared_ptr<Model>> models;

	static std::map<std::string, std::shared_ptr<Texture>> textures;

	static std::map<std::string, std::shared_ptr<Font>> fonts;

	static std::map<std::string, std::shared_ptr<WAVE>> audio;

	static void addTexture(std::string name, Texture texture);

	static void addModel(std::string name, Model model);

	static void addFont(std::string name, Font font);

	static void addAudio(std::string name, WAVE a);

	static std::shared_ptr<Texture> getTexture(std::string name);

	static std::shared_ptr<Model> getModel(std::string name);

	static std::shared_ptr<Font> getFont(std::string name);

	static std::shared_ptr<WAVE> getAudio(std::string name);

	static void deleteAllAssets();

	static void deleteAudio();

	static void deleteTextures();

	static void deleteFonts();

	static void deleteModels();
};

