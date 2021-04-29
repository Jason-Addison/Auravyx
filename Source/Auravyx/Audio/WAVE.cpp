#include "Auravyx/Audio/WAVE.h"
#ifdef __linux__ 
#elif _WIN32
#else
#endif

#include "Logger/Log.h"
#include <fstream>

WAVE::WAVE(const char* path)
{
	load(path);
}


WAVE::~WAVE()
{
}

void WAVE::destroy()
{
	alDeleteBuffers(1, &buffer);
}
#include "Logger/Log.h"
#include <iostream>
void WAVE::load(const char* path)
{
	alGenBuffers((ALuint)1, &buffer);
	ALvoid* data;
	ALboolean loop = AL_FALSE;

	FILE* fp = NULL;
	fp = (FILE*)fopen(path, "rb");

	char type[4] = { 0, 0, 0, 0 };
	unsigned long size, chunkSize;
	short formatType, channels;
	unsigned long sampleRate, avgBytesPerSec;
	short bytesPerSample, bitsPerSample;
	unsigned long dataSize;


	fread(type, 1, 4, fp);
	if (type[0] != 'R' || type[1] != 'I' || type[2] != 'F' || type[3] != 'F')
	{
		printf("No RIFF\n");
	}
	fread(&size, 4, 1, fp);
	fread(type, 1, 4, fp);

	if (type[0] != 'W' || type[1] != 'A' || type[2] != 'V' || type[3] != 'E')
	{
		printf("Not WAVE\n");
	}
	fread(type, 1, 4, fp);
	if (type[0] != 'f' || type[1] != 'm' || type[2] != 't' || type[3] != ' ')
	{
		printf("Not fmt \n");
	}
	fread(&chunkSize, 4, 1, fp);
	fread(&formatType, 2, 1, fp);
	fread(&channels, 2, 1, fp);
	fread(&sampleRate, 4, 1, fp);
	fread(&avgBytesPerSec, 4, 1, fp);
	fread(&bytesPerSample, 2, 1, fp);
	fread(&bitsPerSample, 2, 1, fp);

	fread(type, 1, 4, fp);
	if (type[0] != 'd' || type[1] != 'a' || type[2] != 't' || type[3] != 'a')
	{
		Log::out("OpenAL", "Audio device missing 'data' (" + std::to_string(type[0]) + "" + std::to_string(type[1])
			+ "" + std::to_string(type[2]) + "" + std::to_string(type[3]) + ")", RED);
	}
	fread(&dataSize, 4, 1, fp);

	unsigned char* buf = new unsigned char[dataSize];
	fread(buf, 1, dataSize, fp);
	if (bitsPerSample == 8)
	{
		if (channels == 1)
		{
			format = AL_FORMAT_MONO8;
		}
		if (channels == 2)
		{
			format = AL_FORMAT_STEREO8;
		}
	}
	if (bitsPerSample == 16)
	{
		if (channels == 1)
		{
			format = AL_FORMAT_MONO16;
		}
		if (channels == 2)
		{
			format = AL_FORMAT_STEREO16;
		}
	}
	frequency = sampleRate;
	alBufferData(buffer, format, buf, dataSize, frequency);
	this->buf = buf;

	lengthInSamples = dataSize * 8 / (channels * bitsPerSample);
}
