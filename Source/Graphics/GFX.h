#pragma once
#include "Engine/Texture.h"
#include "Engine/Model.h"
#include "Engine/Camera.h"
#include <vector>
#include "Engine/FBO.h"
class GFX
{
public:
	GFX();
	~GFX();

	double FPS = 144;

	Camera CAM = Camera();

	bool WIREFRAME;

	Model quad = Model();

	GLuint tileMap = 0;

	int viewDistance = 0;

	int mipmapBias = 0;

	int terrainTextureResolution = -1;

	double brightness = 0.13;

	int UNLIMITED_FPS = -1;
	
	GLuint materials;

	GLuint terrainMaterials = -1;

	void fillRect(float x, float y, float width, float height, float xScale, float yScale, float r, float g, float b, float a);

	void fillRect(float x, float y, float xScale, float yScale, float r, float g, float b, float a);


	void drawImage(float x, float y, float width, float height, int texture);

	void drawImage(float x, float y, float width, float height, float rot, int texture);

	void drawImage(float x, float y, float width, float height, float rotation, int texture, int xScale, int yScale);

	void drawImage(float x, float y, float xVel, float yVel, float width, float height, int texture);
	
	void drawString(std::string string, float x, float y, float size, float r, float g, float b, float a);

	void drawStringBG(std::string string, float x, float y, float size, float r, float g, float b, float a, 
		float xB, float yB, float xSB, float ySB, float rB, float gB, float bB, float aB);

	void drawStringBGR(std::string string, float x, float y, float size, float r, float g, float b, float a,
		float xB, float yB, float xSB, float ySB, float rB, float gB, float bB, float aB);

	void drawStringR(std::string string, float x, float y, float size, float r, float g, float b, float a);

	void drawStringC(std::string string, float x, float y, float size, float width, float r, float g, float b, float a);

	void drawStringBGC(std::string string, float x, float y, float size, float width, float r, float g, float b, float a,
		float xB, float yB, float xSB, float ySB, float rB, float gB, float bB, float aB);

	float stringWidth(std::string string, float size);

	void renderModel(float x, float y, float z, float xScale, float yScale,
		float zScale, float xRot, float yRot, float zRot, Model* m, Camera *c, Matrix4f *projection, Texture *tex);

	void init();

	void setFPS(double fps);

	//static void enableScissor(int x, int y, int width, int height);

	//static void disableScissor();
	void enableScissor(float x, float y, float width, float height);

	void disableScissor();

};

