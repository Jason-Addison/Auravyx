#include "Auravyx/Audio/Sound.h"
#include "AL/efx.h"
#include <Auravyx/Audio/SoundManager.h>
#include <iostream>
Sound::Sound()
{
	//create();
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

	SoundManager::getSoundManager()->addSound(source);
}

void Sound::play(const WAVE& wave)
{
	alSourcei(source, AL_BUFFER, wave.buffer);
	alSourcePlay(source);
}

void Sound::play(const std::shared_ptr<WAVE>& wave)
{
	if (wave)
	{
		alSourcei(source, AL_BUFFER, wave->buffer);
		alSourcePlay(source);
	}
}

void Sound::stop()
{
	alSourceStop(source);
}

void Sound::setGain(const float gain)
{
	this->gain = gain;
	alSourcef(source, AL_GAIN, gain);
}

void Sound::setPitch(const float pitch)
{
	this->pitch = pitch;
	alSourcef(source, AL_PITCH, pitch);
}

void Sound::setPosition(const float x, const float y, const float z)
{
	alSource3f(source, AL_POSITION, x, y, z);
}

void Sound::setVelocity(const float x, const float y, const float z)
{
	alSource3f(source, AL_VELOCITY, x, y, z);
}

void Sound::pause()
{
	stop();
	time = getTime();
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

void Sound::setTime(const ALfloat time)
{
	alSourcef(source, AL_SEC_OFFSET, time);
}

void Sound::destroy()
{
	SoundManager::getSoundManager()->removeSound(source);
	alDeleteSources(1, &source);
}
