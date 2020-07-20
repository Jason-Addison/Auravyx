#pragma once
#include "Texture.h"
#include "Model.h"
#include "Camera.h"
#include <vector>
#include "FBO.h"
class GFX
{
public:
	GFX();
	~GFX();

	static double FPS;

	static Camera CAM;

	static bool WIREFRAME;

	static Model quad;

	static GLuint tileMap;

	static int viewDistance;

	static int mipmapBias;

	static int terrainTextureResolution;

	static double brightness;

	static int UNLIMITED_FPS;
	
	static GLuint materials;

	static GLuint terrainMaterials;

	static void fillRect(float x, float y, float width, float height, float xScale, float yScale, float r, float g, float b, float a);

	static void fillRect(float x, float y, float xScale, float yScale, float r, float g, float b, float a);


	static void drawImage(float x, float y, float width, float height, int texture);

	static void drawImage(float x, float y, float width, float height, float rot, int texture);

	static void drawImage(float x, float y, float width, float height, float rotation, int texture, int xScale, int yScale);

	static void drawImage(float x, float y, float xVel, float yVel, float width, float height, int texture);
	
	static void drawString(std::string string, float x, float y, float size, float r, float g, float b, float a);

	static void drawStringBG(std::string string, float x, float y, float size, float r, float g, float b, float a, 
		float xB, float yB, float xSB, float ySB, float rB, float gB, float bB, float aB);

	static void drawStringBGR(std::string string, float x, float y, float size, float r, float g, float b, float a,
		float xB, float yB, float xSB, float ySB, float rB, float gB, float bB, float aB);

	static void drawStringR(std::string string, float x, float y, float size, float r, float g, float b, float a);

	static void drawStringC(std::string string, float x, float y, float size, float width, float r, float g, float b, float a);

	static void drawStringBGC(std::string string, float x, float y, float size, float width, float r, float g, float b, float a,
		float xB, float yB, float xSB, float ySB, float rB, float gB, float bB, float aB);

	static float stringWidth(std::string string, float size);

	static void renderModel(float x, float y, float z, float xScale, float yScale,
		float zScale, float xRot, float yRot, float zRot, Model* m, Camera *c, Matrix4f *projection, Texture *tex);

	static void init();

	//static void enableScissor(int x, int y, int width, int height);

	//static void disableScissor();
	static void enableScissor(float x, float y, float width, float height);

	static void disableScissor();

};

