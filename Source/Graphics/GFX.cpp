#include "pch.h"
#include "stdafx.h"
#include "Graphics/GFX.h"
#include "Utilities/M.h"
#include "Shader/Shaders.h"
#include "Engine/WindowManager.h"
#include "Utilities/Assets.h"
#include "Engine/Clock.h"
#include "Auravyx.h"

GFX::GFX()
{
}


GFX::~GFX()
{
}

void GFX::drawImage(float x, float y, float width, float height, float rotation, int texture, int xScale, int yScale)
{
	//x = (int)x;
	//y = (int)y;
	//width = (int)width;
	//height = (int)height;
	glDisable(GL_DEPTH_TEST);
	Matrix4f transformation = M::createTransformationMatrix(0, 0, 0, width / (float)xScale * 2, height / (float)yScale * 2, 1, 0, 0, 0);
	
	transformation.translate(-1 + (x / (float)xScale * 2), -1 + (y / (float)yScale * 2), 0);
	Auravyx::getAuravyx()->getRenderer()->getShaders()->imageShader->start();
	Auravyx::getAuravyx()->getRenderer()->getShaders()->imageShader->loadTransformation(transformation);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindVertexArray(quad.getVAO());
	glEnableVertexArrayAttrib(quad.getVAO(), 0);

	glDrawArrays(GL_TRIANGLES, 0, quad.getCount());
	glEnable(GL_DEPTH_TEST);
}

void GFX::drawImage(float x, float y, float xVel, float yVel, float width, float height, int texture)
{
	//drawImage((int) x, (int) y, width, height, texture);
	//drawImage((int) (Clock::lerp * (x + xVel) + (1 - Clock::lerp) * (x)),
	//	(int) (Clock::lerp * (y + -yVel) + (1 - Clock::lerp) * (y)), width, height, texture);
	drawImage((Clock::lerp * (x + xVel) + (1 - Clock::lerp) * (x)),
		(Clock::lerp * (y + -yVel) + (1 - Clock::lerp) * (y)), width, height, texture);
}

void GFX::drawString(std::string string, float x, float y, float size, float r, float g, float b, float a)
{
	Font font = *Auravyx::getAuravyx()->getAssets()->getFont("font_plain");
	size *= 1 / (font.largestValue / size) * 2;

	glDisable(GL_DEPTH_TEST);

	//x * 2 + totalX, (int)(y * 2f + fontCharacter.getYOffset() * yScale * 2.5f / font.getSize() - yScale / 4), xScale * 2.5f / font.getSize(), -yScale * 2.5f / font.getSize()
	Matrix4f transformation;

	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	Auravyx::getAuravyx()->getRenderer()->getShaders()->fontShader->start();
	Auravyx::getAuravyx()->getRenderer()->getShaders()->fontShader->loadThickness(0.53, 0.5 / (0.5 * size));
	Auravyx::getAuravyx()->getRenderer()->getShaders()->fontShader->loadColour(r, g, b, a);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font.texture.texture);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	float totalX = 0;

	for (int i = 0; i < string.length(); i++)
	{
		FontChar fchar = font.characters.at(string.at(i));
		//
		if (fchar.width != -1)
		{
			float newX = -1 + ((x) / Auravyx::getAuravyx()->getWindow()->getWidth() * 2) + ((totalX) / Auravyx::getAuravyx()->getWindow()->getWidth() * 2);
			float newY = 1 - ((y + (fchar.yOffset - 12) * (size / font.size)) / Auravyx::getAuravyx()->getWindow()->getHeight() * 2);
			float newWidth = (size / font.size) / Auravyx::getAuravyx()->getWindow()->getWidth() * 2;
			float newHeight = (size / font.size) / Auravyx::getAuravyx()->getWindow()->getHeight() * 2;

			transformation = M::createTransformationMatrix(newX, newY, 0, newWidth, newHeight, 1, 0, 0, 0);
			Auravyx::getAuravyx()->getRenderer()->getShaders()->fontShader->loadTransformation(transformation);
			totalX += ((float)fchar.xAdvance) * (size / font.size) * 0.6; //////////////////////////////////////////////////////////

			glBindVertexArray(fchar.vaoID);
			glEnableVertexArrayAttrib(fchar.vaoID, 0);
			glEnableVertexArrayAttrib(fchar.vaoID, 1);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
	glEnable(GL_DEPTH_TEST);
}

void GFX::drawStringBG(std::string string, float x, float y, float size, float r, float g, float b, float a, float xB, float yB, float xSB, float ySB, float rB, float gB, float bB, float aB)
{
	float width = stringWidth(string, size) + 10;
	fillRect(x + xB, y + yB, width + xSB, size + ySB, rB, gB, bB, aB);
	drawString(string, x, y, size, r, g, b, a);
}

void GFX::drawStringBGR(std::string string, float x, float y, float size, float r, float g, float b, float a, float xB, float yB, float xSB, float ySB, float rB, float gB, float bB, float aB)
{
	float width = stringWidth(string, size) + 7;
	fillRect(Auravyx::getAuravyx()->getWindow()->getWidth() - width - x + xB, y + yB, width + xSB, size + ySB, rB, gB, bB, aB);
	drawStringR(string, x, y, size, r, g, b, a);
}

void GFX::drawStringR(std::string string, float x, float y, float size, float r, float g, float b, float a)
{
	drawString(string, Auravyx::getAuravyx()->getWindow()->getWidth() - stringWidth(string, size) - x - 7, y, size, r, g, b, a);
}

void GFX::drawStringC(std::string string, float x, float y, float size, float width, float r, float g, float b, float a)
{
	drawString(string, x + width / 2 - stringWidth(string, size) / 2, y, size, r, g, b, a);
}

void GFX::drawStringBGC(std::string string, float x, float y, float size, float width, float r, float g, float b, float a,
	float xB, float yB, float xSB, float ySB, float rB, float gB, float bB, float aB)
{
	float w = stringWidth(string, size);
	fillRect(width / 2 - w / 2 - x + xB, y + yB, w + xSB + 7, size + ySB, rB, gB, bB, aB);
	drawString(string, x + width / 2 - w / 2, y, size, r, g, b, a);
}

float GFX::stringWidth(std::string string, float size)
{
	Font font = *Auravyx::getAuravyx()->getAssets()->getFont("font_plain");
	size *= 1 / (font.largestValue / size) * 2;

	float totalX = 0;

	float x = 0;
	float y = 0;

	for (int i = 0; i < string.size(); i++)
	{
		FontChar fchar = font.characters.at(string.at(i));

		if (fchar.width != -1)
		{
			float newX = -1 + ((x) / Auravyx::getAuravyx()->getWindow()->getWidth() * 2) + ((totalX) / Auravyx::getAuravyx()->getWindow()->getWidth() * 2);
			float newY = 1 - ((y + (fchar.yOffset - 12) * (size / font.size)) / Auravyx::getAuravyx()->getWindow()->getHeight() * 2);
			float newWidth = (size / font.size) / Auravyx::getAuravyx()->getWindow()->getWidth() * 2;
			float newHeight = (size / font.size) / Auravyx::getAuravyx()->getWindow()->getHeight() * 2;

			totalX += ((float)fchar.xAdvance) * (size / font.size) * 0.6;
		}
	}
	return totalX;
	return -1 + ((x) / Auravyx::getAuravyx()->getWindow()->getWidth() * 2) + ((totalX) / Auravyx::getAuravyx()->getWindow()->getWidth() * 2);
}

void GFX::renderModel(float x, float y, float z, float xScale, float yScale, 
	float zScale, float xRot, float yRot, float zRot, Model* m, Camera* c, Matrix4f* projection, Texture *tex)
{
	Auravyx::getAuravyx()->getRenderer()->getShaders()->modelShader->start();
	Auravyx::getAuravyx()->getRenderer()->getShaders()->modelShader->loadCamera(c->getViewMatrix());
	Auravyx::getAuravyx()->getRenderer()->getShaders()->modelShader->loadProjectionMatrix(*projection);
	Matrix4f t = M::createTransformationMatrix(x, y, z, xScale, yScale, zScale, xRot, yRot, zRot);
	Auravyx::getAuravyx()->getRenderer()->getShaders()->modelShader->loadTransformationMatrix(t);
	Auravyx::getAuravyx()->getRenderer()->getShaders()->modelShader->loadCamera(c->x, c->y, c->z, viewDistance * 128);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex->texture);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindVertexArray(m->getVAO());
	glEnableVertexArrayAttrib(m->getVAO(), 0);
	glEnableVertexArrayAttrib(m->getVAO(), 1);
	glEnableVertexArrayAttrib(m->getVAO(), 2);
	glDrawArrays(GL_TRIANGLES, 0, m->getCount());
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	Auravyx::getAuravyx()->getRenderer()->getShaders()->modelShader->stop();
}

void GFX::fillRect(float x, float y, float width, float height, float xScale, float yScale, float r, float g, float b, float a)
{
	glDisable(GL_DEPTH_TEST);
	Matrix4f transformation = M::createTransformationMatrix(0, 0, 0, width / (float)xScale * 2, height / (float)yScale * 2, 1, 0, 0, 0);
	transformation.translate(-1 + (x / (float)xScale * 2), -1 + (y / (float)yScale * 2), 0);
	Auravyx::getAuravyx()->getRenderer()->getShaders()->rectShader->start();
	Auravyx::getAuravyx()->getRenderer()->getShaders()->rectShader->loadTransformation(transformation);
	Auravyx::getAuravyx()->getRenderer()->getShaders()->rectShader->loadColour(r, g, b, a);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glBindVertexArray(quad.getVAO());
	glEnableVertexArrayAttrib(quad.getVAO(), 0);

	glDrawArrays(GL_TRIANGLES, 0, quad.getCount());
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void GFX::fillRect(float x, float y, float xScale, float yScale, float r, float g, float b, float a)
{
	fillRect(x, Auravyx::getAuravyx()->getWindow()->getHeight() - y, xScale, -yScale, Auravyx::getAuravyx()->getWindow()->getWidth(), Auravyx::getAuravyx()->getWindow()->getHeight(), r, g, b, a);
}

void GFX::drawImage(float x, float y, float width, float height, int texture)
{
	drawImage(x, y, width, height, 0, texture, Auravyx::getAuravyx()->getWindow()->getWidth(), Auravyx::getAuravyx()->getWindow()->getHeight());
}
void GFX::drawImage(float x, float y, float width, float height, float rot, int texture)
{
	drawImage(x, y, width, height, rot, texture, Auravyx::getAuravyx()->getWindow()->getWidth(), Auravyx::getAuravyx()->getWindow()->getHeight());
}
/*void GFX::drawImage(float x, float y, float width, float height, int texture, int xScale, int yScale)
{
	drawImage(x, y, width, height, texture, xScale, yScale);
}*/
void GFX::init()
{
	std::vector<float> quadVert = { 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1 };
	quad = Model::load2DModel(quadVert);
}

void GFX::setFPS(double fps)
{
	this->FPS = fps;
}

void GFX::enableScissor(float x, float y, float width, float height)
{
	glScissor(x, Auravyx::getAuravyx()->getWindow()->getHeight() - y - height, width, height);
	glEnable(GL_SCISSOR_TEST);
}

void GFX::disableScissor()
{
	glDisable(GL_SCISSOR_TEST);
}
