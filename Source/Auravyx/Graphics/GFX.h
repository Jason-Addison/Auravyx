#pragma once
#include "Auravyx/Graphics/Texture.h"
#include "Auravyx/Graphics/Model/Model.h"
#include "Auravyx/Graphics/Camera.h"
#include <vector>
#include "Auravyx/Graphics/FBO.h"
class GFX
{
public:

	GFX();
	GFX(GFX *gfx);
	~GFX();

	double FPS = 144;

	double crosshairX = 0;
	double crosshairY = 0;
	double crosshairZ = 0;
	bool crosshairFound = true;

	Camera CAM = Camera();

	bool WIREFRAME;

	Model quad = Model();

	GLuint tileMap = 0;

	int viewDistance = 12;

	int mipmapBias = 0;

	int terrainTextureResolution = -1;

	double brightness = 0.13;

	int UNLIMITED_FPS = -1;
	
	GLuint materials;

	GLuint terrainMaterials = -1;

	int pos1x = 0;
	int pos1y = 0;
	int pos1z = 0;

	int pos2x = 0;
	int pos2y = 0;
	int pos2z = 0;

	void fillRect(const float x, const float y, const float width, const float height, const float xScale, const float yScale, const float r, const float g, const float b, const float a);

	void fillRect(const float x, const float y, const float xScale, const float yScale, const float r, const float g, const float b, const float a);


	void drawImage(const float x, const float y, const float width, const float height, const int texture);

	void drawImage(const float x, const float y, const float width, const float height, const float rot, const int texture);

	void drawImage(const float x, const float y, const float width, const float height, const float rotation, const int texture, const int xScale, const int yScale);

	void drawImage(const float x, const float y, const float xVel, const float yVel, const float width, const float height, const int texture);
	
	void drawString(const std::string& string, const float x, const float y, const float size, const float r, const float g, const float b, const float a);

	void drawString(const std::string& string, const float x, const float y, const float size, const float r, const float g, const float b, const float a, const float windowWidth, const float windowHeight);

	void drawStringBG(const std::string& string, const float x, const float y, const float size, const float r, const float g, const float b, const float a, 
		const float xB, const float yB, const float xSB, const float ySB, const float rB, const float gB, const float bB, const float aB);

	void drawStringBGR(const std::string& string, const float x, const float y, const float size, const float r, const float g, const float b, const float a,
		const float xB, const float yB, const float xSB, const float ySB, const float rB, const float gB, const float bB, const float aB);

	void drawStringR(const std::string& string, const float x, const float y, const float size, const float r, const float g, const float b, const float a);

	void drawStringC(const std::string& string, const float x, const float y, const float size, const float width, const float r, const float g, const float b, const float a);

	void drawStringBGC(const std::string& string, const float x, const float y, const float size, const float width, const float r, const float g, const float b, const float a,
		const float xB, const float yB, const float xSB, const float ySB, const float rB, const float gB, const float bB, const float aB);

	float stringWidth(const std::string& string, const float size);

	void renderModel(const float x, const float y, const float z, const float xScale, const float yScale,
		const float zScale, const float xRot, const float yRot, const float zRot, Model& m, Camera& c, const Matrix4f& projection, const Texture& tex);

	void renderModelIndex(const float x, const float y, const float z, const float xScale, const float yScale,
		const float zScale, const float xRot, const float yRot, const float zRot, Model& m, Camera& c, const Matrix4f& projection, const Texture& tex);

	void init();

	void setFPS(const double fps);

	//static void enableScissor(const int x, const int y, const int width, const int height);

	//static void disableScissor();
	void enableScissor(const float x, const float y, const float width, const float height);

	void disableScissor();

	static GFX* getOverlay();

	GLenum checkForGLError(std::string* s);

	GLenum checkForGLError(std::string* s, const int l);

private:
	static GFX * gfx;
};

