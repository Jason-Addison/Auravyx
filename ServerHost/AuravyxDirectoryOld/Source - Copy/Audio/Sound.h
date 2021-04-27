#pragma once
#include "Library/AL/al.h"
#include "Audio/WAVE.h"
#include <memory>
#include <unordered_set>

class Sound
{
public:

	ALuint source;

	float gain;
	float pitch;

	ALfloat time = 0;

	Sound();
	~Sound();

	void create();

	void play(const WAVE& wave);

	void play(const std::shared_ptr<WAVE>& wave);

	void stop();

	void setGain(const float gain);

	void setPitch(const float pitch);

	void setPosition(const float x, const float y, const float z);

	void setVelocity(const float x, const float y, const float z);

	void pause();

	void unpause();

	ALfloat getTime();

	void setTime(const ALfloat time);

	void destroy();

private:

};

