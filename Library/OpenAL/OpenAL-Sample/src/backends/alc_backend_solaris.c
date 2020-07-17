/* -*- mode: C; tab-width:8; c-basic-offset:8 -*-
 * vi:set ts=8:
 *
 * solaris_native.c
 *
 * functions related to the aquisition and management of the native
 * audio on Solaris.
 *
 * This is a second-cut implementation.  Playback is well
 * supported.  Still no recording.
 *   Yotam Gingold, April 28, 2001
 *   <ygingold@cs.brown.edu>
 *   <ygingold@stat.wvu.edu>
 *
 *
 * This is a crude first-cut implementation, which doesn't do any
 * sophisticated error handling at all yet.
 *   John E. Stone, September 13, 2000
 *   <johns@megapixel.com>
 *   <j.stone@acm.org>
 */
#include "al_siteconfig.h"
#include <stdlib.h>
#include "backends/alc_backend.h"

#ifndef USE_BACKEND_NATIVE_SOLARIS

void alcBackendOpenNative_ (UNUSED(ALC_OpenMode mode), UNUSED(ALC_BackendOps **ops),
			    struct ALC_BackendPrivateData **privateData)
{
	*privateData = NULL;
}

#else

#include <AL/al.h>

#include <assert.h>
#include <stdio.h>

#include "al_main.h"
#include "al_debug.h"
#include "alc/alc_context.h"
#include "audioconvert/ac_endian.h"


#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/audioio.h>  /* Sun's audio system headers */

typedef struct {
  int fd;                /* file descriptor of audio device          */
  audio_info_t ainfo;    /* audio info structure used in ioctl calls */
  ALC_OpenMode mode;
} solaris_audio;


#define EMAIL_ADDRESS "ygingold@stat.wvu.edu"
#define EMAIL_ADDRESS2 "j.stone@acm.org"


static const char *implement_me(const char *fn) {
	static char retval[2048];

	snprintf(retval, sizeof(retval),
	"%s is not implemented under Solaris.  Please contact %s for\n"
	"information on how you can help get %s implemented on Solaris.\n",
	fn, EMAIL_ADDRESS, fn);

	return retval;
}

static void *grab_read_native(void) {
  return NULL ;
}

static void *grab_write_native(void) {
  int fd ;
  solaris_audio* saudio ;
  fprintf(stderr, "solaris_native: opening /dev/audio\n");

  fd = open("/dev/audio", O_WRONLY | O_NONBLOCK );
  if (fd < 0) {
    perror("open /dev/audio");
    return NULL;
  }

  if(fcntl(fd, F_SETFL, ~O_NONBLOCK) == -1) {
	  perror("fcntl");
	  fprintf(stderr,"fnctl error \n");
  }

  fprintf(stderr, "Opened /dev/audio successfully\n");

  saudio = (solaris_audio*) malloc( sizeof( solaris_audio ) ) ;
  if( saudio == NULL ) {
	  close( fd ) ;
	  return NULL ;
  }
  saudio -> fd = fd ;
  AUDIO_INITINFO( &(saudio->ainfo) ) ;
  saudio->mode = ALC_OPEN_OUTPUT_;

  return saudio ;
}

static void native_blitbuffer(void *handle,
		       const void *dataptr,
		       int bytesToWrite) {
	solaris_audio* sa ;

	/* arch/bsd graft */
	struct timeval tv = { 1, 0 }; /* wait 1 sec max */
	int iterator = 0;
	int err;
	fd_set sa_fd_set ;


	/*fprintf(stderr, "Writing to audio device bytesToWrite{ %d }...\n", bytesToWrite );*/

	if( handle == NULL )
		return ;
	sa = (solaris_audio*) handle ;
	if( sa->fd == NULL )
		return ;

	/* This is the original write. */
	/* write(sa->fd, (char *) dataptr, bytesToWrite);*/

	/* This is the write() adapted from arch/bsd */

	FD_SET(sa->fd, &sa_fd_set);

	for(iterator = bytesToWrite; iterator > 0; ) {
		if(select(sa->fd + 1, NULL, &sa_fd_set, NULL, &tv) == 0) {
			/* timeout occured, don't try and write */
#ifdef DEBUG_MAXIMUS
			fprintf(stderr, "native_blitbuffer: timeout occured\n");
#endif
			return;
		}

		FD_ZERO(&sa_fd_set);
		FD_SET(sa->fd, &sa_fd_set);

		assert(iterator > 0);
		assert(iterator <= bytesToWrite);

		err = write(sa->fd,
			    (char *) dataptr + bytesToWrite - iterator,
			    iterator);
		if(err < 0) {
#ifdef DEBUG_MAXIMUS
			perror("write");
#endif
			return;
		}

		iterator -= err;
	};

	return;
}

static void release_native(void *handle) {
  solaris_audio* sa ;

  fprintf(stderr, "Closing audio device...\n");

  if( handle == NULL )
	  return ;
  sa = (solaris_audio*) handle ;
  if( sa->fd == NULL )
	  return ;

  close(sa->fd);

  free( sa ) ;

  return;
}

int set_nativechannel(UNUSED(void *handle),
		      UNUSED(ALuint channel),
		      UNUSED(ALfloat volume)) {
	fprintf(stderr,  implement_me("set_nativechannel"));

	return 0;
}

static void pause_nativedevice(UNUSED(void *handle)) {
	fprintf(stderr,  implement_me("pause_nativedevice"));

	return;
}

static void resume_nativedevice(UNUSED(void *handle)) {
	fprintf(stderr,  implement_me("resume_nativedevice"));

	return;
}

static ALfloat get_nativechannel(UNUSED(void *handle), UNUSED(ALuint channel)) {
	fprintf(stderr,  implement_me("get_nativechannel"));

	return 0.0;
}

static ALsizei capture_nativedevice(UNUSED(void *handle),
			  UNUSED(void *capture_buffer),
			  UNUSED(int bytesToRead)) {
	return 0; /* unimplemented */
}

static ALboolean set_write_native(void *handle,
				  unsigned int *deviceBufferSizeInBytes,
				  ALenum *fmt,
				  unsigned int *speed) {
  solaris_audio* sa ;

  ALuint channels = _alGetChannelsFromFormat(*fmt);

  if( handle == NULL )
	  return AL_FALSE ;
  sa = (solaris_audio*) handle ;

  if( sa->fd == NULL )
	  return AL_FALSE ;

  AUDIO_INITINFO(&(sa->ainfo));
  /*if( ioctl(gaudio.fd, AUDIO_GETINFO, &(gaudio.ainfo)) < 0 )
    return AL_FALSE ; */

  fprintf(stderr, "Setting audio device...\n");
  sa->ainfo.play.sample_rate = *speed;
  sa->ainfo.play.channels = channels;
  /*fprintf(stderr, "solaris: set_write_native speed{ %d }, channels{ %d }, format{ %d }, buffer_size{ %u } \n", *speed, channels, *fmt, *deviceBufferSizeInBytes ) ;*/
  switch (*fmt) {
    case AL_FORMAT_MONO8:
    case AL_FORMAT_STEREO8:
            /*fprintf(stderr, "Setting Mono8/Stereo8... \n");*/
      sa->ainfo.play.precision = 8;
      sa->ainfo.play.encoding = AUDIO_ENCODING_LINEAR8;
      break;
    case AL_FORMAT_MONO16:
    case AL_FORMAT_STEREO16:
      sa->ainfo.play.precision = 16;
      /*#ifdef WORDS_BIGENDIAN*/
      /*fprintf( stderr, "WORDS BIGENDIAN" ) ;*/
      sa->ainfo.play.encoding = AUDIO_ENCODING_LINEAR;
      /*#else
       fprintf( stderr, "WORDS LITTLEENDIAN" ) ;
       sa->ainfo.play.encoding = AUDIO_ENCODING_LINEAR;*/
      /* #endif*/ /* WORDS_BIGENDIAN */
      break;

    default:
      fprintf(stderr, "Unsuported audio format:%d\n", *fmt);
      return AL_FALSE;
  }

  sa->ainfo.play.buffer_size = *deviceBufferSizeInBytes;

  /*if (ioctl(gaudio.fd, AUDIO_SETINFO, &gaudio.ainfo) < 0)*/
  if (ioctl(sa->fd, AUDIO_SETINFO, &(sa->ainfo)) < 0)
    return AL_FALSE;
  else
    return AL_TRUE;
}

static ALboolean set_read_native(UNUSED(void *handle),
				 UNUSED(unsigned int *deviceBufferSizeInBytes),
				 UNUSED(ALenum *fmt),
				 UNUSED(unsigned int *speed)) {
	return AL_FALSE;
}

static ALboolean
alcBackendSetAttributesNative_(void *handle, ALuint *deviceBufferSizeInBytes, ALenum *fmt, ALuint *speed)
{
	return ((solaris_audio *)handle)->mode == ALC_OPEN_INPUT_ ?
		set_read_native(handle, deviceBufferSizeInBytes, fmt, speed) :
		set_write_native(handle, deviceBufferSizeInBytes, fmt, speed);
}

static const ALCchar *get_nativename(struct ALC_BackendPrivateData *privateData)
{
    return "Solaris Native";
}

static ALC_BackendOps nativeOps = {
	release_native,
	pause_nativedevice,
	resume_nativedevice,
	alcBackendSetAttributesNative_,
	native_blitbuffer,
	capture_nativedevice,
	get_nativechannel,
	set_nativechannel,
	get_nativename
};

void
alcBackendOpenNative_ (ALC_OpenMode mode, ALC_BackendOps **ops, struct ALC_BackendPrivateData **privateData)
{
	*privateData = (mode == ALC_OPEN_INPUT_) ? grab_read_native() : grab_write_native();
	if (*privateData != NULL) {
		*ops = &nativeOps;
	}
}

#endif /* USE_BACKEND_NATIVE_SOLARIS */
