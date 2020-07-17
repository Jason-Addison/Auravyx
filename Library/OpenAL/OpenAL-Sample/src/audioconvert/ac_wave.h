/* -*- mode: C; tab-width:8; c-basic-offset:8 -*-
 * vi:set ts=8:
 *
 * ac_wave.h
 *
 * Defines for RIFF WAVE files.
 */
#ifndef AL_AUDIOCONVERT_AC_WAVE_H_
#define AL_AUDIOCONVERT_AC_WAVE_H_

#include "al_siteconfig.h"
#include <AL/al.h>

#define DATA		0x61746164		/* "data" */
#define FACT		0x74636166		/* "fact" */
#define LIST		0x5453494c		/* "LIST" */
#define RIFF            0x46464952
#define WAVE            0x45564157
#define FMT             0x20746D66

int RiffOffset(ALubyte *rawdata, ALint magic);

#endif /* not AL_AUDIOCONVERT_AC_WAVE_H_ */
