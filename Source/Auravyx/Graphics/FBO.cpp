#include "Auravyx/Graphics/FBO.h"
#include "Auravyx/UI/Window.h"
#include <Logger/Log.h>

FBO::FBO()
{
}
FBO::FBO(const int width, const int height)
{
	this->width = width;
	this->height = height;
	create();
}

FBO::~FBO()
{
}

int FBO::createColourBufferAttachment(const int attachment)
{
	GLuint colourBuffer = 0;
	glGenRenderbuffers(1, &colourBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, colourBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, colourBuffer);
	return colourBuffer;
}
void FBO::create()
{
	GLenum er;
	for (int i = 0; i < 16; i++)
	{
		buffers[i] = 0;
	}
	made = true;
	glGenFramebuffers(1, &framebuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glViewport(0, 0, width, height);
	int size = 0;

	glGenRenderbuffers(1, &depthTexture);
	glBindRenderbuffer(GL_RENDERBUFFER, depthTexture);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthTexture);

	GLenum renderBuffers[8];
	addBuffer(0, GL_RGB);
	addBuffer(1, GL_RGB);
	addBuffer(2, GL_RGB);
	addBuffer(3, GL_RGB);
	addBuffer(4, GL_RGB32F);
	addBuffer(5, GL_RGB);

	//addBuffer(8, GL_RGB);
	//addBuffer(7, GL_RGB);
	//addBuffer(3, GL_RGBA);
	//addBuffer(4, GL_RGBA);
	//addBuffer(5, GL_RGBA);
	//addBuffer(6, GL_RGBA);
	for (int i = 0; i < 8; i++)
	{
		if (buffers[i] != 0)
		{
			renderBuffers[size++] = GL_COLOR_ATTACHMENT0 + i;
		}
	}
	glDrawBuffers(size, renderBuffers);
	unbind();
}
void FBO::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, width, height);
}
void FBO::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Window::getWindow()->getWidth(), Window::getWindow()->getHeight());
}

void FBO::update(const int width, const int height)
{
	if ((this->width != width || this->height != height) && (width != 0 && height != 0))
	{
		this->width = width;
		this->height = height;
		destroy();
		create();
	}
}

void FBO::resolve(const int readBuffer, const FBO& output)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, output.framebuffer);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->framebuffer);
	glReadBuffer(readBuffer);
	glBlitFramebuffer(0, 0, width, height, 0, 0, output.width, output.height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	unbind();
}
void FBO::clear()
{
    glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FBO::addBuffer(const int attachment, const int format)
{
	glActiveTexture(GL_TEXTURE0 + attachment);
	glGenTextures(1, &buffers[attachment]);
	glBindTexture(GL_TEXTURE_2D, buffers[attachment]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, GL_TEXTURE_2D, buffers[attachment], 0);
}

void FBO::destroy()
{
	for (int i = 0; i < 16; i++)
	{
		if (buffers[i] != 0)
		{
			glDeleteTextures(1, &buffers[i]);
			glDeleteRenderbuffers(1, &buffers[i]);
		}
	}
	glDeleteRenderbuffers(1, &depthTexture);
	glDeleteFramebuffers(1, &framebuffer);
}

GLenum FBO::checkStatus()
{
	GLenum s = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (s == GL_FRAMEBUFFER_COMPLETE)
	{
		return 0;
	}
	return s;
}

void FBO::addBuffer(const int attachment)
{
	addBuffer(attachment, GL_RGBA16);
}