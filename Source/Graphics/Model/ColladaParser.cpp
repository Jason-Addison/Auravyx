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

std::vector<float> ColladaParser::stringToFloats(std::string str)
{
    std::vector<float> v;

    std::istringstream iss(str);

    std::copy(std::istream_iterator<float>(iss),
        std::istream_iterator<float>(),
        std::back_inserter(v));
    return v;
}

std::vector<GLuint> ColladaParser::stringToIntegers(std::string str)
{
    std::vector<GLuint> v;

    std::istringstream iss(str);

    std::copy(std::istream_iterator<int>(iss),
        std::istream_iterator<int>(),
        std::back_inserter(v));
    return v;
}

void ColladaParser::processVertex(int v, int n, int t, int c, std::vector<GLuint>* indices,
    std::vector<float>* textures, std::vector<float>* normals,
    std::vector<float>* colors, std::vector<float>* processedTextures,
    std::vector<float>* processedNormals, std::vector<float>* processedColors)
{
    indices->emplace_back(v);

    float tx = textures->at(t * 2 + 0);
    float ty = textures->at(t * 2 + 1);

    processedTextures->at(v * 2 + 0) = tx;
    processedTextures->at(v * 2 + 1) = ty;

    float nx = normals->at(n * 3 + 0);
    float ny = normals->at(n * 3 + 1);
    float nz = normals->at(n * 3 + 2);

    processedNormals->at(v * 3 + 0) = -nz;
    processedNormals->at(v * 3 + 1) = ny;
    processedNormals->at(v * 3 + 2) = nx;

    float r = colors->at(c * 3 + 0);
    float g = colors->at(c * 3 + 1);
    float b = colors->at(c * 3 + 2);

    processedColors->at(v * 3 + 0) = r;
    processedColors->at(v * 3 + 1) = g;
    processedColors->at(v * 3 + 2) = b;
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
std::map<std::string, GLuint> ColladaParser::getTextureLinks(XMLParser::XMLNode* doc, std::map<std::string, std::string> textures)
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
    XMLParser::XMLNode* sources = g;
    sources = sources->FirstChildElement("mesh")->FirstChildElement("source");
    std::string positionsStr = sources->FirstChildElement("float_array")->GetText();
    sources = sources->NextSibling();
    std::string normalsStr = sources->FirstChildElement("float_array")->GetText();
    sources = sources->NextSibling();
    std::string texID = ((XMLParser::XMLElement*) sources)->Attribute("id");
    std::string texCoordsStr = sources->FirstChildElement("float_array")->GetText();
    sources = sources->NextSibling();
    std::string colorsStr = sources->FirstChildElement("float_array")->GetText();
    XMLParser::XMLNode* pNode = g->FirstChildElement("mesh")->FirstChildElement("polylist");


    if (pNode == NULL)
    {
        pNode = g->FirstChildElement("mesh")->FirstChildElement("triangles");
    }
    std::string polylistStr = pNode->FirstChildElement("p")->GetText();

    std::vector<float> vertices = stringToFloats(positionsStr);
    std::vector<float> normals = stringToFloats(normalsStr);
    std::vector<float> texCoords = stringToFloats(texCoordsStr);
    std::vector<float> colors = stringToFloats(colorsStr);
    std::vector<GLuint> polylist = stringToIntegers(polylistStr);

    std::vector<GLuint> indices;
    std::vector<float> processedTex((vertices.size() / 3) * 2);
    std::vector<float> processedNormals(vertices.size());
    std::vector<float> processedColors(vertices.size());

    for (int i = 0; i < polylist.size() / 4; i++)
    {
        processVertex(polylist.at(i * 4 + 0), polylist.at(i * 4 + 1), polylist.at(i * 4 + 2), polylist.at(i * 4 + 3),
            &indices, &texCoords, &normals, &colors, &processedTex, &processedNormals, &processedColors);
    }
    
    AnimatedMesh mesh;
    mesh.id = texID;
    mesh.indices = indices;
    mesh.textureCoords = processedTex;
    mesh.normals = processedNormals;
    mesh.vertices = vertices;
    mesh.colors = processedColors;
    return mesh;
}
AnimatedMesh ColladaParser::loadAllGeometries(XMLParser::XMLElement* doc, std::map<std::string, GLuint> texLinks)
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


void ColladaParser::parse(std::string dir)
{
    XMLParser::XMLDocument doc;
    doc.LoadFile(dir.c_str());


    XMLParser::XMLElement* dae = doc.FirstChildElement("COLLADA");

    std::map<std::string, std::string> textures = loadTextures(dae);
    std::map<std::string, GLuint> links = getTextureLinks(dae, textures);

    AnimatedMesh meshes = loadAllGeometries(dae, links);

    Model m = Model::loadIndexed3DModel(meshes.vertices, meshes.normals, meshes.textureCoords, meshes.colors, meshes.indices);
    m.setMaterials(meshes.materials);
    Assets::getAssets()->addModel("mesh", m);
    
}

