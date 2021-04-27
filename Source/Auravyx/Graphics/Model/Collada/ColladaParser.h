#pragma once
#include "Auravyx/Utility/XML/XMLParser.h"
#include <string>
#include <vector>
#include "AnimatedMesh.h"
#include <map>
#include <Auravyx/Utility/Math/Vec4f.h>

/**
 * @brief Parser that reads Collada (.dae) files for animated and non animated meshes.
*/
class ColladaParser
{
public:

	/**
	 * @todo Add proper parse function, remove this temp function.
	*/
	static AnimatedMesh parse(const std::string& dir);

private:

	struct VertexAttribute
	{
		std::string source;
		std::vector<int> indices;
		std::vector<float> processed;
		int offset;
		int stride;
	};

	struct ColladaSource
	{
		std::string name;
		std::vector<float> data;
		int stride;
	};

	/**
	 * @brief Returns list of textures.
	 * @param doc XML document
	 * @return list of textures
	*/
	static std::map<std::string, std::string> loadTextures(XMLParser::XMLNode* doc);

	/**
	 * @brief Returns list of texture links.
	 * @param doc XML document
	 * @param texture list of textures
	 * @return list of textures
	*/
	static std::map<std::string, GLuint> getTextureLinks(XMLParser::XMLNode* doc,
		std::map<std::string, std::string>& texture);

	/**
	 * @brief Returns 'AnimatedMesh' containing geometric data.
	 * @param doc XML document
	 * @return mesh
	*/
	static AnimatedMesh loadGeometry(XMLParser::XMLNode* doc);

	/**
	 * @brief Returns 'AnimatedMesh' containing all of the model's geometric data and linked textures.
	 * @param doc XML document
	 * @param texLinks list of texture links
	 * @return mesh
	*/
	static AnimatedMesh loadAllGeometries(XMLParser::XMLElement* doc,
		std::map<std::string, GLuint>& texLinks);

	/**
	 * @brief Get list of floats from string
	 * @param str string containing floats
	 * @return list of floats
	*/
	static std::vector<float> stringToFloats(const std::string& str);

	/**
	 * @brief Get list of integers from string
	 * @param str string containing integers
	 * @return list of integers
	*/
	static std::vector<GLuint> stringToIntegers(const std::string& str);
};