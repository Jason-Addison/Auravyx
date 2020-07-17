#include "stdafx.h"
#include "Assets.h"
#include "Model.h"
#include <memory>
#include "Util.h"
#include "Log.h"
#include <stdexcept>
#include "Font.h"

Assets::Assets()
{
}


Assets::~Assets()
{
}
std::map<std::string, std::shared_ptr<Model>> Assets::models = std::map<std::string, std::shared_ptr<Model>>();
std::map<std::string, std::shared_ptr<Texture>> Assets::textures = std::map<std::string, std::shared_ptr<Texture>>();
std::map<std::string, std::shared_ptr<Font>> Assets::fonts = std::map<std::string, std::shared_ptr<Font>>();
std::map<std::string, std::shared_ptr<WAVE>> Assets::audio = std::map<std::string, std::shared_ptr<WAVE>>();

void Assets::addTexture(std::string name, Texture texture)
{
	textures.emplace(name, std::shared_ptr<Texture>(new Texture(texture)));
}

void Assets::addModel(std::string name, Model model)
{
	std::shared_ptr<Model> mod(new Model(model));
	mod->name = name;
	models.emplace(name, mod);
}

void Assets::addFont(std::string name, Font font)
{
	fonts.emplace(name, std::shared_ptr<Font>(new Font(font)));
}

void Assets::addAudio(std::string name, WAVE a)
{
	audio.emplace(name, std::shared_ptr<WAVE>(new WAVE(a)));
}

std::shared_ptr<Texture> Assets::getTexture(std::string name)
{
	try
	{
		return textures.at(name);
	}
	catch (std::out_of_range e)
	{
		Log::out("Asset Fetch", "Texture '" + name + "' was not found!", RED, RED);
	}
	return NULL;
}

std::shared_ptr<Font> Assets::getFont(std::string name)
{
	try
	{
		return fonts.at(name);
	}
	catch (std::out_of_range e)
	{
		Log::out("Asset Fetch", "Font '" + name + "' was not found!", RED, RED);
	}
	return NULL;
}

std::shared_ptr<WAVE> Assets::getAudio(std::string name)
{
	try
	{
		return audio.at(name);
	}
	catch (std::out_of_range e)
	{
		Log::out("Asset Fetch", "Audio '" + name + "' was not found!", RED, RED);
	}
	return NULL;
}

std::shared_ptr<Model> Assets::getModel(std::string name)
{
	try
	{
		return models.at(name);
	}
	catch (std::out_of_range e)
	{
		Log::out("Asset Fetch", "Model '" + name + "' was not found!", RED, RED);
	}
	return NULL;
}
