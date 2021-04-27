#pragma once
#include "AL/al.h"

class WAVE
{
public:

	ALuint buffer;
	ALenum format;

	ALuint lengthInSamples;
	ALuint frequency;

	float duration = 0;

	unsigned char* buf;

	WAVE(const char* path);
	~WAVE();

	void destroy();

private:

	void load(const char * path);
};



