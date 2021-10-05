#include "MSVC/pch.h"
#include "Auravyx/Utility/IO/Resource.h"
#include <iostream>
#include "Auravyx/Utility/IO/FileIO.h"
#include "Logger/Log.h"
#include "SOIL/SOIL.h"
#include "Auravyx/Core/Assets.h"
#include <Auravyx/Graphics/Font/FontLoader.h>
#include <Auravyx/UI/Window.h>
#include <Auravyx/Audio/SoundManager.h>
#include <Auravyx/Core/Settings.h>
#include <Auravyx/Graphics/Shader/Shaders.h>
#include <Auravyx/UI/Control/Controller.h>
#include <deque>
#include <filesystem>
#include <thread>
#include "Auravyx/Graphics/Model/Collada/ColladaParser.h"
#include "Auravyx/Graphics/Renderer.h"
#include "Auravyx/Graphics/GFX.h"
#include <Auravyx/Modify/Modify.h>
#include <cmath>
#include <Auravyx/Core/Command/CommandRegistry.h>

Resource::Resource()
{
}

Resource::Resource(Resource* r)
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
	unsigned char* tex = 0;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int channel = 0;
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

std::string DIR;

std::atomic_bool shadersLoaded = false;
std::atomic_bool audioLoaded = false;
std::atomic_bool texturesLoaded = false;
std::atomic_bool terrainTexturesLoaded = false;
std::atomic_bool modsLoaded = false;
std::atomic_bool modelsLoaded = false;
std::atomic_bool doneAssetLoading = false;

std::string whatIsLoadingPrimary = "";
std::string whatIsLoadingSecondary = "";

loadingMessage currentLoadingMessage;

std::string ttDebug = "Terrain texture";
std::string fDebug = "Font texture";
std::string otherDebug = "Other";
std::string bootDebug = "Boot";

int textureCounter = 0;

bool Resource::loadAllResources()
{
	//loadAllTextures();
	//loadTerrainTextures();
	//loadAllModels();
	if (!doneAssetLoading)
	{
		if (shadersLoaded)
		{
			Renderer::getRenderer()->getShaders()->init(preloadShaders);
			shadersLoaded = false;
		}
		if (modelsLoaded)
		{
			for (auto pm : preloadedModels)
			{
				Model m = Model::load3DModelT(pm.vertices, pm.normals, pm.uvs);
				Assets::getAssets()->getAssets()->addModel(pm.name, m);
			}
			modelsLoaded = false;
		}
		if (texturesLoaded)
		{
			int nextUp = textureCounter + 1;
			if (textureCounter >= preloadedTextures.size())
			{
				texturesLoaded = false;
				Log::out("AWWEWAJIOWADIJUDAWJIOU");
			}
			else
			{
				for (int i = textureCounter; i < nextUp; i++)
				{
					auto pt = preloadedTextures.at(i);
					unsigned int tex = SOIL_create_OGL_texture(pt.tex, pt.width, pt.height, pt.channel, SOIL_CREATE_NEW_ID,
						SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
					if (tex == 0)
					{
						Log::out("[SOIL] Error code: " + std::string(SOIL_last_result()) + " at " + pt.name);
					}

					SOIL_free_image_data(pt.tex);

					if (tex == 0)
					{
						Log::out("[SOIL] Error code: " + std::string(SOIL_last_result()) + " at " + pt.name);
					}
					Texture t(tex, pt.width, pt.height);
					Assets::getAssets()->getAssets()->addTexture(pt.name, t);
				}
				textureCounter++;
			}
		}
		else if (terrainTexturesLoaded)
		{
			int l = 0;
			int width = 1024, height = 1024;
			GLsizei count = (GLsizei) preloadedTerrainTextures.size();
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
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, count, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, NULL);
			for (auto pt : preloadedTerrainTextures)
			{
				glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, pt.tex);
				glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_LOD_BIAS, (GLfloat) GFX::getOverlay()->mipmapBias);
				i++;
				SOIL_free_image_data(pt.tex);
				if (pt.tex == 0)
				{
					Log::out("[SOIL] Error code: " + std::string(SOIL_last_result()) + " at " + pt.name);
				}
			}
			GFX::getOverlay()->terrainMaterials = texture3D;
			terrainTexturesLoaded = false;
		}
		else if (modsLoaded)
		{
			whatIsLoadingPrimary = "";
			whatIsLoadingSecondary = "";
			doneAssetLoading = true;
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
		CommandRegistry::loadBaseCommands();
		CommandRegistry::loadAllStrings();
		return true;
	}

	return false;
}

void Resource::loadAllTextures()
{
	whatIsLoadingPrimary = "Loading textures";
	std::vector<std::string> textures = FileIO::listDirectory(DIR + "/Assets/Textures", "png");
	Log::out("Loader", "Loading textures...", YELLOW);
	for (auto &texture : textures)
	{
		int width, height;
		int channel;
		unsigned char* pixels = SOIL_load_image(texture.c_str(), &width, &height, &channel, SOIL_LOAD_AUTO);
		if (pixels == 0)
		{
			Log::out("[SOIL] Error code: " + std::string(SOIL_last_result()) + " at " + texture);
		}
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
				Log::out("         - " + textureName + " (" + std::to_string(width) + " x " + std::to_string(height) + ")");
			}
		}
		else
		{
			Log::out("Error!");
		}
	}
	texturesLoaded = true;
}

void Resource::loadTextureSilent(const std::string& texture)
{
	int tex = loadTexture(texture);
	int width = 0;
	int height = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	Texture t(tex, width, height);
	std::string textureName = FileIO::getFileName(texture);
	t.name = textureName.substr(0, textureName.length() - 4);
	Assets::getAssets()->getAssets()->addTexture(t.name, t);
}

void Resource::loadAllAsyncAssets()
{
	preloadShaders = loadShaders("/Shaders/Regular");
	shadersLoaded = true;
	SoundManager::getSoundManager()->start();

	Log::out("Loader", "Loading settings...", YELLOW);
	Settings::settings = FileIO::readConfig(Resource::getInstance().DIR + "/settings.txt");
	Log::out("Loader", "Loading controller...", YELLOW);

	Window::getWindow()->getController()->init();
	Log::out("Loader", "Loading textures...", YELLOW);
	loadAllAudio();

	//Sound s;
	//s.play(Assets::getAssets()->getAssets()->getAudio("song"));
	//s.setPitch(1.1);
	//Server::loadPackets();
	loadAllModels();
	loadAllTextures();
	loadAllTerrainTextures();
	Log::out("Loader", "Done.", YELLOW);
	loadAllMods();
}
double lastP = 0;
double nowP = 0;
double progress = 0;
void Resource::renderProgress()
{
	lastP = nowP;
	nowP = glfwGetTime();
	//GFX::getOverlay()->fillRect(0, 0, (float) WindowManager::getWindow()->getWidth(), (float) WindowManager::getWindow()->getHeight() / 4, 1, 1, 1, 1);
	//progress = now - last;
	progress -= (nowP - lastP) * (double) 3;
	float a = (float) sin((progress) * 3.14);
	float b = (float) sin((progress + 0.5) * 3.14);
	float c = (float) sin((progress + 1) * 3.14);
	float d = (float) sin((progress + 1.5) * 3.14);

	float squareSize = 10;
	float spacingSize = 5;

	float loadingX = 10;
	float loadingY = Window::getWindow()->getHeight() - squareSize * 2 - spacingSize - loadingX;

	float rR = 1;
	float gG = 1;
	float bB = 1;

	GFX::getOverlay()->fillRect(loadingX, loadingY, squareSize, squareSize, rR, gG, bB, a);
	GFX::getOverlay()->fillRect(loadingX + squareSize + spacingSize, loadingY, squareSize, squareSize, rR, gG, bB, b);
	GFX::getOverlay()->fillRect(loadingX, loadingY + squareSize + spacingSize, squareSize, squareSize, rR, gG, bB, d);
	GFX::getOverlay()->fillRect(loadingX + squareSize + spacingSize, loadingY + squareSize + spacingSize, squareSize, squareSize, rR, gG, bB, c);

	GFX::getOverlay()->fillRect(Window::getWindow()->getWidth() - loadingX - squareSize * 2 - spacingSize, loadingY, squareSize, squareSize, rR, gG, bB, b);
	GFX::getOverlay()->fillRect(Window::getWindow()->getWidth() - loadingX - squareSize * 2 - spacingSize + squareSize + spacingSize, loadingY, squareSize, squareSize, rR, gG, bB, a);
	GFX::getOverlay()->fillRect(Window::getWindow()->getWidth() - loadingX - squareSize * 2 - spacingSize, loadingY + squareSize + spacingSize, squareSize, squareSize, rR, gG, bB, c);
	GFX::getOverlay()->fillRect(Window::getWindow()->getWidth() - loadingX - squareSize * 2 - spacingSize + squareSize + spacingSize, loadingY + squareSize + spacingSize, squareSize, squareSize, rR, gG, bB, d);

	GFX::getOverlay()->drawStringC(whatIsLoadingPrimary, 0, (float)Window::getWindow()->getHeight() - 45, 30, (float) Window::getWindow()->getWidth(), rR, gG, bB, 1);
	GFX::getOverlay()->drawStringC(whatIsLoadingSecondary, 0, (float)Window::getWindow()->getHeight() - 20, 25, (float)Window::getWindow()->getWidth(), rR, gG, bB, 1);

	if (loadingMessages.size() > 0)
	{
		int l = 1;
		for (int i = 0; i < loadingMessages.at(0).messageLines.size(); i++)
		{
			GFX::getOverlay()->drawString(loadingMessages.at(0).messageLines.at(i), 5, 
				Window::getWindow()->getHeight() / 4 + (float) (l++) * 15, 25,
				loadingMessages.at(0).messageLinesColours.at(i).x,
				loadingMessages.at(0).messageLinesColours.at(i).y, loadingMessages.at(0).messageLinesColours.at(i).z, 1);
		}
		double remainingTime = (loadingMessages.at(0).start + loadingMessages.at(0).delay) - glfwGetTime();
		if (remainingTime < 0)
		{
			remainingTime = 0;
		}
		GFX::getOverlay()->drawString("Message ends in " + Util::removeDecimal(remainingTime, 1) + "s", 5,
			loadingMessages.at(0).sourceLines.size() * 15 + Window::getWindow()->getHeight() / 4 + (float) (l++) * 15, 25,
			rR, gG, bB, 1);
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
void Resource::printLoadingMessage(const std::vector<std::string>& lines, const double delay, const std::vector<Vec3f>& colours)
{
	loadingMessage lm;
	lm.messageLines = lines;
	lm.messageLinesColours = colours;
	lm.delay = delay;
	lm.start = glfwGetTime();
	
	loadingMessages.emplace_back(lm);
}
void Resource::cleanupPrimaryResources()
{
	Assets::getAssets()->getAssets()->deleteAudio();
	Log::out("Cleanup", "Cleaning up models...", LIGHT_GRAY);
	Assets::getAssets()->getAssets()->deleteModels();
}
void Resource::cleanupRemainingResources()
{
	Log::out("Cleanup", "Cleaning up textures...", LIGHT_GRAY);
	Assets::getAssets()->getAssets()->deleteTextures();
	Log::out("Cleanup", "Cleaning up fonts...", LIGHT_GRAY);
	Assets::getAssets()->getAssets()->deleteFonts();
}
Resource& Resource::getInstance()
{
	static Resource resource;
	return resource;
}
void Resource::loadBootAssets()
{
	Renderer::getRenderer()->getShaders()->initBootShaders(Resource::loadShaders("/Shaders/Base"));

	//loadTextureSilent(Resource::getInstance().DIR + "/Assets/Boot/font.png");
	///loadTextureSilent(Resource::getInstance().DIR + "/Assets/Boot/font_plain-.png");
	//FontLoader::loadFont(Resource::getInstance().DIR + "/Assets/font.fnt");
	loadTextureSilent(Resource::getInstance().DIR + "/Assets/Boot/font_plain.png");
	FontLoader::loadFont(Resource::getInstance().DIR + "/Assets/Boot/font_plain.fnt");
}

int Resource::loadTexture(const std::string& dir)
{
	unsigned int tex = SOIL_load_OGL_texture(dir.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (tex == 0)
	{
		Log::out("[SOIL] Error code: " + std::string(SOIL_last_result()) + " at " +dir);
	}
	int width = 0, height = 0;
	
	
	return tex;
}

void Resource::loadAllModels()
{
	whatIsLoadingPrimary = "Loading models";
	std::vector<std::string> models = FileIO::listDirectory(DIR + "/Assets/Models", "obj");
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
			verticesOut.emplace_back(vertices.at((size_t) (((size_t) i - 1) * 3 + 0)));
			verticesOut.emplace_back(vertices.at((size_t) (((size_t) i - 1) * 3 + 1)));
			verticesOut.emplace_back(vertices.at((size_t) (((size_t) i - 1) * 3 + 2)));
		}
		for (int i : indicesN)
		{
			normalsOut.emplace_back(normals.at((size_t) (((size_t) i - 1) * 3 + 0)));
			normalsOut.emplace_back(normals.at((size_t) (((size_t) i - 1) * 3 + 1)));
			normalsOut.emplace_back(normals.at((size_t) (((size_t) i - 1) * 3 + 2)));
		}
		for (int i : indicesUV)
		{
			uvOut.emplace_back(uv.at((size_t) (((size_t) i - 1) * 2 + 0)));
			uvOut.emplace_back(uv.at((size_t) (((size_t) i - 1) * 2 + 1)));
		}
		std::string fileName = FileIO::getFileName(model);
		if (printEachAssetLoad)
		{
			Log::out("         - " + fileName);
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
	modelsLoaded = true;
}

void Resource::loadAllAudio()
{
	whatIsLoadingPrimary = "Loading audio";
	std::vector<std::string> sounds = FileIO::listDirectory(DIR + "/Assets/Audio", "wav");
	Log::out("Loader", "Loading audio...", YELLOW);
	for (auto& sound : sounds)
	{
        std::string audioName = FileIO::getFileName(sound);
        audioName = audioName.substr(0, audioName.length() - 4);
        if (printEachAssetLoad)
        {
            Log::out("         - " + audioName);
        }

        whatIsLoadingSecondary = audioName;
		WAVE wave(sound.c_str());
		Assets::getAssets()->getAssets()->addAudio(audioName, wave);
	}
}

std::map<std::string, std::string> Resource::loadShaders(const std::string& dir)
{
	whatIsLoadingPrimary = "Loading shaders";
	int pass = 0;
	int fail = 0;
	if (logShaders)
	{
		Log::out("OpenGL", "--- Loading all shaders ---", LBLUE);
	}

	std::string GAME_DIR = DIR + dir;
	Log::out("OpenGL", "Loading shaders from : " + GAME_DIR, LBLUE);

	std::string extVert = "vert";
	std::string extFrag = "frag";
	std::vector<std::string> vertexShaders = FileIO::listDirectory(GAME_DIR, extVert);

	std::map<std::string, std::string> shaders;
	Log::out("OpenGL", "Loading vertex shaders...", LBLUE);
	
	for (auto & i : vertexShaders)
	{
		std::string path = i;
		std::string shaderName = FileIO::getFileName(i);
		whatIsLoadingSecondary = shaderName;
		if (printEachAssetLoad)
		{
			Log::out("         - " + shaderName);
		}
		shaders.emplace(shaderName, FileIO::readTextFile(path));
		pass++;
	}
	std::vector<std::string> fragmentShaders = FileIO::listDirectory(GAME_DIR, extFrag);
	Log::out("OpenGL", "Loading fragment shaders...", LBLUE);
	for (auto & i : fragmentShaders)
	{
		std::string path = i;
		std::string shaderName = FileIO::getFileName(i);
		whatIsLoadingSecondary = shaderName;
		if (printEachAssetLoad)
		{
			Log::out("         - " + shaderName);
		}
		shaders.emplace(shaderName, FileIO::readTextFile(path));
		pass++;
	}
	return shaders;
}


void Resource::loadAllTerrainTextures()
{
	whatIsLoadingPrimary = "Loading terrain textures";
	std::vector<std::string> textures = FileIO::listDirectory(DIR + "/Assets/Terrain", "png");
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
			GFX::getOverlay()->terrainTextureResolution = width;
			std::string textureName = FileIO::getFileName(texture);
			textureName = textureName.substr(0, textureName.length() - 4);
			whatIsLoadingSecondary = textureName;

			if (printEachAssetLoad)
			{
				Log::out("         - " + textureName + " (" + std::to_string(width) + " x " + std::to_string(height) + ")");
			}
		}
		else
		{
			Log::out("Loader", "Texture error (" + FileIO::getFileName(texture) + ")", RED);
		}
	}
	terrainTexturesLoaded = true;
}
void Resource::loadAllMods()
{
	whatIsLoadingPrimary = "Loading mods";

	Log::out("Modify", "Loading all mods...", LIGHT_GRAY);
	std::vector<std::string> mods = FileIO::listDirectory(Resource::getInstance().DIR + "/Mods/Enabled/");
	for (auto m : mods)
	{
		if (std::filesystem::is_directory(m))
		{
			std::vector<std::string> confFiles = FileIO::listDirectory(m, "conf");
			std::vector<std::string> dllFiles = FileIO::listDirectory(m, "dll");
			std::vector<std::string> soFiles = FileIO::listDirectory(m, "so");
			std::string modName = "? Mod Name ?";
			std::string modVersion = "? Mod Version ?";
			std::string gameVersion = "? Game Version ?";

			if (confFiles.size() >= 1)
			{
				std::map<std::string, std::string> configEntries = FileIO::readConfig(confFiles.at(0));
				std::map<std::string, std::string>::const_iterator modNameIter = configEntries.find("mod-name");
				std::map<std::string, std::string>::const_iterator modVersionIter = configEntries.find("mod-version");
				std::map<std::string, std::string>::const_iterator modGameVersionIter = configEntries.find("game-version");
				if (modNameIter != configEntries.end())
				{
					modName = modNameIter->second;
					whatIsLoadingSecondary = modName;
				}
				if (modVersionIter != configEntries.end())
				{
					modVersion = modVersionIter->second;
				}
				if (modGameVersionIter != configEntries.end())
				{
					gameVersion = modGameVersionIter->second;
				}
			}
			else
			{
				if (dllFiles.size() >= 1)
				{
					modName = FileIO::getFileNameNoEXT(dllFiles.at(0));
				}
			}
			if (dllFiles.size() > 0)
			{
				if (!Modify::getModify()->loadMod(dllFiles.at(0)))
				{
					Log::out("         - " + modName + " [v" + modVersion + "] for game version " + gameVersion);
				}
				else
				{
					Log::out("         - [!] Can't load : " + modName + " [v" + modVersion + "] for game version " + gameVersion, RED);
				}
			}
			else
			{
				Log::out("         - [!] DLL missing : " + modName + " [v" + modVersion + "] for game version " + gameVersion, RED);
			}
		}
	}
	Log::out("Modify", "Done.", LIGHT_GRAY);
	modsLoaded = true;
}