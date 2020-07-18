#pragma once
#include "al.h"

class WAVE
{
public:

	ALuint buffer;
	ALenum format;
	unsigned char* buf;

	WAVE(const char* path);
	~WAVE();

private:

	void load(const char * path);
};



