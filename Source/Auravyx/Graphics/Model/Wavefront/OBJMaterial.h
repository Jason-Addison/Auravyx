#pragma once
#include "Auravyx/Graphics/Texture.h"
class OBJMaterial
{
public:
	OBJMaterial();
	OBJMaterial(const Texture& texture, const int length);
	~OBJMaterial();

	Texture texture;
	int length;
	float shine;
	float damper;

	Texture getTexture();

	int getLength();

};