#pragma once
#include "GL\glew.h"
#include "GLFW\glfw3.h"

class FBO
{
public:
	FBO();
	FBO(int width, int height);
	~FBO();
	bool made;
	int width, height;
	GLuint framebuffer;
	GLuint texture;
	GLuint depthTexture;

	GLuint buffers[16];

	void create();

	void bind();

	static void unbind();

	void update(int width, int height);

	int createColourBufferAttachment(int attachment);

	void resolve(int readBuffer, FBO output);

	void clear();

	void addBuffer(int attachment);

	void addBuffer(int attachment, int format);

	void destroy();
};

