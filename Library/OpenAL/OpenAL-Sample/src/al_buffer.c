/* -*- mode: C; tab-width:8; c-basic-offset:8 -*-
 * vi:set ts=8:
 *
 * al_buffer.c
 *
 * Stuff related to the management and use of buffers.
 *
 */
#include "al_siteconfig.h"

#include <AL/al.h>

/* for alutLoadVorbis_LOKI and alBufferAppendData_LOKI */
#include <AL/alext.h>

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "al_buffer.h"
#include "al_bpool.h"
#include "al_debug.h"
#include "al_error.h"
#include "al_main.h"
#include "al_source.h"
#include "al_types.h"
#include "alc/alc_context.h"
#include "alc/alc_speaker.h"

#include "audioconvert/audioconvert.h"

#include "al_threadlib.h"
#include "al_mutexlib.h"

#ifndef elementsof
#define elementsof(a) ((sizeof(a)) / (sizeof *(a)))
#endif

/*
 * buf_pool is a the bpool_t structure which we use to slab allocate
 * AL_buffers.
 */
static bpool_t buf_pool;

/*
 * Mutex guarding buf_pool.
 */
static MutexID buf_mutex;

/*
 * _alDestroyBuffer(void *buf)
 *
 * _alDestroyBuffer is passed an AL_buffer pointer, masquerading as a void
 * pointer, and frees the data structures internal to the AL_buffer, but
 * not the buffer itself.
 *
 * This is called by bpool_dealloc.
 */
static void _alDestroyBuffer( void *buf );

/*
 * ALvoid *_alConvert( const ALvoid *data,
 *			ALenum f_format, ALuint f_size, ALuint f_freq,
 *			ALenum t_format, ALuint t_freq, ALuint *retsize,
 *			ALenum should_use_passed_data);
 *
 * _alConvert takes the passed data and converts it from it's current
 * format (f_format) to the desired format (t_format), etc, returning
 * the converted data and setting retsize to the new size of the
 * converted data.  The passed data must either be raw PCM data or
 * must correspond with one of the headered extension formats.
 *
 * If should_use_passed_data is set to AL_TRUE, then _alConvert will
 * attempt to do the conversion in place.  Otherwise, new data will
 * be allocated for the purpose.
 *
 * Returns NULL on error.
 */
static ALvoid *_alConvert(const ALvoid *data,
			ALenum f_format, ALuint f_size, ALuint f_freq,
			ALenum t_format, ALuint t_freq, ALuint *retsize,
			ALenum should_use_passed_data);

/*
 * _alBufferDestroyCallbackBuffer( AL_buffer *buf )
 *
 * Executes the buffer completion callback associated with buf, if there is
 * one.
 */
static void _alBufferDestroyCallbackBuffer( AL_buffer *buf );

/*
 * buffer source state
 */

/*
 * _alBufferAddQueueRef( AL_buffer *buf, ALuint sid )
 *
 * adds a queue reference to the AL_buffer *buf.  The queue reference
 * refers to the source named by sid.
 *
 * If no current reference is added, and this queue reference is not deleted,
 * _alGet{Bid,Buffer}State will return AL_PENDING.
 *
 */
static void _alBufferAddQueueRef( AL_buffer *buf, ALuint sid );

/*
 * _alBufferRemoveQueueRef( AL_buffer *buf, ALuint sid )
 *
 * removes a queue reference to the AL_buffer *buf.  The first queue
 * reference refering to sid will be removed.
 */
static void _alBufferRemoveQueueRef( AL_buffer *buf, ALuint sid );

/*
 * _alBufferAddCurrentRef( AL_buffer *buf, ALuint sid )
 *
 * adds a current reference to the AL_buffer *buf.  The reference refers
 * to the source named by sid.
 *
 * If this reference is not removed, _alGet{Bid,Buffer}state will return
 * AL_PROCESSED.
 */
static void _alBufferAddCurrentRef( AL_buffer *buf, ALuint sid );

/*
 * _alBufferRemoveCurrentRef( AL_buffer *buf, ALuint sid )
 *
 * removes a current reference to the AL_buffer *buf.  The first current
 * reference refering to sid will be removed.
 */
static void _alBufferRemoveCurrentRef( AL_buffer *buf, ALuint sid );

/*
 * alGenBuffers( ALsizei n, ALuint *buffer )
 *
 * Perform full allocation of n-1 buffer ids.  Fails and nops
 * if n-1 buffers could not be created.
 *
 * If n is 0, legal nop.  If n < 0, set INVALID_VALUE and nop.
 */
void alGenBuffers( ALsizei n, ALuint *buffer ) {
	ALuint *temp;
	int bindex;
	int i;

	if(n == 0) {
		return; /* silently return */
	}

	if(n < 0) {
		_alDebug(ALD_BUFFER, __FILE__, __LINE__,
		      "alGenBuffers: invalid n %d\n", n);

		_alcDCLockContext();
		_alDCSetError( AL_INVALID_VALUE );
		_alcDCUnlockContext();
		return;
	}

	temp = malloc(n * sizeof *temp);
	if(temp == NULL) {
		/*
		 * Could not reserve memory for temporary
		 * ALuint *buffer.
		 */
		_alcDCLockContext();
		_alDCSetError( AL_OUT_OF_MEMORY );
		_alcDCUnlockContext();
		return;
	}

	_alLockBuffer();

	for(i = 0; i < n; i++) {
		bindex = bpool_alloc(&buf_pool);

		if(bindex == -1) {
			/*
			 * Could not honor request in full.  We
			 * unlock, dealloc, set error, return.
			 *
			 * FIXME: Should we have a non-locking
			 * version of DeleteBuffers and maintain this
			 * lock as long as possible?
			 */
			_alUnlockBuffer();

			if(i > 0) {
				/*
				 * Only delete buffers that have
				 * been created.
				 */
				alDeleteBuffers(i, temp);
			}

			_alcDCLockContext();
			_alDCSetError(AL_OUT_OF_MEMORY);
			_alcDCUnlockContext();

			free( temp );

			return;
		}

		temp[i] = bindex;
	}

	_alUnlockBuffer();

	/*
	 * temp[0..n-1] now populated with n valid buffer names,
	 * so copy it.
	 */
	memcpy( buffer, temp, n * sizeof *buffer );

	free( temp );

	return;
}

/*
 * alDeleteBuffers( ALsizei n, ALuint *buffers )
 *
 * Perform full deallocation of buffers[0..n-1].  If a member
 * of buffers[0..n-1] is not a valid buffer id, set INVALID_NAME
 * and return without deallocating any member.
 *
 * If n is 0, legal nop.  If n < 0, set INVALID_VALUE and nop.
 *
 * FIXME: not well tested
 *
 * Well, that's not totally true.  I've tested deleting
 * buffers (obviously!) but there's a whole set of mojo where it
 * becomes possible to delete buffers in playing sources, which
 * is bad bad bad.
 *
 * The ref counting system is made to avoid that bad mojo by never
 * deleting buffers associated with playing sources.  *That* is
 * what is not well tested.
 *
 * Of course, the behaviour of deleting buffers which are currently
 * associated with sources (especially playing sources!) needs to
 * be specified.  It's not at-the-moment.
 */
void alDeleteBuffers( ALsizei n, const ALuint* buffers ) {
	AL_buffer *buf;
	ALenum bufstate;
	int i;

	if(n == 0) {
		/* silently return */
		return;
	}

	_alLockBuffer();

	if(n < 0) {
		_alUnlockBuffer();

		_alcDCLockContext();
		_alDCSetError( AL_INVALID_VALUE );
		_alcDCUnlockContext();

		return;
	}

	/*
	 * test each buffer to ensure we don't have any
	 * invalid names in there.
	 */
	for(i = 0; i < n; i++) {
		if(_alIsBuffer(buffers[i]) == AL_FALSE) {
			/* not a buffer */
			_alcDCLockContext();
			_alDCSetError( AL_INVALID_NAME );
			_alcDCUnlockContext();

			_alUnlockBuffer();
			return;
		}
	}

	while(n--) {
		bufstate = _alGetBidState( buffers[n] );

		if(bufstate == AL_UNUSED) {
			bpool_dealloc(&buf_pool, buffers[n],
				_alDestroyBuffer);
		} else {
			buf = _alGetBuffer( buffers[n] );
			if(buf == NULL) {
				/* should never happen */
				_alcDCLockContext();
				_alDCSetError( AL_INVALID_NAME );
				_alcDCUnlockContext();

				continue;
			}

			/* still in use */
			buf->flags |= ALB_PENDING_DELETE;
		}
	}
	_alUnlockBuffer();

	return;
}

/*
 * alIsBuffer( ALuint bid )
 *
 * Returns AL_TRUE if bid is a valid buffer name, AL_FALSE otherwise.
 *
 */
ALboolean alIsBuffer( ALuint bid ) {
	ALboolean retval;

	_alLockBuffer();

	retval = _alIsBuffer( bid );

	_alUnlockBuffer();

	return retval;
}

/*
 * alBufferData( ALuint  bid,
 *               ALenum  format,
 *               ALvoid  *data,
 *               ALsizei size,
 *               ALsizei freq );
 *
 * associates data with bid.
 *
 * If format is invalid, set AL_INVALID_ENUM.  If bid is not a valid buffer
 * name, set AL_INVALID_NAME.  If not enough memory is available to make a
 * copy of this data, set AL_OUT_OF_MEMORY.
 */
void alBufferData( ALuint  bid,
		   ALenum  format,
                   const ALvoid* data,
		   ALsizei size,
		   ALsizei freq ) {
	AL_buffer *buf;
	ALvoid *cdata;
	ALuint i;
	ALuint retsize;
	ALenum tformat;
	ALint tfreq;
	ALubyte tchannels;

	if((data == NULL) || (size == 0))
	{
		_alcDCLockContext();
		_alDCSetError(AL_INVALID_VALUE);
		_alcDCUnlockContext();

		return;
	}

	switch(format) {
#ifdef ENABLE_EXTENSION_AL_EXT_VORBIS
		/*
		 * If compiled with the vorbis extension, and we get passed
		 * vorbis data, then pass that to the correct extension.
		 */
		case AL_FORMAT_VORBIS_EXT:
			if(alutLoadVorbis_LOKI(bid, data, size) == AL_FALSE) {
				_alcDCLockContext();
				_alDCSetError(AL_INVALID_OPERATION);
				_alcDCUnlockContext();
			}
			return;
			break;
#endif /* ENABLE_EXTENSION_AL_EXT_VORBIS */
		case AL_FORMAT_WAVE_EXT:
		case AL_FORMAT_MONO8:
		case AL_FORMAT_MONO16:
		case AL_FORMAT_STEREO8:
		case AL_FORMAT_STEREO16:
		case AL_FORMAT_QUAD8_LOKI:
		case AL_FORMAT_QUAD16_LOKI:
		case AL_FORMAT_IMA_ADPCM_MONO16_EXT:
		case AL_FORMAT_IMA_ADPCM_STEREO16_EXT:
			break;
		default:
			_alDebug(ALD_BUFFER, __FILE__, __LINE__,
				"alBufferData: unknown format 0x%x", format);
			_alcDCLockContext();
			_alDCSetError(AL_INVALID_VALUE);
			_alcDCUnlockContext();
			return;
			break;
	}

	_alLockBuffer();

	buf = _alGetBuffer(bid);
	if(buf == NULL) {
		_alDebug(ALD_BUFFER, __FILE__, __LINE__,
		      "alBufferData: buffer id %d not valid",
		      bid);

		_alcDCLockContext();
		_alDCSetError(AL_INVALID_NAME);
		_alcDCUnlockContext();

		_alUnlockBuffer();
		return;
	}

	if(buf->flags & ALB_STREAMING) {
		/* Streaming buffers cannot use alBufferData */
		_alcDCLockContext();
		_alDCSetError(AL_INVALID_OPERATION);
		_alcDCUnlockContext();

		_alUnlockBuffer();

		return;
	}

	if(buf->flags & ALB_CALLBACK) {
		/* If this was previously a callback buffer,
		 * reset it.
		 */
		buf->flags &= ~ALB_CALLBACK;
	}

	/* Convert 8-bit formats to 16-bit, but leave the channel count alone */
	if(format == AL_FORMAT_MONO8)
		tformat = AL_FORMAT_MONO16;
	else if(format == AL_FORMAT_STEREO8)
		tformat = AL_FORMAT_STEREO16;
	else if(format == AL_FORMAT_QUAD8_LOKI)
		tformat = AL_FORMAT_QUAD16_LOKI;
	else
		tformat = format;
	tfreq = buf->frequency;

	_alUnlockBuffer();

	cdata = _alBufferCanonizeData(format,
				      data,
				      size,
				      freq,
				      tformat,
				      tfreq,
				      &retsize,
				      AL_FALSE);

	if(cdata == NULL) {
		/*  _alBufferCanonize Data should set error */
		return;
	}

	/*
	 * alter buffer's data.
	 */
	_alLockBuffer();

	if(buf->size < retsize)
	{
		void *temp_copies[_ALC_MAX_CHANNELS] = { NULL };
		ALubyte channels;

		/* don't use realloc */
		_alBufferFreeOrigBuffers(buf);

		channels = _alGetChannelsFromFormat(tformat);

		for(i = 0; i < channels; i++)
		{
			temp_copies[i] = malloc(retsize);
			if (!temp_copies[i])
				break;
		}

		if(i != channels)
		{
			int j;
			free(cdata);

			for(j = 0; j < i; j++)
				free(temp_copies[j]);

			/* JIV FIXME: lock context */
			_alcDCLockContext();
			_alDCSetError(AL_OUT_OF_MEMORY);
			_alcDCUnlockContext();

			_alUnlockBuffer();

			return;
		}

		switch(channels)
		{
			case 1:
			  for(i = 0; i < elementsof(buf->orig_buffers); i++)
			  {
				  buf->orig_buffers[i] = temp_copies[0];
			  }

			  break;
			case 2:
			  for(i = 0; i < elementsof(buf->orig_buffers); i += 2)
			  {
				  buf->orig_buffers[i]   = temp_copies[0];
				  buf->orig_buffers[i+1] = temp_copies[1];
			  }

			  break;
			case 4:
			  assert(elementsof(buf->orig_buffers) >= 4);
			  for(i = 0; i < elementsof(buf->orig_buffers); i += 4)
			  {
				  buf->orig_buffers[i]   = temp_copies[0];
				  buf->orig_buffers[i+1] = temp_copies[1];
				  buf->orig_buffers[i+2] = temp_copies[2];
				  buf->orig_buffers[i+3] = temp_copies[3];
			  }
			  break;
			case 6:
			  assert(elementsof(buf->orig_buffers) >= 6);
			  for(i = 0; i < elementsof(buf->orig_buffers); i += 6)
			  {
				  buf->orig_buffers[i]   = temp_copies[0];
				  buf->orig_buffers[i+1] = temp_copies[1];
				  buf->orig_buffers[i+2] = temp_copies[2];
				  buf->orig_buffers[i+3] = temp_copies[3];
				  buf->orig_buffers[i+4] = temp_copies[4];
				  buf->orig_buffers[i+5] = temp_copies[5];
			  }

			  break;
			default:
			  /* well this is weird */
			  assert(0);
			  break;
		}
	}

	tchannels = _alGetChannelsFromFormat(tformat);
	_alMonoify((ALshort **) buf->orig_buffers,
		   cdata,
		   retsize / tchannels,
	       buf->num_buffers, tchannels);

	buf->size = retsize / tchannels;
	buf->format = tformat;

	_alUnlockBuffer();

	free(cdata);

	return;
}

/*
 * _alIsBuffer( ALuint bid )
 *
 * Non locking version of alIsBuffer.
 *
 * assumes locked buffers
 */
ALboolean _alIsBuffer( ALuint bid ) {
	AL_buffer *buf;
	ALboolean retval = AL_TRUE;

	buf = _alGetBuffer( bid );
	if ( !buf || (buf->flags & ALB_PENDING_DELETE) ) {
		retval = AL_FALSE;
	}

	return retval;
}

/*
 *_alGetBuffer( ALuint bid ).
 *
 * Returns pointer to the AL_buffer with buffer name bid, or NULL if bid in
 * invalid.
 *
 * assumes that buffers are locked
 */
AL_buffer *_alGetBuffer( ALuint bid ) {
	int bindex;

	bindex = bpool_bid_to_index( &buf_pool, bid );
	if(bindex < 0) {
		/* invalid bid */
		return NULL;
	}

	if(bindex >= (int) buf_pool.size) {
		/* buffer id too big */
		return NULL;
	}

	if(buf_pool.pool[bindex].inuse == AL_FALSE) {
		return NULL;
	}

	return bpool_index(&buf_pool, bid);
}

/*
 * _alInitBuffers( void )
 *
 * Performs global initialization of buffer specific data
 * structures.
 *
 * Doesn't do much.  No default size, so we just initialize
 * the mutex.
 */
ALboolean _alInitBuffers( void )
{
	buf_mutex = _alCreateMutex();

	return AL_TRUE;
}

/*
 * _alGetBufferFromSid( ALuint cid, ALuint sid )
 *
 * Retrieves a pointer to the AL_buffer (not the buffer id) associated with
 * the source with name sid ( from context with name cid), or NULL if cid, sid
 * are invalid or if sid does not have its buffer attribute set.
 *
 * assumes locked context
 */
AL_buffer *_alGetBufferFromSid( ALuint cid, ALuint sid ) {
	AL_buffer *retval;
	AL_source *src;
	ALuint *buffid;

	src = _alGetSource( cid, sid );
	if(src == NULL) {
		return NULL;
	}

	buffid = _alGetSourceParam( src, AL_BUFFER );
	if( buffid == NULL ) {
		return NULL;
	}

	_alLockBuffer();

	retval = _alGetBuffer( *buffid );

	_alUnlockBuffer();

	return retval;
}

/*
 * ALvoid *_alBufferCanonizeData( ALenum format,
 *                              const ALvoid *data, ALuint size, ALuint freq,
 *                              ALenum t_format, ALuint t_freq,
 *                              ALuint *retsize,
 *                              ALenum should_use_passed_data )
 *
 * Put data in canonical format, setting retsize and returning a newly
 * alloced memory.
 *
 * If should_use_passed_data is set to AL_TRUE, the data will
 * be converted (if possible) in place.
 *
 * Returns NULL on error.
 *
 * assumes locked buffers.
 */
ALvoid *_alBufferCanonizeData( ALenum format,
			     const ALvoid *data, ALuint size, ALuint freq,
			     ALenum t_format, ALuint t_freq,
			     ALuint *retsize,
			     ALenum should_use_passed_data ) {
	if(format < 0) {
		return NULL;
	}

	return _alConvert(data, format, size, freq, t_format, t_freq,
			  retsize, should_use_passed_data);
}

/*
 * _alDestroyBuffer( void *bufp )
 *
 * _alDestroyBuffer is passed an AL_buffer pointer, masquerading as a void
 * pointer, and frees the data structures internal to the AL_buffer, but
 * not the buffer itself.
 *
 * This is called by bpool_dealloc.  It shouldn't be called my anything else.
 */
static void _alDestroyBuffer( void *bufp ) {
	AL_buffer *buf = (AL_buffer *) bufp;

	if(_alBufferIsCallback( buf ) == AL_TRUE) {
		/*
	 	 * alut decoders need to be informed of
	 	 * buffer destruction
	 	 */
		_alBufferDestroyCallbackBuffer(buf);
		buf->destroy_buffer_callback = NULL;
	}

	_alBufferFreeOrigBuffers(buf);

	free( buf->queue_list.sids );
	free( buf->current_list.sids );

	buf->queue_list.sids = NULL;
	buf->current_list.sids = NULL;

	buf->queue_list.size   = buf->queue_list.items = 0;
	buf->current_list.size = buf->current_list.items = 0;

	/* don't free bufp, let the caller do it if needed */

	return;
}

/*
 * _alDestroyBuffers( void )
 *
 *  Destroy all buffers, freeing all data associated with
 *  each buffer.
 */
void _alDestroyBuffers( void ) {
	bpool_free( &buf_pool, _alDestroyBuffer );
	bpool_init( &buf_pool );

	_alDestroyMutex( buf_mutex );

	buf_mutex = NULL;

	return;
}

/*
 * FL_alLockBuffer(UNUSED(const char *fn), UNUSED(int ln))
 *
 * Locks the buffer mutex, passing fn and ln to _alLockPrintf for debugging
 * purposes.
 */
ALboolean FL_alLockBuffer(UNUSED(const char *fn), UNUSED(int ln)) {
	_alLockPrintf("_alLockBuffer", fn, ln);

	if( buf_mutex == NULL ) {
		return AL_FALSE;
	}

	_alLockMutex( buf_mutex );

	return AL_TRUE;
}

/*
 * FL_alUnlockBuffer(UNUSED(const char *fn), UNUSED(int ln))
 *
 * Unlocks the buffer mutex, passing fn and ln to _alLockPrintf for debugging
 * purposes.
 */
ALboolean FL_alUnlockBuffer(UNUSED(const char *fn), UNUSED(int ln)) {
	_alLockPrintf("_alUnlockBuffer", fn, ln);

	if(buf_mutex == NULL) {
		return AL_FALSE;
	}

	_alUnlockMutex(buf_mutex);

	return AL_TRUE;
}

/*
 * _alNumBufferHint( ALuint nb )
 *
 * Resize buf_pool to contain space for at least nb buffers.  A performance
 * hint best excercised before creating buffers.
 */
void _alNumBufferHint( ALuint nb ) {
	_alLockBuffer();

	bpool_resize( &buf_pool, nb );

	_alUnlockBuffer();

	return;
}

/*
 * _alBidIsStreaming( ALuint bid )
 *
 * Returns AL_TRUE if the buffer named by bid was generated by
 * alGenStreamingBuffers_LOKI, AL_FALSE otherwise.
 *
 * assumes locked buffer
 */
ALboolean _alBidIsStreaming( ALuint bid ) {
	AL_buffer *buf;
	ALboolean retval = AL_FALSE;

	buf = _alGetBuffer( bid );
	if(buf != NULL) {
		if(buf->flags & ALB_STREAMING) {
			retval = AL_TRUE;
		}
	}

	return retval;
}

/*
 * _alBidIsCallback( ALuint bid )
 *
 * Returns AL_TRUE if the buffer named by bid has been used with
 * alBufferDataWithCallback_LOKI, AL_FALSE if bid does not refer to a valid
 * bid or has not been used with such a call.
 *
 * assumes locked buffer
 */
ALboolean _alBidIsCallback( ALuint bid ) {
	AL_buffer *buf;

	buf = _alGetBuffer(bid);

	return _alBufferIsCallback(buf);
}

/*
 * _alBufferIsCallback( AL_buffer *buf )
 *
 * Returns AL_TRUE if the AL_buffer *buf been used with
 * alBufferDataWithCallback_LOKI, AL_FALSE otherwise.
 *
 * assumes locked buffer
 */
ALboolean _alBufferIsCallback( AL_buffer *buf ) {
	ALboolean retval = AL_FALSE;

	if(buf != NULL) {
		if(buf->flags & ALB_CALLBACK) {
			retval = AL_TRUE;
		}
	}

	return retval;
}

/*
 * _alBufferDataWithCallback_LOKI( ALuint bid,
 *                                 int (*callback)( ALuint sid,
 *                                                  ALuint bid,
 *                                 ALshort *outdata,
 *                                 ALenum format,
 *                                 ALint freq,
 *                                 ALint samples ),
 *                                 DestroyCallback_LOKI d_sid,
 *                                 DestroyCallback_LOKI d_bid )
 *
 * Associates the AL_buffer named by bid with a callback.  This is somewhat
 * equivilant to calling alBufferData( bid, ... ), except that instead of
 * getting all the data at once, whenever the buffer is required to provide
 * data, it relies on the callback.
 *
 * The destroyer callbacks are used to update data structures.  They are not
 * available publically, and are only used by internal functions ( in alut,
 * mostly ).
 */
void _alBufferDataWithCallback_LOKI( ALuint bid,
				     int (*callback)( ALuint sid,
						      ALuint bid,
						      ALshort *outdata,
						      ALenum format,
						      ALint freq,
						      ALint samples ),
				     DestroyCallback_LOKI d_sid,
				     DestroyCallback_LOKI d_bid ) {
	AL_buffer *buf;

	_alLockBuffer();
	buf = _alGetBuffer( bid );

	if(buf == NULL) {
		/* bid was invalid */
		_alDebug(ALD_BUFFER, __FILE__, __LINE__,
			"Invalid buffer id %d", bid);

		_alcDCLockContext();
		_alDCSetError( AL_INVALID_NAME );
		_alcDCUnlockContext();

		_alUnlockBuffer();

		return;
	}

	_alBufferFreeOrigBuffers(buf);


	buf->size     = 0;
	buf->callback = callback;
	buf->flags    |= ALB_CALLBACK;

	buf->destroy_buffer_callback = d_bid;
	buf->destroy_source_callback = d_sid;

	_alUnlockBuffer();

	return;
}

/*
 * _alBufferDestroyCallbackBuffer( AL_buffer *buf )
 *
 * Executes the buffer completion callback associated with buf, if there is
 * one.
 *
 * assumes locked buffers
 */
static void _alBufferDestroyCallbackBuffer( AL_buffer *buf ) {
	if(buf == NULL) {
		return;
	}

	if(buf->destroy_buffer_callback != NULL) {
		buf->destroy_buffer_callback(buf->bid);
	}

	return;
}

/*
 * _alBidCallDestroyCallbackSource( ALuint sid )
 *
 * Executes the source completion callback from the buffer associated with
 * sid, if there is one.
 *
 * assumes locked context
 */
void _alBidCallDestroyCallbackSource( ALuint sid ) {
	AL_buffer *buf;
	AL_source *src;
	ALuint *bid;

	src = _alDCGetSource( sid );
	if(src == NULL) {
		return;
	}

	bid = _alGetSourceParam( src, AL_BUFFER );
	if(bid == NULL) {
		return;
	}

	_alLockBuffer();

	buf = _alGetBuffer( *bid );
	if(buf == NULL) {
		_alUnlockBuffer();
		return;
	}

	if(buf->destroy_source_callback != NULL) {
		buf->destroy_source_callback( sid );
	}

	_alUnlockBuffer();

	return;
}

/*
 * ALvoid *_alConvert( const ALvoid *data,
 *			ALenum f_format, ALuint f_size, ALuint f_freq,
 *			ALenum t_format, ALuint t_freq, ALuint *retsize,
 *			ALenum should_use_passed_data);
 *
 * _alConvert takes the passed data and converts it from it's current
 * format (f_format) to the desired format (t_format), etc, returning
 * the converted data and setting retsize to the new size of the
 * converted data.  The passed data must either be raw PCM data or
 * must correspond with one of the headered extension formats.
 *
 * If should_use_passed_data is set to AL_TRUE, then _alConvert will
 * attempt to do the conversion in place.  Otherwise, new data will
 * be allocated for the purpose.
 *
 * Returns NULL on error.
 *
 * assumes locked buffers
 */
static ALvoid *_alConvert( const ALvoid *data,
			 ALenum f_format, ALuint f_size, ALuint f_freq,
			 ALenum t_format, ALuint t_freq, ALuint *retsize,
			 ALenum should_use_passed_data ) {
	ALvoid *compressed = NULL;
	ALvoid *retval = NULL;
	acAudioCVT s16le;

	if((f_format == t_format) && (f_freq == t_freq)) {
		/*
		 * no conversion needed.
		 */
		*retsize = f_size;

		if( should_use_passed_data == AL_TRUE ) {
			_alDebug(ALD_CONVERT, __FILE__, __LINE__,
				 "_alConvert: no conversion needed: %p", data);

			return (ALvoid *)data;
		}

		retval = malloc( f_size );
		if( retval == NULL ) {
			_alcDCLockContext();
			_alDCSetError( AL_OUT_OF_MEMORY );
			_alcDCUnlockContext();

			return NULL;
		}

		memcpy( retval, data, f_size );

		return retval;
	}

	/*
	 * Compressed auto formats like IMA_ADPCM get converted in
	 * full here.
	 */
	if(_al_RAWFORMAT(f_format) == AL_FALSE) {
		ALushort acfmt;
		ALushort achan;
		ALushort acfreq;

		switch(f_format) {
			case AL_FORMAT_IMA_ADPCM_MONO16_EXT:
			case AL_FORMAT_IMA_ADPCM_STEREO16_EXT:
			case AL_FORMAT_WAVE_EXT:
				acLoadWAV(data, &f_size, &retval, &acfmt, &achan, &acfreq);

				f_format = _al_AC2ALFMT(acfmt, achan);
				f_freq   = acfreq;
				break;
			default:
			break;
		}

		data = compressed = retval;
	}

	_alDebug(ALD_CONVERT, __FILE__, __LINE__,
		"_alConvert [f_size|f_channels|f_freq] [%d|%d|%d]",
		f_size, _alGetChannelsFromFormat(f_format), f_freq);

	if(_alGetChannelsFromFormat(f_format) != 0) {
		_alDebug(ALD_CONVERT, __FILE__, __LINE__,
			"_alConvert [t_channels|f_channels|t/f] [%d|%d|%d]",
			_alGetChannelsFromFormat(t_format),
			_alGetChannelsFromFormat(f_format),
			_alGetChannelsFromFormat(t_format) /
			_alGetChannelsFromFormat(f_format));
	}

	if(f_freq != 0) {
		_alDebug(ALD_CONVERT, __FILE__, __LINE__,
			"_alConvert [t_freq|f_freq|t/f] [%d|%d|%d]",
			t_freq, f_freq, t_freq/f_freq);
	}

	if(f_format != 0) {
		_alDebug(ALD_CONVERT, __FILE__, __LINE__,
			"_alConvert [t_bits|f_bits|t/f] [%d|%d|%d]",
			_alGetBitsFromFormat(t_format),
			_alGetBitsFromFormat(f_format),
			(_alGetBitsFromFormat(t_format) / _alGetBitsFromFormat(f_format)));
	}

	_alDebug(ALD_CONVERT, __FILE__, __LINE__,
		"_alConvert f|c|s [0x%x|%d|%d] -> [0x%x|%d|%d]",
		/* from */
		f_format, _alGetChannelsFromFormat(f_format), f_freq,
		/* to */
		t_format,
		_alGetChannelsFromFormat(t_format),
		t_freq);

	if(acBuildAudioCVT(&s16le,
		/* from */
		_al_AL2ACFMT(f_format),
		_alGetChannelsFromFormat(f_format),
		f_freq,

		/* to */
		_al_AL2ACFMT(t_format),
		_alGetChannelsFromFormat(t_format),
		t_freq) < 0) {
		_alDebug(ALD_CONVERT, __FILE__, __LINE__,
			"Couldn't build audio convertion data structure.");

		free(compressed);

		return NULL;
	}

	_alDebug(ALD_CONVERT, __FILE__, __LINE__,
		"_alConvert [len|newlen] [%d|%d]",
		f_size, f_size * s16le.len_mult);

	if(should_use_passed_data == AL_TRUE) {
		_alDebug(ALD_CONVERT, __FILE__, __LINE__,
			"Converting with passed data = %p", data);
		_alDebug(ALD_CONVERT, __FILE__, __LINE__,
			"len_multi = %d", s16le.len_mult);

		s16le.buf = retval = (ALvoid *)data;
	} else {
		/* alloc space for buffer if we aren't using the original */

		s16le.buf = retval = malloc(f_size * s16le.len_mult);
		if(retval == NULL) {
			_alDCSetError(AL_OUT_OF_MEMORY);

			free( compressed );
			return NULL;
		}
		memcpy(retval, data, f_size);
	}

	s16le.len = f_size;

	if(acConvertAudio(&s16le) < 0) {
		_alDebug( ALD_CONVERT, __FILE__, __LINE__,
			"Couldn't execute conversion into canon.");

		free( compressed );

		return NULL;
	}

	/* set return size */
	*retsize = s16le.len_cvt;

	if( s16le.buf != compressed ) {
		/*
		 * this comparison is false iff we are using a
		 * compressed/headered format that requires us to allocate
		 * a temporary buffer.
		 */
		free( compressed );
	}

	return s16le.buf;
}

/*
 * _alBidRemoveQueueRef( ALuint bid, ALuint sid )
 *
 * removes a queue reference to the buffer named by bid.  The first queue
 * reference refering to sid will be removed.
 *
 */
void _alBidRemoveQueueRef( ALuint bid, ALuint sid ) {
	AL_buffer *buf;

	_alLockBuffer();

	buf = _alGetBuffer( bid );
	if( buf == NULL ) {
		/* invalid name */
		_alUnlockBuffer();
		return;
	}

	_alBufferRemoveQueueRef( buf, sid );

	if( buf->flags & ALB_PENDING_DELETE ) {
		/* do deffered deletion */
		if( _alGetBidState(bid) == AL_UNUSED ) {
			bpool_dealloc(&buf_pool, bid, _alDestroyBuffer);
		}
	}

	_alUnlockBuffer();

	return;
}

/*
 * _alBidRemoveCurrentRef( ALuint bid, ALuint sid )
 *
 * removes a current reference to the buffer named by bid.  The first current
 * reference refering to sid will be removed.
 */
void _alBidRemoveCurrentRef(ALuint bid, ALuint sid) {
	AL_buffer *buf;

	_alLockBuffer();

	buf = _alGetBuffer(bid);
	if(buf == NULL) {
		/* invalid name */
		_alUnlockBuffer();
		return;
	}

	_alBufferRemoveCurrentRef( buf, sid );

	if( buf->flags & ALB_PENDING_DELETE ) {
		/* do deffered deletion */
		if( _alGetBidState(bid) == AL_UNUSED ) {
			bpool_dealloc(&buf_pool, bid, _alDestroyBuffer);
		}
	}

	_alUnlockBuffer();

	return;
}

/*
 * _alBidAddQueueRef( ALuint bid, ALuint sid )
 *
 * adds a queue reference to the buffer named by bid.  The queue reference
 * refers to the source named by sid.
 *
 * If no current reference is added, and this queue reference is not deleted,
 * _alGet{Bid,Buffer}State will return AL_PENDING.
 *
 */
void _alBidAddQueueRef(ALuint bid, ALuint sid) {
	AL_buffer *buf;

	_alLockBuffer();

	buf = _alGetBuffer( bid );
	if(buf == NULL) {
		/* invalid name, set error elsewhere? */
		_alUnlockBuffer();

		return;
	}

	_alBufferAddQueueRef( buf, sid );

	_alUnlockBuffer();

	return;
}

/*
 * _alBidAddCurrentRef( ALuint bid, ALuint sid )
 *
 * adds a current reference to the buffer named by bid.  The reference refers
 * to the source named by sid.
 *
 * If this reference is not removed, _alGet{Bid,Buffer}state will return
 * AL_PROCESSED.
 */
void _alBidAddCurrentRef( ALuint bid, ALuint sid ) {
	AL_buffer *buf;

	_alLockBuffer();

	buf = _alGetBuffer( bid );
	if(buf == NULL) {
		/* invalid name */
		_alUnlockBuffer();
		return;
	}

	_alBufferAddCurrentRef( buf, sid );

	_alUnlockBuffer();

	return;
}

/*
 * _alBufferAddQueueRef( AL_buffer *buf, ALuint sid )
 *
 * adds a queue reference to the AL_buffer *buf.  The queue reference refers
 * to the source named by sid.
 *
 * If no current reference is added, and this queue reference is not deleted,
 * _alGet{Bid,Buffer}State will return AL_PENDING.
 *
 * assumes locked context, buffers
 */
static void _alBufferAddQueueRef( AL_buffer *buf, ALuint sid ) {
	ALvoid *temp;
	ALuint newsize = buf->queue_list.size;

	if(buf->queue_list.size <= buf->queue_list.items) {
		/* resize */
		newsize *= 2;
		newsize += 1;
		temp = realloc(buf->queue_list.sids, newsize * sizeof *buf->queue_list.sids);
		if(temp == NULL) {
			/* well la-di-da */
			return;
		}

		buf->queue_list.sids = temp;
		buf->queue_list.size = newsize;
	}

	buf->queue_list.sids[buf->queue_list.items++] = sid;

	return;
}

/*
 * _alBufferAddCurrentRef( AL_buffer *buf, ALuint sid )
 *
 * adds a current reference to the AL_buffer *buf.  The current reference refers
 * to the source named by sid.
 *
 * If this current reference is not removed, _alGet{Bid,Buffer}State will
 * return AL_PENDING.
 *
 * assumes locked context, buffers
 */
static void _alBufferAddCurrentRef( AL_buffer *buf, ALuint sid ) {
	ALvoid *temp;
	ALuint newsize = buf->current_list.size;

	if( buf->current_list.size <= buf->current_list.items ) {
		/* resize */
		newsize *= 2;
		newsize += 1;

		temp = realloc( buf->current_list.sids, newsize * sizeof *buf->current_list.sids );
		if(temp == NULL) {
			/* well la-di-da */
			return;
		}

		buf->current_list.sids = temp;
		buf->current_list.size = newsize;
	}

	buf->current_list.sids[buf->current_list.items++] = sid;

	return;
}

/*
 * _alBufferRemoveQueueRef( AL_buffer *buf, ALuint sid )
 *
 * removes the first queue reference from the AL_buffer *buf that refers to
 * sid.
 *
 * assumes locked context, buffer
 */
static void _alBufferRemoveQueueRef( AL_buffer *buf, ALuint sid ) {
	ALuint i;

	for(i = 0; i < buf->queue_list.items; i++) {
		if(buf->queue_list.sids[i] == sid) {
			buf->queue_list.items--;
			buf->queue_list.sids[i] = buf->queue_list.sids[buf->queue_list.items];

			return;
		}
	}

	return;
}

/*
 * _alBufferRemoveCurrentRef( AL_buffer *buf, ALuint sid )
 *
 * removes the first current reference from the AL_buffer *buf that refers to
 * sid.
 *
 * assumes locked context, buffers
 */
static void _alBufferRemoveCurrentRef( AL_buffer *buf, ALuint sid ) {
	ALuint i;

	for(i = 0; i < buf->current_list.items; i++) {
		if(buf->current_list.sids[i] == sid) {
			buf->current_list.items--;
			buf->current_list.sids[i] = buf->current_list.sids[buf->current_list.items];

			return;
		}
	}

	return;
}

/*
 * _alGetBidState( ALuint bid )
 *
 * Returns the state (one of AL_UNUSED, AL_PROCESSED, AL_PENDING) associated
 * with a buffer.
 *
 * assumes locked buffers
 */
ALenum _alGetBidState( ALuint bid ) {
	ALenum retval = AL_UNUSED;
	AL_buffer *buf;

	buf = _alGetBuffer( bid );
	if(buf != NULL) {
		retval = _alGetBufferState( buf );
	}

	return retval;
}

/*
 * _alGetBufferState( AL_buffer *buffer )
 *
 * Returns the state associated with the AL_buffer *buffer.  If there is even
 * one current ref, AL_PROCESED is returned.  If there are no current
 * references and one queued ref, AL_PENDING is returned.  If there are no
 * current or queued references, AL_UNUSED is returned.
 *
 * assumes locked buffers.
 */
ALenum _alGetBufferState( AL_buffer *buffer )
{
	if(buffer->current_list.items > 0)
	{
		return AL_PROCESSED;
	}

	if(buffer->queue_list.items > 0)
	{
		return AL_PENDING;
	}

	return AL_UNUSED;
}


void _alBufferFreeOrigBuffers(AL_buffer *buf)
{
	ALuint i, j;
	ALvoid *temp;

	/* sort */
	for(i = 0; i < elementsof(buf->orig_buffers); i++)
	{
		for(j = i+1; j < elementsof(buf->orig_buffers); j++)
		{
			if(buf->orig_buffers[i] > buf->orig_buffers[j])
			{
				temp = buf->orig_buffers[i];
				buf->orig_buffers[i] = buf->orig_buffers[j];
				buf->orig_buffers[j] = temp;
			}
		}
	}

	/* uniq */
	for(i = 0; i < elementsof(buf->orig_buffers) - 1; i++)
	{
		if(buf->orig_buffers[i] == buf->orig_buffers[i+1])
		{
			buf->orig_buffers[i] = NULL;
		}
	}


	/* free */
	for(i = 0; i < elementsof(buf->orig_buffers); i++)
	{
		free(buf->orig_buffers[i]);
		buf->orig_buffers[i] = NULL;
	}
}

/* binary compatibility function */
ALsizei alBufferAppendData( ALuint   buffer,
                            ALenum   format,
                            void*    data,
		            ALsizei  osamps,
                            ALsizei  freq) {
	return alBufferAppendData_LOKI(buffer, format, data, osamps, freq);
}

#define MAX_BUFFER_NUM_VALUES 1

static ALint
numValuesForAttribute( ALenum param )
{
	switch (param) {
	case AL_FREQUENCY:
	case AL_SIZE:
	case AL_BITS:
	case AL_CHANNELS:
		return 1;
	default:
		return 0;
	}
}

static void
setBufferAttributef( ALuint bid, ALenum param, const ALfloat *values, ALint numValues)
{
	_alLockBuffer();

	if (!alIsBuffer(bid)) {
		_alcDCLockContext();
		_alDCSetError( AL_INVALID_NAME );
		_alcDCUnlockContext();
		_alUnlockBuffer();
		return;
	}

	if (numValues != numValuesForAttribute(param)) {
		_alcDCLockContext();
		_alDCSetError(AL_INVALID_ENUM);
		_alcDCUnlockContext();
		_alUnlockBuffer();
		return;
	}

	if( values == NULL ) {
		_alcDCLockContext();
		_alDCSetError(AL_INVALID_VALUE);
		_alcDCUnlockContext();
		_alUnlockBuffer();
		return;
	}

	switch (param) {
	default:
		_alcDCLockContext();
		_alDCSetError( AL_INVALID_ENUM );
		_alcDCUnlockContext();
		break;
	}

	_alUnlockBuffer();
}

static void
setBufferAttributei( ALuint bid, ALenum param, const ALint *intValues, ALint numValues)
{
	ALfloat floatValues[MAX_BUFFER_NUM_VALUES];
	int i;
	for (i = 0; i < numValues; i++) {
		floatValues[i] = (ALfloat)intValues[i];
	}
	setBufferAttributef(bid, param, floatValues, numValues);
}

void
alBufferf( ALuint bid, ALenum param, ALfloat value )
{
	setBufferAttributef(bid, param, &value, 1);
}

void
alBuffer3f( ALuint bid, ALenum param, ALfloat value1, ALfloat value2, ALfloat value3 )
{
	ALfloat values[3];
	values[0] = value1;
	values[1] = value2;
	values[2] = value3;
	setBufferAttributef(bid, param, values, 3);
}

void
alBufferfv( ALuint bid, ALenum param, const ALfloat *values )
{
	setBufferAttributef(bid, param, values, numValuesForAttribute(param));
}

void
alBufferi( ALuint bid, ALenum param, ALint value )
{
	setBufferAttributei(bid, param, &value, 1);
}

void
alBuffer3i( ALuint bid, ALenum param, ALint value1, ALint value2, ALint value3 )
{
	ALint values[3];
	values[0] = value1;
	values[1] = value2;
	values[2] = value3;
	setBufferAttributei(bid, param, values, 3);
}

void
alBufferiv( ALuint bid, ALenum param, const ALint *values )
{
	setBufferAttributei(bid, param, values, numValuesForAttribute(param));
}

static ALboolean
getBufferAttribute( ALuint bid, ALenum param, ALfloat *values, ALint numValues )
{
	ALboolean ok = AL_FALSE;
	AL_buffer *buf;

	_alLockBuffer();

	buf = _alGetBuffer(bid);
	if (buf == NULL) {
		_alcDCLockContext();
		_alDCSetError( AL_INVALID_NAME );
		_alcDCUnlockContext();
		_alUnlockBuffer();
		return ok;
	}

	if (numValues != numValuesForAttribute(param)) {
		_alcDCLockContext();
		_alDCSetError(AL_INVALID_ENUM);
		_alcDCUnlockContext();
		_alUnlockBuffer();
		return ok;
	}

	if( values == NULL ) {
		_alcDCLockContext();
		_alDCSetError(AL_INVALID_VALUE);
		_alcDCUnlockContext();
		_alUnlockBuffer();
		return ok;
	}

	switch( param ) {

	case AL_FREQUENCY:
		values[0] = (ALfloat)buf->frequency;
		ok = AL_TRUE;
		break;

	case AL_SIZE:
		values[0] = (ALfloat)(buf->size * _alGetChannelsFromFormat(buf->format));
		ok = AL_TRUE;
		break;

	case AL_BITS:
		values[0] = _alGetBitsFromFormat( buf->format );
		ok = AL_TRUE;
		break;

	case AL_CHANNELS:
		values[0] = _alGetChannelsFromFormat( buf->format );
		ok = AL_TRUE;
		break;

	default:
		_alcDCLockContext();
		_alDCSetError( AL_INVALID_ENUM );
		_alcDCUnlockContext();
		break;
	}

	_alUnlockBuffer();
	return ok;
}

void
alGetBufferf( ALuint bid, ALenum param, ALfloat *value )
{
	getBufferAttribute(bid, param, value, 1);
}

void
alGetBuffer3f( ALuint bid, ALenum param, ALfloat *value1, ALfloat *value2, ALfloat *value3)
{
	ALfloat floatValues[3];
	if (getBufferAttribute(bid, param, floatValues, 3)) {
		*value1 = floatValues[0];
		*value2 = floatValues[1];
		*value3 = floatValues[2];
	}
}

void
alGetBufferfv( ALuint bid, ALenum param, ALfloat *values )
{
	getBufferAttribute(bid, param, values, numValuesForAttribute(param));
}

void
alGetBufferi( ALuint bid, ALenum param, ALint *value )
{
	ALfloat floatValues[1];
	if (getBufferAttribute(bid, param, floatValues, 1)) {
		*value = floatValues[0];
	}
}

void
alGetBuffer3i( ALuint bid, ALenum param, ALint *value1, ALint *value2, ALint *value3)
{
	ALfloat floatValues[3];
	if (getBufferAttribute(bid, param, floatValues, 3)) {
		*value1 = (ALint)floatValues[0];
		*value2 = (ALint)floatValues[1];
		*value3 = (ALint)floatValues[2];
	}
}

void
alGetBufferiv( ALuint bid, ALenum param, ALint *values )
{
	ALfloat floatValues[MAX_BUFFER_NUM_VALUES];
	int i;
	int n = numValuesForAttribute(param);
	if (getBufferAttribute(bid, param, floatValues, n)) {
		for (i = 0; i < n; i++) {
			values[i] = (ALint)floatValues[i];
		}
	}
}
