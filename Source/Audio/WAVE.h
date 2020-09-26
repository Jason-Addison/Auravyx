#pragma once
#include "Library/AL/al.h"

class WAVE
{
public:

	ALuint buffer;
	ALenum format;
	unsigned char* buf;

	WAVE(const char* path);
	~WAVE();

	void destroy();

private:

	void load(const char * path);
};



