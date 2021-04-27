#include "ColladaParser.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Utilities/Assets.h"
#include "Engine/Model.h"
#include <Utilities\Log.h>

#define ALBEDO 0
#define NORMAL 1
#define SPECULAR 2
#define AMBIENT_OCCLUSION 3
#define DETAIL 4

std::vector<float> ColladaParser::stringToFloats(const std::string& str)
{
    std::vector<float> v;

    std::istringstream iss(str);

    std::copy(std::istream_iterator<float>(iss),
        std::istream_iterator<float>(),
        std::back_inserter(v));
    return v;
}

std::vector<GLuint> ColladaParser::stringToIntegers(const std::string& str)
{
    std::vector<GLuint> v;

    std::istringstream iss(str);

    std::copy(std::istream_iterator<int>(iss),
        std::istream_iterator<int>(),
        std::back_inserter(v));
    return v;
}

std::map<std::string, std::string> ColladaParser::loadTextures(XMLParser::XMLNode* doc)
{
    std::map<std::string, std::string> textures;
    for (XMLParser::XMLElement* current = doc->FirstChildElement("library_images")->
        FirstChildElement("image"); current != NULL; current = (XMLParser::XMLElement*) current->NextSibling())
    {
        std::string s = std::string(current->FirstChildElement("init_from")->GetText());
        s = s.substr(0, s.size() - 4);
        textures.emplace(std::string(current->Attribute("name")), s);
    }
    for (std::map<std::string, std::string>::const_iterator it = textures.begin();
        it != textures.end(); ++it)
    {
        Log::out("Texture: " + it->first + " " + it->second);
    }
    return textures;
}
std::map<std::string, GLuint> ColladaParser::getTextureLinks(XMLParser::XMLNode* doc, std::map<std::string, std::string>& textures)
{
    std::map<std::string, GLuint> links;
    for (XMLParser::XMLElement* current = doc->FirstChildElement("library_effects")->
        FirstChildElement("effect"); current != NULL; current = (XMLParser::XMLElement*) current->NextSibling())
    {
        XMLParser::XMLElement* l = current->FirstChildElement("profile_COMMON")->FirstChildElement("technique");
       
        XMLParser::XMLElement* source = l->FirstChildElement("phong");
        if (!source)
        {
            source = l->FirstChildElement("lambert");
        }

        XMLParser::XMLElement* albedo = source->FirstChildElement("diffuse")->FirstChildElement("texture");
        XMLParser::XMLElement* extra = l->FirstChildElement("extra");
        XMLParser::XMLElement* diffuse = source->FirstChildElement("diffuse")->FirstChildElement("color");

        if (albedo)
        {
            std::string s(albedo->Attribute("texture"));
            s = s.substr(0, s.size() - 8);
            s = textures.at(s);
            if (Assets::getAssets()->getTexture(s))
            {
                links.emplace(std::string(albedo->Attribute("texcoord")), Assets::getAssets()->getTexture(s)->texture);
            }
        }
        if (extra)
        {
            XMLParser::XMLElement* bump = extra->FirstChildElement("technique")->FirstChildElement("bump")->FirstChildElement("texture");
            std::string s(bump->Attribute("texture"));
            s = s.substr(0, s.size() - 8);
            s = textures.at(s);
            if (Assets::getAssets()->getTexture(s))
            {
                //links.emplace(std::string(bump->Attribute("texcoord")), Assets::getAssets()->getTexture(s)->texture);
            }
        }
    }
    for (std::map<std::string,GLuint>::const_iterator it = links.begin();
        it != links.end(); ++it)
    {
        Log::out(it->first + " " + std::to_string(it->second));
    }
    return links;
}

AnimatedMesh ColladaParser::loadGeometry(XMLParser::XMLNode* g)
{
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<float> colors;
    XMLParser::XMLNode* sources = g;
    sources = sources->FirstChildElement("mesh")->FirstChildElement("source");
    std::map<std::string, ColladaSource> colsrc;
    do
    {
        ColladaSource src;
        src.data = stringToFloats(sources->FirstChildElement("float_array")->GetText());
        src.stride = sources->FirstChildElement("technique_common")->FirstChildElement("accessor")->IntAttribute("stride");
        std::string name = sources->FirstChildElement("technique_common")->FirstChildElement("accessor")->Attribute("source");
        colsrc.emplace(name.substr(0, name.length() - std::string("-array").length()), src);
    }
    while ((sources = sources->NextSiblingElement("source")));

    XMLParser::XMLElement* vNode = g->FirstChildElement("mesh")->FirstChildElement("vertices")->FirstChildElement("input");

    std::map<std::string, VertexAttribute> attributes;

    XMLParser::XMLNode* pNode = g->FirstChildElement("mesh")->FirstChildElement("polylist");

    if (pNode == NULL)
    {
        pNode = g->FirstChildElement("mesh")->FirstChildElement("triangles");
    }

    XMLParser::XMLElement* nextPNode = pNode->FirstChildElement("input");
    do
    {
        if (nextPNode)
        {
            VertexAttribute va;
            va.offset = nextPNode->IntAttribute("offset");
            if (std::string(nextPNode->Attribute("semantic")) == std::string("VERTEX"))
            {
                va.stride = 3;
                va.source = vNode->Attribute("source");
                attributes.emplace(nextPNode->Attribute("semantic"), va);
            }
            else
            {
                va.source = nextPNode->Attribute("source");
                va.stride = colsrc.at(nextPNode->Attribute("source")).stride;
                attributes.emplace(nextPNode->Attribute("semantic"), va);
            }
        }
    }
    while ((nextPNode = nextPNode->NextSiblingElement("input")));

    std::string polylistStr = pNode->FirstChildElement("p")->GetText();
    std::vector<GLuint> polylist = stringToIntegers(polylistStr);

    std::vector<GLuint> indices;

    std::map<std::string, VertexAttribute>::iterator it;
    for (it = attributes.begin(); it != attributes.end(); it++)
    {
        for (int i = 0; i < polylist.size() / attributes.size(); i++)
        {
            it->second.indices.emplace_back(polylist.at(i * attributes.size() + it->second.offset));
            indices.emplace_back(polylist.at(i * attributes.size() + 0));
        }
        //it->second.processed = std::vector<float>(it->second.indices.size() * it->second.stride);
        it->second.processed = std::vector<float>(vertices.size());
    }

    if (attributes.find("VERTEX") != attributes.end())
    {
        std::string vertLocation = vNode->Attribute("source");
        vertices = colsrc.at(vertLocation).data;
    }

    for (it = attributes.begin(); it != attributes.end(); it++)
    {
        for (int i = 0; i < polylist.size() / attributes.size(); i++)
        {
            int stride = colsrc.at(it->second.source).stride;
            for (int j = 0; j < stride; j++)
            {
                float x = colsrc.at(it->second.source).data.at(it->second.indices.at(i) * stride + j);
                it->second.processed.at(indices.at(i) * stride + j) = x;
            }
        }
    }
    //flip z normal

    AnimatedMesh mesh;
    mesh.id = g->FirstChildElement("mesh")->FirstChildElement("source")->NextSiblingElement("source")->NextSiblingElement("source")->Attribute("id");
    mesh.indices = indices;
    mesh.textureCoords = attributes.at("TEXCOORD").processed;
    mesh.normals = attributes.at("NORMAL").processed;
    mesh.vertices = vertices;
    if (attributes.find("COLOR") != attributes.end())
    {
        mesh.colors = attributes.at("COLOR").processed;
    }
    return mesh;
}
AnimatedMesh ColladaParser::loadAllGeometries(XMLParser::XMLElement* doc, std::map<std::string, GLuint>& texLinks)
{
    AnimatedMesh mesh;
    std::vector<GLuint> indices;
    std::vector<float> vertices;
    std::vector<float> textureCoords;
    std::vector<float> normals;
    std::vector<float> colors;
    GLuint offset = 0;
    for (XMLParser::XMLNode* current = doc->FirstChildElement("library_geometries")->
        FirstChildElement("geometry"); current != NULL; current = current->NextSibling())
    {
        AnimatedMesh m = loadGeometry(current);

        if (texLinks.find(m.id) == texLinks.end())
        {
        }
        else
        {
            for (int i = 0; i < m.indices.size(); i++)
            {
                indices.emplace_back(m.indices.at(i) + offset);
            }
            offset += (GLuint)(m.vertices.size() / 3);
            vertices.insert(vertices.end(), m.vertices.begin(), m.vertices.end());
            textureCoords.insert(textureCoords.end(), m.textureCoords.begin(), m.textureCoords.end());
            normals.insert(normals.end(), m.normals.begin(), m.normals.end());
            ModelMaterial mat;

            mat.textured = true;
            
            mat.albedo = texLinks.at(m.id);
            mat.length = m.indices.size();
            mesh.materials.emplace_back(mat);
        }
    }
    mesh.indices = indices;
    mesh.vertices = vertices;
    mesh.textureCoords = textureCoords;
    mesh.normals = normals;
    return mesh;
}
AnimatedMesh ColladaParser::parse(const std::string& dir)
{
    XMLParser::XMLDocument doc;
    doc.LoadFile(dir.c_str());

    XMLParser::XMLElement* dae = doc.FirstChildElement("COLLADA");

    unsigned long long k = dir.rfind('\\', dir.length());

    std::string colladaFileName = "[Error]";
    if (k != std::string::npos)
    {
        colladaFileName = dir.substr(k + 1, dir.length() - k);
        int periodIndex = -1;
        for (int i = colladaFileName.length() - 1; i >= 0; i--)
        {
            if (colladaFileName.at(i) == '.')
            {
                periodIndex = i;
                break;
            }
        }
        colladaFileName = colladaFileName.substr(0, periodIndex);
    }

    std::map<std::string, std::string> textures = loadTextures(dae);
    std::map<std::string, GLuint> links = getTextureLinks(dae, textures);

    AnimatedMesh meshes = loadAllGeometries(dae, links);
    meshes.id = colladaFileName;
    return meshes;
}
