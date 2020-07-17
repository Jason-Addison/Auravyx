/* -*- mode: C; tab-width:8; c-basic-offset:8 -*-
 * vi:set ts=8:
 *
 * alc_context.h
 *
 * Prototypes, defines etc for context aquisition and management.
 */
#ifndef AL_ALC_ALC_CONTEXT_H_
#define AL_ALC_ALC_CONTEXT_H_

#include "al_siteconfig.h"
#include <AL/al.h>
#include <AL/alc.h>
#include "al_types.h"

/*
 * _ALC_DEF_FREQ is the default internal mixing frequency.
 */
#define _ALC_DEF_FREQ        48000

/*
 * ALC_DEFAULT_DEVICE_BUFFER_SIZE_IN_BYTES is the default length of chunks mixed
 * and written to the audio device.  Must be a power of two.
 */
#define ALC_DEFAULT_DEVICE_BUFFER_SIZE_IN_BYTES 1024

/*
 * Canonical stuff
 */
#define _ALC_CANON_FMT           AL_FORMAT_MONO16
#define _ALC_CANON_SPEED         _ALC_DEF_FREQ

#define _ALC_EXTERNAL_FMT        AL_FORMAT_STEREO16
#define _ALC_EXTERNAL_SPEED      _ALC_DEF_FREQ

/* globally accesible data */

/*
 * _alcCCId holds the context id of the current context.
 */
extern ALuint _alcCCId;

/*
 * canon_max and canon_min are the max/min values for PCM data in our
 * canonical format, respectively.
 */
extern const int canon_max, canon_min;



/*
 * canon_format is the canonical format that we represent data internally as.
 */
extern ALenum canon_format;

/*
 * canon_speed is the sampling rate at which we internally represent data.
 */
extern ALuint canon_speed;

/*
 * Deallocates the data structures allocated in _alcInitContexts.
 */
void _alcDestroyAll( void );

/*
 * Initialize the context named by cid.
 */
AL_context *_alcInitContext( ALuint cid );

/*
 * Returns pointer to the AL_context named by cid, or NULL if cid is not a
 * valid context name.
 */
AL_context *_alcGetContext( ALuint cid );

/*
 * Returns AL_TRUE if cid names a valid context, AL_FALSE otherwise.
 */
ALboolean _alcIsContext( ALuint cid );

/*
 * Sets context id paramaters according to an attribute list and device.
 */
ALCboolean _alcSetContext( const ALCint *attrlist, ALuint cid, AL_device *dev );

/*
 * Returns a new id for use as a context name.
 */
ALint _alcGetNewContextId( void );

/*
 * Returns AL_TRUE if the context named by cid is in use, AL_FALSE otherwise.
 */
ALboolean _alcInUse( ALuint cid );

/*
 * Sets the use flag of context with id cid to value.
 */
ALboolean _alcSetUse( ALuint cid, ALboolean value );

/*
 * Returns the preferred read buffer size of the context named by cid,
 * in bytes.
 */
ALuint _alcGetReadDeviceBufferSizeInBytes( ALuint cid );

/*
 * Returns the preferred write buffer size of the context named by cid,
 * in bytes.
 */
ALuint _alcGetWriteDeviceBufferSizeInBytes( ALuint cid );

/*
 * Returns the preferred read openal format of the context named by cid.
 */
ALenum _alcGetReadFormat( ALuint cid );

/*
 * Returns the preferred write openal format of the context named by cid.
 */
ALenum _alcGetWriteFormat( ALuint cid );

/*
 * Returns the preferred sampling rate of the read device associated with the
 * context named by cid.
 */
ALuint _alcGetReadSpeed( ALuint cid );

/*
 * Returns the preferred sampling rate of the write device associated with the
 * context named by cid.
 */
ALuint _alcGetWriteSpeed( ALuint cid );

/*
 * Returns a pointer to the listener associated with context named by cid, or
 * NULL if cid does not name a valid context.
 */
AL_listener *_alcGetListener( ALuint cid );

/*
 * Returns AL_TRUE if this context is suspended, AL_FALSE otherwise.
 * Suspended contexts do not have their sources updated, or mixed.
 */
ALboolean _alcIsContextSuspended( ALuint cid );

/*
 * Returns a pointer to the time_filter_set associated with the context named
 * by cid, or NULL if cid does not name a context.
 */
time_filter_set *_alcGetTimeFilters( ALuint cid );

/*
 * Locks the mutex associated with the context named by cid, passing fn and ln
 * to _alLockPrintf for debugging purposes.
 */
void FL_alcLockContext( ALuint cid, const char *fn, int ln );

/*
 * Unlocks the mutex associated with the context named by cid, passing fn and ln
 * to _alLockPrintf for debugging purposes.
 */
void FL_alcUnlockContext( ALuint cid, const char *fn, int ln );

/*
 * Locks the mutex associated guarding all contexts, passing fn and ln to
 * _alLockPrintf for debugging purposes.
 */
void FL_alcLockAllContexts( const char *fn, int ln );

/*
 * Unlocks the mutex associated guarding all contexts, passing fn and ln to
 * _alLockPrintf for debugging purposes.
 */
void FL_alcUnlockAllContexts( const char *fn, int ln );

/* default context macros */
#define _alcDCGetContext()        _alcGetContext(_alcCCId)
#define _alcDCGetReadDeviceBufferSizeInBytes() _alcGetReadDeviceBufferSizeInBytes(_alcCCId)
#define _alcDCGetWriteDeviceBufferSizeInBytes() _alcGetWriteDeviceBufferSizeInBytes(_alcCCId)
#define _alcDCGetTimeFilters()    _alcGetTimeFilters(_alcCCId)
#define _alcDCGetFreqFilters()    _alcGetFreqFilters(_alcCCId)
#define _alcDCGetListener()       _alcGetListener(_alcCCId)
#define _alcDCEnableCapture()     _alcEnableCapture(_alcCCId)
#define _alcDCDisableCapture()    _alcDisableCapture(_alcCCId)

#define _alcDCGetReadSpeed()      _alcGetReadSpeed(_alcCCId)
#define _alcDCGetReadFormat()     _alcGetReadFormat(_alcCCId)
#define _alcDCGetWriteSpeed()     _alcGetWriteSpeed(_alcCCId)
#define _alcDCGetWriteFormat()    _alcGetWriteFormat(_alcCCId)

#define _alcDCLockContext()     FL_alcLockContext(_alcCCId, __FILE__, __LINE__)
#define _alcDCUnlockContext()   FL_alcUnlockContext(_alcCCId,__FILE__, __LINE__)
#define _alcUnlockContext(c)    FL_alcUnlockContext(c, __FILE__, __LINE__)
#define _alcLockContext(c)      FL_alcLockContext(c, __FILE__, __LINE__)
#define _alcUnlockAllContexts() FL_alcUnlockAllContexts(__FILE__, __LINE__)
#define _alcLockAllContexts()   FL_alcLockAllContexts(__FILE__, __LINE__)

#endif /* not AL_ALC_ALC_CONTEXT_H_ */
