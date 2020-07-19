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
	float shine;
	float damper;

	Texture getTexture();

	int getLength();

};

