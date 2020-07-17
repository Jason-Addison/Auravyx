#pragma once
#include "Texture.h"
class OBJMaterial
{
public:
	OBJMaterial();
	OBJMaterial(Texture texture, int length);
	~OBJMaterial();

	Texture texture;
	int length;
	float shine = 0.4;
	float damper = 7;

	Texture getTexture();

	int getLength();

};

