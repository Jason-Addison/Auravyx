#pragma once
#include "XMLParser.h"
#include <string>
#include <vector>
#include "AnimatedMesh.h"
#include <map>
#include <Utilities\Vec4f.h>

class ColladaParser
{
public:

	static void parse(const std::string& dir);

private:

	static std::map<std::string, std::string> loadTextures(XMLParser::XMLNode* doc);

	static std::map<std::string, GLuint> getTextureLinks(XMLParser::XMLNode* doc,
		std::map<std::string, std::string>& texture);

	static AnimatedMesh loadGeometry(XMLParser::XMLNode* doc);

	static AnimatedMesh loadAllGeometries(XMLParser::XMLElement* doc,
		std::map<std::string, GLuint>& texLinks);

	static std::vector<float> stringToFloats(const std::string& str);

	static std::vector<GLuint> stringToIntegers(const std::string& str);

	static void processVertex(int v, int n, int t, int c, std::vector<GLuint>& indices,
		std::vector<float>& textures, std::vector<float>& normals, 
		std::vector<float>& colors,	std::vector<float>& processedTextures, 
		std::vector<float>& processedNormals, std::vector<float>& processedColors);
};

