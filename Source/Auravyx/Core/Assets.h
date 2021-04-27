#pragma once
#include <map>
#include "Auravyx/Graphics/Texture.h"
#include "Auravyx/Graphics/Model/Model.h"
#include <memory>
#include "Auravyx/Graphics/Font/Font.h"
#include "Auravyx/Audio/WAVE.h"

/**
 * @brief Helper class to manage, store, retrieve and delete assets.
*/
class Assets
{
public:

	Assets() = default;

	/**
	 * @brief Sets current Assets instance.
	 * @param a instance to set
	*/
	Assets(Assets* a);

	~Assets() = default;

	/**
	 * @brief Add texture to texture storage.
	 * @param name texture name
	 * @param texture texture value
	*/
	void addTexture(const std::string& name, const Texture& texture);

	/**
	 * @brief Add model to model storage.
	 * @param name model name
	 * @param texture model value
	*/
	void addModel(const std::string& name, const Model& model);

	/**
	 * @brief Add font to font storage.
	 * @param name font name
	 * @param texture font value
	*/
	void addFont(const std::string& name, const Font& font);

	/**
	 * @brief Add WAVE to audio storage.
	 * @param name WAVE name
	 * @param texture WAVE audio value
	*/
	void addAudio(const std::string& name, const WAVE& a);

	/**
	 * @brief Finds texture by name and returns it, if found.
	 * @param name texture name
	 * @return shared_ptr to texture
	 * @throws std::out_of_range if 'name' is not found
	*/
	std::shared_ptr<Texture> getTexture(const std::string& name);

	/**
	 * @brief Finds model by name and returns it, if found.
	 * @param name model name
	 * @return shared_ptr to model
	 * @throws std::out_of_range if 'name' is not found
	*/
	std::shared_ptr<Model> getModel(const std::string& name);

	/**
	 * @brief Finds font by name and returns it, if found.
	 * @param name font name
	 * @return shared_ptr to font
	 * @throws std::out_of_range if 'name' is not found
	*/
	std::shared_ptr<Font> getFont(const std::string& name);

	/**
	 * @brief Finds audio by name and returns it, if found.
	 * @param name audio name
	 * @return shared_ptr to audio
	 * @throws std::out_of_range if 'name' is not found
	*/
	std::shared_ptr<WAVE> getAudio(const std::string& name);

	/**
	 * @brief Deletes all loaded audio.
	*/
	void deleteAudio();

	/**
	 * @brief Deletes all loaded textures.
	 * @warning Not thread safe!
	*/
	void deleteTextures();

	/**
	 * @brief Deletes all loaded fonts.
	 * @warning Not thread safe!
	*/
	void deleteFonts();

	/**
	 * @brief Deletes all loaded models.
	 * @warning Not thread safe!
	*/
	void deleteModels();

	/**
	 * @brief Returns the current instance's assets (for mod support)
	 * @return current 'Assets' instance
	*/
	static Assets* getAssets();

private:

	/**
	 * @brief Current Assets instance (for mod support)
	*/
	static Assets* assets;

	std::map<std::string, std::shared_ptr<Model>> models;

	std::map<std::string, std::shared_ptr<Texture>> textures;

	std::map<std::string, std::shared_ptr<Font>> fonts;

	std::map<std::string, std::shared_ptr<WAVE>> audio;
};

