#include "stdafx.h"
#include "Sound.h"
#include "efx.h"
#include <memory>

Sound::Sound()
{
	create();
}


Sound::~Sound()
{
}

void Sound::create()
{
	alGenSources((ALuint)1, &source);

	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_TRUE);
	alSourcei(source, AL_AIR_ABSORPTION_FACTOR, 100);
}

void Sound::play(WAVE wave)
{
	alSourcei(source, AL_BUFFER, wave.buffer);
	alSourcePlay(source);
}

void Sound::play(std::shared_ptr<WAVE> wave)
{
	alSourcei(source, AL_BUFFER, wave->buffer);
	alSourcePlay(source);
}

void Sound::stop()
{
	alSourceStop(source);
}

void Sound::setGain(float gain)
{
	this->gain = gain;
	alSourcef(source, AL_GAIN, gain);
}

void Sound::setPitch(float pitch)
{
	this->pitch = pitch;
	alSourcef(source, AL_PITCH, pitch);
}

void Sound::setPosition(float x, float y, float z)
{
	alSource3f(source, AL_POSITION, x, y, z);
}

void Sound::setVelocity(float x, float y, float z)
{
	alSource3f(source, AL_VELOCITY, x, y, z);
}

void Sound::pause()
{
	//time = getTime();
}

void Sound::unpause()
{
}

ALfloat Sound::getTime()
{
	ALfloat i;
	alGetSourcef(source, AL_SEC_OFFSET, &i);
	return i;
}

void Sound::setTime(ALfloat time)
{
	alSourcef(source, AL_SEC_OFFSET, time);
}
