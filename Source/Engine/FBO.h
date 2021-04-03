#pragma once
#include "Library\GL\glew.h"
#include "Library\GLFW\glfw3.h"

class FBO
{
public:
	FBO();
	FBO(const int width, const int height);
	~FBO();
	bool made;
	int width, height;
	GLuint framebuffer;
	GLuint texture;
	GLuint depthTexture;

	GLuint buffers[16];

	void bind();

	static void unbind();

	void update(const int width, const int height);

	void resolve(const int readBuffer, const FBO& output);

	void clear();

	void destroy();

	GLenum checkStatus();

private:

	void create();

	int createColourBufferAttachment(const int attachment);

	void addBuffer(const int attachment);

	void addBuffer(const int attachment, const int format);
};

