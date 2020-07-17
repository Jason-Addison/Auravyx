/* -*- mode: C; tab-width:8; c-basic-offset:8 -*-
 * vi:set ts=8:
 *
 * ac_bits.c
 *
 * audioconvert functions related to bit depth.
 */
#include "al_siteconfig.h"

#include <AL/al.h>
#include <stdio.h>

#include "audioconvert/audioconvert.h"

/* Convert 8-bit to 16-bit - LSB */
void acConvert16LSB(acAudioCVT *cvt, ALushort format, ALushort channels)
{
	int i;
	ALubyte *src, *dst;

	src = (ALubyte *) cvt->buf + cvt->len_cvt;
	dst = (ALubyte *) cvt->buf + cvt->len_cvt * 2;
	for(i = cvt->len_cvt; i; i-- ) {
		src -= 1;
		dst -= 2;
		dst[0] = 0;
		dst[1] = *src;
	}

	format = ((format & ~0x0008) | AUDIO_U16LSB);
	cvt->len_cvt *= 2;

	if ( cvt->filters[++cvt->filter_index] ) {
		cvt->filters[cvt->filter_index](cvt, format, channels);
	}
}

/* Convert 8-bit to 16-bit - MSB */
void acConvert16MSB(acAudioCVT *cvt, ALushort format, ALushort channels)
{
	int i;
	ALubyte *src, *dst;

	src = (ALubyte *) cvt->buf + cvt->len_cvt;
	dst = (ALubyte *) cvt->buf + cvt->len_cvt * 2;
	for(i = cvt->len_cvt; i; i--) {
		src -= 1;
		dst -= 2;
		dst[1] = 0;
		dst[0] = *src;
	}

	format = ((format & ~0x0008) | AUDIO_U16MSB);
	cvt->len_cvt *= 2;

	if ( cvt->filters[++cvt->filter_index] ) {
		cvt->filters[cvt->filter_index](cvt, format, channels);
	}
}

/* Convert 16-bit to 8-bit */
void acConvert8(acAudioCVT *cvt, ALushort format, ALushort channels)
{
	int i;
	ALubyte *src, *dst;

	src = cvt->buf;
	dst = cvt->buf;

	if((format & 0x1000) != 0x1000) { /* Little endian */
		++src;
	}

	for(i = cvt->len_cvt/2; i; i--) {
		*dst = *src;
		src += 2;
		dst += 1;
	}
	format = ((format & ~0x9010) | AUDIO_U8);

	cvt->len_cvt /= 2;

	if ( cvt->filters[++cvt->filter_index] ) {
		cvt->filters[cvt->filter_index](cvt, format, channels);
	}
}
