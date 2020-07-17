#include "stdafx.h"
#include "Resource.h"
#include <iostream>
#include <string>
#include "FileIO.h"
#include "Log.h"
#include "SOIL\SOIL.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "Texture.h"
#include "Assets.h"
#include "FBO.h"
#include "GFX.h"
#include <FontLoader.h>
#include <atomic>
#include "GFX.h"
#include <thread>
#include <WindowManager.h>
#include <Sound.h>
#include <SoundManager.h>
#include <Settings.h>
#include <Shaders.h>
#include <Controller.h>
#include <deque>
Resource::Resource()
{
}


Resource::~Resource()
{
}

bool printEachAssetLoad = false;

struct preloadModel
{
	std::string name;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> uvs;
};

struct preloadTexture
{
	std::string name;
	unsigned char* tex;
	unsigned int width;
	unsigned int height;
	unsigned int channel;
};

struct loadingMessage
{
	std::vector<std::string> sourceLines;
	std::vector<std::string> messageLines;
	std::vector<Vec3f> messageLinesColours;
	double delay = 0;
	double start = 0;
};

std::deque<loadingMessage> loadingMessages;

std::vector<preloadModel> preloadedModels;
std::vector<preloadTexture> preloadedTextures;
std::vector<preloadTexture> preloadedTerrainTextures;

std::map<std::string, std::string> preloadShaders;

static bool logShaders = true;
std::string Resource::DIR;

std::atomic_bool shadersLoaded = false;
std::atomic_bool audioLoaded = false;
std::atomic_bool texturesLoaded = false;
std::atomic_bool terrainTexturesLoaded = false;
std::atomic_bool modelsLoaded = false;
std::atomic_bool doneAssetLoading = false;

std::string whatIsLoadingPrimary = "";
std::string whatIsLoadingSecondary = "";

loadingMessage currentLoadingMessage;

bool Resource::loadAllResources()
{
	//loadAllTextures();
	//loadTerrainTextures();
	//loadAllModels();
	if (!doneAssetLoading)
	{
		if (shadersLoaded)
		{
			Shaders shader;
			shader.init(preloadShaders);
			shadersLoaded = false;
		}
		if (modelsLoaded)
		{
			for (auto pm : preloadedModels)
			{
				Model m = Model::load3DModelT(pm.vertices, pm.normals, pm.uvs);
				Assets::addModel(pm.name, m);
			}
			modelsLoaded = false;
		}
		if (texturesLoaded)
		{
			for (auto pt : preloadedTextures)
			{
				unsigned int tex = SOIL_create_OGL_texture(pt.tex, pt.width, pt.height, pt.channel, SOIL_CREATE_NEW_ID,
					SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
				SOIL_free_image_data(pt.tex);

				Texture t(tex, pt.width, pt.height);
				Assets::addTexture(pt.name, t);
			}
			texturesLoaded = false;
		}
		if (terrainTexturesLoaded)
		{
			int width = 1024, height = 1024;
			GLsizei count = preloadedTerrainTextures.size();
			int i = 0;
			GLuint texture3D;
			glGenTextures(1, &texture3D);
			glBindTexture(GL_TEXTURE_2D_ARRAY, texture3D);

			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
			width = 1024;
			height = 1024;
			glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, count, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, NULL);
			for (auto pt : preloadedTerrainTextures)
			{
				glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, pt.tex);
				//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_LOD_BIAS, GFX::mipmapBias);
				i++;
				SOIL_free_image_data(pt.tex);
			}
			GFX::terrainMaterials = texture3D;
			terrainTexturesLoaded = false;
			doneAssetLoading = true;
			whatIsLoadingPrimary = "";
			whatIsLoadingSecondary = "";
		}
	}
	if (loadingMessages.size() > 0)
	{
		if (glfwGetTime() > loadingMessages.at(0).start + loadingMessages.at(0).delay)
		{
			loadingMessages.pop_front();
			if (loadingMessages.size() > 0)
			{
				loadingMessages.at(0).start = glfwGetTime();
			}
		}
	}
	else if(doneAssetLoading)
	{
		return true;
	}
	return false;
}

void Resource::loadAllTextures()
{
	whatIsLoadingPrimary = "Loading textures";
	std::vector<std::string> textures = FileIO::listDirectory(DIR + "\\Assets\\Textures", "png");
	Log::out("Loader", "Loading textures...", YELLOW);
	for (auto &texture : textures)
	{
		int width, height;
		int channel;
		unsigned char* pixels = SOIL_load_image(texture.c_str(), &width, &height, &channel, SOIL_LOAD_AUTO);
	
		if (pixels)
		{
			preloadTexture pt;
			pt.tex = pixels;
			std::string textureName = FileIO::getFileName(texture);
			textureName = textureName.substr(0, textureName.length() - 4);
			pt.name = textureName;
			pt.width = width;
			pt.height = height;
			pt.channel = channel;
			whatIsLoadingSecondary = pt.name;
			preloadedTextures.emplace_back(pt);
			if (printEachAssetLoad)
			{
				std::cout << "         - " << textureName << " (" << width << " x " << height << ")\n";
			}
		}
		else
		{
			std::cout << "ERORR";
		}
	}
	std::cout << "\n";
	texturesLoaded = true;
}

void Resource::loadTextureSilent(std::string texture)
{
	int tex = loadTexture(texture);
	int width = 0;
	int height = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	Texture t(tex, width, height);
	std::string textureName = FileIO::getFileName(texture);
	t.name = textureName.substr(0, textureName.length() - 4);
	Assets::addTexture(textureName.substr(0, textureName.length() - 4), t);
}

void Resource::loadAllAsyncAssets()
{
	Log::out("Loader", "Start", YELLOW);
	Log::out("Loader", "Loading settings", YELLOW);
	Settings::settings = FileIO::readConfig(Resource::DIR + "\\settings.txt");
	Log::out("Loader", "Loading controller", YELLOW);
	Controller::init();
	Log::out("Loader", "Loading textures", YELLOW);
	SoundManager m;
	m.start();
	loadAllAudio();
	//Sound s;
	//s.play(Assets::getAudio("song"));
	//s.setPitch(1.1);
	preloadShaders = loadShaders("\\Shaders\\Regular");
	shadersLoaded = true;
	//Server::loadPackets();
	loadAllModels();
	loadAllTextures();
	loadTerrainTextures();
	Log::out("Loader", "Finish", YELLOW);
}
float last = 0;
float now = 0;
double progress = 0;
void Resource::renderProgress()
{
	last = now;
	now = glfwGetTime();
	GFX::fillRect(0, 0, WindowManager::getWidth(), WindowManager::getHeight() / 4, 1, 1, 1, 1);
	//progress = now - last;
	progress -= (now - last) * 3;
	float a = sin((progress) * 3.14);
	float b = sin((progress + 0.5) * 3.14);
	float c = sin((progress + 1) * 3.14);
	float d = sin((progress + 1.5) * 3.14);

	float squareSize = 10;
	float spacingSize = 5;

	float loadingX = 10;
	float loadingY = WindowManager::getHeight() - squareSize * 2 - spacingSize - loadingX;

	GFX::fillRect(loadingX, loadingY, squareSize, squareSize, 1, 1, 1, a);
	GFX::fillRect(loadingX + squareSize + spacingSize, loadingY, squareSize, squareSize, 1, 1, 1, b);
	GFX::fillRect(loadingX, loadingY + squareSize + spacingSize, squareSize, squareSize, 1, 1, 1, d);
	GFX::fillRect(loadingX + squareSize + spacingSize, loadingY + squareSize + spacingSize, squareSize, squareSize, 1, 1, 1, c);

	GFX::fillRect(WindowManager::getWidth() - loadingX - squareSize * 2 - spacingSize, loadingY, squareSize, squareSize, 1, 1, 1, b);
	GFX::fillRect(WindowManager::getWidth() - loadingX - squareSize * 2 - spacingSize + squareSize + spacingSize, loadingY, squareSize, squareSize, 1, 1, 1, a);
	GFX::fillRect(WindowManager::getWidth() - loadingX - squareSize * 2 - spacingSize, loadingY + squareSize + spacingSize, squareSize, squareSize, 1, 1, 1, c);
	GFX::fillRect(WindowManager::getWidth() - loadingX - squareSize * 2 - spacingSize + squareSize + spacingSize, loadingY + squareSize + spacingSize, squareSize, squareSize, 1, 1, 1, d);

	GFX::drawStringC(whatIsLoadingPrimary, 0, WindowManager::getHeight() - 45, 30, WindowManager::getWidth(), 1, 1, 1, 1);
	GFX::drawStringC(whatIsLoadingSecondary, 0, WindowManager::getHeight() - 20, 25, WindowManager::getWidth(), 1, 1, 1, 1);

	if (loadingMessages.size() > 0)
	{
		int l = 1;
		for (int i = 0; i < loadingMessages.at(0).messageLines.size(); i++)
		{
			GFX::drawString(loadingMessages.at(0).messageLines.at(i), 5, 
				WindowManager::getHeight() / 4 + 0 + l++ * 15, 25,
				loadingMessages.at(0).messageLinesColours.at(i).x,
				loadingMessages.at(0).messageLinesColours.at(i).y, loadingMessages.at(0).messageLinesColours.at(i).z, 1);
		}
		double remainingTime = (loadingMessages.at(0).start + loadingMessages.at(0).delay) - glfwGetTime();
		if (remainingTime < 0)
		{
			remainingTime = 0;
		}
		GFX::drawString("Message ends in " + Util::removeDecimal(remainingTime, 1) + "s", 5,
			loadingMessages.at(0).sourceLines.size() * 15 + WindowManager::getHeight() / 4 + l++ * 15, 25,
			1, 1, 1, 1);
	}
}
void Resource::clearPreloadedResources()
{
	preloadedModels = std::vector<preloadModel>();
	preloadedTextures = std::vector<preloadTexture>();
	preloadedTerrainTextures = std::vector<preloadTexture>();
	preloadShaders = std::map<std::string, std::string>();

	whatIsLoadingPrimary.clear();
	whatIsLoadingSecondary.clear();
}
void Resource::printLoadingMessage(std::vector<std::string> lines, double delay, std::vector<Vec3f> colours)
{
	loadingMessage lm;
	lm.messageLines = lines;
	lm.messageLinesColours = colours;
	lm.delay = delay;
	lm.start = glfwGetTime();
	
	loadingMessages.emplace_back(lm);
}
void Resource::loadBootAssets()
{
	Shaders shaders;
	shaders.initBootShaders(Resource::loadShaders("\\Shaders\\Base"));
	//loadTextureSilent(Resource::DIR + "\\Assets\\Boot\\font.png");
	///loadTextureSilent(Resource::DIR + "\\Assets\\Boot\\font_plain-.png");
	//FontLoader::loadFont(Resource::DIR + "\\Assets\\font.fnt");
	loadTextureSilent(Resource::DIR + "\\Assets\\Boot\\font_plain.png");
	loadTextureSilent(Resource::DIR + "\\Assets\\Boot\\goo.png");
	FontLoader::loadFont(Resource::DIR + "\\Assets\\Boot\\font_plain.fnt");
}

int Resource::loadTexture(std::string dir)
{
	unsigned int tex = SOIL_load_OGL_texture(dir.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	int width, height;
	
	
	return tex;
}

void Resource::loadAllModels()
{
	whatIsLoadingPrimary = "Loading models";
	std::vector<std::string> models = FileIO::listDirectory(DIR + "\\Assets\\Models", "obj");
	std::vector<int> texList;
	std::string s;
	s += ((char) 0);

	Log::out("Loader", "Loading models...", YELLOW);

	for (auto& model : models)
	{
		std::vector<std::string> lines = FileIO::readLines(model);
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> uv;
		std::vector<int> indicesV;
		std::vector<int> indicesN;
		std::vector<int> indicesUV;
		for (auto l : lines)
		{
			std::vector<std::string> splitLine = Util::splitString(l, " ");
			if (l.at(0) == 'v' && l.at(1) == 't')
			{
				uv.emplace_back(stof(splitLine.at(1)));
				uv.emplace_back(stof(splitLine.at(2)));
			}
			else if (l.at(0) == 'v' && l.at(1) == 'n')
			{
				normals.emplace_back(stof(splitLine.at(1)));
				normals.emplace_back(stof(splitLine.at(2)));
				normals.emplace_back(stof(splitLine.at(3)));
			}
			else if (l.at(0) == 'v')
			{
				vertices.emplace_back(stof(splitLine.at(1)));
				vertices.emplace_back(stof(splitLine.at(2)));
				vertices.emplace_back(stof(splitLine.at(3)));
			}
			else if (l.at(0) == 'f')
			{
				std::vector<std::string> aFace = Util::splitString(splitLine.at(1), "/");
				std::vector<std::string> bFace = Util::splitString(splitLine.at(2), "/");
				std::vector<std::string> cFace = Util::splitString(splitLine.at(3), "/");
				indicesV.emplace_back(stoi(aFace.at(0)));
				indicesUV.emplace_back(stoi(aFace.at(1)));
				indicesN.emplace_back(stoi(aFace.at(2)));

				indicesV.emplace_back(stoi(bFace.at(0)));
				indicesUV.emplace_back(stoi(bFace.at(1)));
				indicesN.emplace_back(stoi(bFace.at(2)));

				indicesV.emplace_back(stoi(cFace.at(0)));
				indicesUV.emplace_back(stoi(cFace.at(1)));
				indicesN.emplace_back(stoi(cFace.at(2)));
			}
		}
		std::vector<float> verticesOut;
		std::vector<float> normalsOut;
		std::vector<float> uvOut;
		
		for (int i : indicesV)
		{
			verticesOut.emplace_back(vertices.at((i - 1) * 3 + 0));
			verticesOut.emplace_back(vertices.at((i - 1) * 3 + 1));
			verticesOut.emplace_back(vertices.at((i - 1) * 3 + 2));
		}
		for (int i : indicesN)
		{
			normalsOut.emplace_back(normals.at((i - 1) * 3 + 0));
			normalsOut.emplace_back(normals.at((i - 1) * 3 + 1));
			normalsOut.emplace_back(normals.at((i - 1) * 3 + 2));
		}
		for (int i : indicesUV)
		{
			uvOut.emplace_back(uv.at((i - 1) * 2 + 0));
			uvOut.emplace_back(uv.at((i - 1) * 2 + 1));
		}
		for (int i = 0; i < verticesOut.size(); i++)
		{
			//std::cout << verticesOut.at(i) << "\n";
		}
		std::string fileName = FileIO::getFileName(model);
		if (printEachAssetLoad)
		{
			std::cout << "         - " << fileName << "\n";
		}
		fileName = fileName.substr(0, fileName.length() - 4);

		preloadModel pm;
		pm.vertices = verticesOut;
		pm.normals = normalsOut;
		pm.uvs = uvOut;
		pm.name = fileName;

		whatIsLoadingSecondary = pm.name;

		preloadedModels.emplace_back(pm);
	}
	std::cout << "\n";
	modelsLoaded = true;
}

void Resource::loadAllAudio()
{
	whatIsLoadingPrimary = "Loading audio";
	std::vector<std::string> sounds = FileIO::listDirectory(DIR + "\\Assets\\Audio", "wav");
	Log::out("Loader", "Loading audio...", YELLOW);
	for (auto& sound : sounds)
	{
		WAVE wave(sound.c_str());

		std::string audioName = FileIO::getFileName(sound);
		audioName = audioName.substr(0, audioName.length() - 4);
		Assets::addAudio(audioName, wave);
		if (printEachAssetLoad)
		{
			std::cout << "         - " << audioName << "\n";
		}

		whatIsLoadingSecondary = audioName;
	}
	std::cout << "\n";
}

std::map<std::string, std::string> Resource::loadShaders(std::string dir)
{
	whatIsLoadingPrimary = "Loading shaders";
	int pass = 0;
	int fail = 0;
	std::cout << "\n";
	if (logShaders)
	{
		Log::out("OpenGL", "--- Loading all shaders ---", LBLUE);
	}

	std::string GAME_DIR = DIR + dir;
	Log::out("OpenGL", "Loading shaders from : " + GAME_DIR, LBLUE);
	std::string extVert = "vert";
	std::string extFrag = "frag";
	std::vector<std::string> vertexShaders = FileIO::listDirectory(GAME_DIR, extVert);
	//std::vector<std::string> vertexShaders = FileIO::listDirectory(GAME_DIR + "Source\\Shaders\\", extVert);
	std::map<std::string, std::string> shaders;
	Log::out("OpenGL", "Loading vertex shaders...", LBLUE);
	for (auto & i : vertexShaders)
	{
		std::string path = i;
		std::string shaderName = FileIO::getFileName(i);
		whatIsLoadingSecondary = shaderName;
		if (printEachAssetLoad)
		{
			std::cout << "         - " << shaderName << "\n";
		}
		shaders.emplace(shaderName, FileIO::readTextFile(path));
		pass++;
	}
	std::cout << "\n";
	std::vector<std::string> fragmentShaders = FileIO::listDirectory(GAME_DIR, extFrag);
	Log::out("OpenGL", "Loading fragment shaders...", LBLUE);
	for (auto & i : fragmentShaders)
	{
		std::string path = i;
		std::string shaderName = FileIO::getFileName(i);
		whatIsLoadingSecondary = shaderName;
		if (printEachAssetLoad)
		{
			std::cout << "         - " << shaderName << "\n";
		}
		shaders.emplace(shaderName, FileIO::readTextFile(path));
		pass++;
	}
	std::cout << "\n";
	return shaders;
}


void Resource::loadTerrainTextures()
{
	whatIsLoadingPrimary = "Loading terrain textures";
	std::vector<std::string> textures = FileIO::listDirectory(DIR + "\\Assets\\Terrain", "png");
	std::vector<int> texList;
	std::string png = "png";
	
	Log::out("Loader", "Loading terrain textures...", YELLOW);
	for (auto & texture : textures)
	{
		int width, height;
		int channel;
		unsigned char* pixels = SOIL_load_image(texture.c_str(), &width, &height, &channel, SOIL_LOAD_RGBA);
		if (pixels)
		{
			preloadTexture pt;
			pt.tex = pixels;
			pt.width = width;
			pt.height = height;
			pt.channel = channel;
			preloadedTerrainTextures.emplace_back(pt);
			GFX::terrainTextureResolution = width;
			std::string textureName = FileIO::getFileName(texture);
			textureName = textureName.substr(0, textureName.length() - 4);
			whatIsLoadingSecondary = textureName;
		}
		else
		{
			Log::out("Loader", "Texture error (" + FileIO::getFileName(texture) + ")", RED);
		}
	}
	terrainTexturesLoaded = true;
}
