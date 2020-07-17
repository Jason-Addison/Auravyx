/* -*- mode: C; tab-width:8; c-basic-offset:8 -*-
 * vi:set ts=8:
 *
 * al_source.h
 *
 * Prototypes, macros and definitions related to the creation and
 * management of sources.
 *
 */
#ifndef AL_AL_SOURCE_H_
#define AL_AL_SOURCE_H_

#include "al_siteconfig.h"
#include <stdio.h>
#include "al_types.h"

/*
 * Non locking version of alIsSource.
 */
ALboolean _alIsSource( ALuint sid );

/*
 * Returns AL_source named by sid from context named by cid.
 */
AL_source *_alGetSource( ALuint cid, ALuint sid );

/*
 * Returns a pointer to the attribute specified by param in the AL_source
 * object *source, or NULL if the attribute is invalid or has not been set.
 */
void *_alGetSourceParam( AL_source *source, ALenum param );

/*
 * Populates *retref with the default value for source attribute param.  If
 * param is not a valid source attribute, no action is taken.
 */
void _alSourceGetParamDefault( ALenum param, ALvoid *retref );

/*
 * Finalize a source object.
 */
void _alDestroySource( ALvoid *src );

/*
 * Finalize each source object referenced in spool ( see al_spool.h for more
 * information about source pool objects and their relationship to AL_sources).
 */
void _alDestroySources( spool_t *spool );

/*
 * Populate buffs[0..nc-1][0..len/2-1] with data from buf, with offset into
 * buf given by the position associated with the source named by sid in the
 * context named by cid.
 *
 * This function delegates to static functions in the case of looping or
 * callback sounds.
 */
void _alSplitSources( ALuint cid, ALuint sid,
		      ALint nc, ALuint len,
		      AL_buffer *buf, ALshort **buffs );

/*
 * Populates the scratch space associated with the source named by sid in the
 * context named by cid with interleaved data.  The data is interleaved using
 * alternating channels from buffers[0..nc-1].  Each member of the set
 * buffers[0..nc-1] is an indepedent channel's worth of data, 0..mixbuflen/2
 * long bytes long.
 */
void _alCollapseSource( ALuint cid, ALuint sid,
			ALuint nc, ALuint mixbuflen,
			ALshort **buffers );


/*
 * Translates the source (src) position attribute by delta.  Delta is a three
 * tuple x/y/z.
 */
void _alSourceTranslate( AL_source *src, ALfloat *delta );

/*
 * srcParam functions
 *
 * The use of srcParam settings is this: most filter operations break down
 * into applying a multiplier or delay to the raw PCM data copied in
 * SplitSources.  Since there operations are cumulative, this can be simplified
 * by simply collecting the coefficients of the operations and applying them
 * all at one.  So instead of actually performing a gain or delay operation on
 * the PCM data, the filter can change the srcParam setting, which is applied
 * in _alSourceParamApply.
 *
 */

/*
 * Resets the srcParam settings gain and delay.  Does not affect srcParam
 * settings associated with sound position or the temporary scratch space.
 */
void _alSourceParamReset( AL_source *src );

/*
 * Applies the srcParam settings of src to buffers[0..nc-1][0..(len/2)-1].
 */
void _alSourceParamApply( AL_source *src,
			  ALuint nc, ALuint len, ALshort **buffers );

/*
 * Returns the number of buffers queued in the source, not including the
 * current one ( if any ).
 */
ALint _alSourceGetPendingBids( AL_source *src );

/*
 * Returns the next buffer in a queue, or NULL if no next buffer exists.
 * Assumes locked source.
 *
 */
AL_buffer *_alSourceGetNextBuffer( AL_source *src );

/*
 * Returns the total number of buffers queued for the source,
 * without regard to the read or write pointer.
 */
ALint _alSourceQueuedBuffers( AL_source *src );

/*
 * Copies the interleaved data from src[0..(src_size/2)-1] to
 * dstret[0..dest_channels-1], splitting it into seperate channels.
 * src_channels describes the period of the channel repetition in src,
 * dest_channels describes the number of independant buffers in dstref.
 * src_size is the size of src in bytes, and offset is the offset into each
 * seperate channel in dstref where the copying begins.
 */
void _alMonoifyOffset( ALshort **dstref, ALuint offset,
		       ALvoid *src, ALuint src_size,
		       ALuint dest_channels, ALuint src_channels );

/*
 * This function is sort of the complement of _alMonoifyOffset.  Data is
 * copied from srcs[0..nc-1][offset/2..(offset + size)/2-1] into an
 * interleaved array dst.
 */
void _alChannelifyOffset( ALshort *dst, ALuint offset,
			  ALshort **srcs, ALuint size, ALuint nc );


/*
 * Usually, the top-level mixing function _alMixSources handles updating each
 * source's sound position ( a pointer into its associate buffer's raw PCM
 * data. )  Some attributes, like pitch, demand a more sophisticated approach
 * that can only be done by the filter which handles this attribute.
 *
 * _alSourceShouldIncrement returns AL_TRUE if the top-level mixing function
 * should update this sort of state information, and AL_FALSE if it should
 * leave it to another portion of the library.
 */
ALboolean _alSourceShouldIncrement( AL_source *src );

/*
 * Increments the source's (src) offset into its current buffer's PCM data.
 */
void _alSourceIncrement( AL_source *src, ALuint bytes );

/*
 * Returns the byte length of the amount of data left before this source/samp
 * pair will either run out of data or be required to wrap.  This returns the
 * byte length of an interleaved array, with periodic repetition equal to the
 * number of channels in the canonical format.
 */
ALint _alSourceBytesLeft( AL_source *src, AL_buffer *samp );

/*
 * Returns the byte length of the amount of data left before this source/samp
 * pair will either run out of data or be required to wrap.  This returns the
 * byte length of a simple array, with 1 channel's worth of data.
 */
ALint _alSourceBytesLeftByChannel( AL_source *src, AL_buffer *samp );

/*
 * Returns AL_TRUE if the source (src) has its queue set to AL_TRUE,
 * AL_FALSE otherwise.
 */
ALboolean _alSourceIsQueue( AL_source * src );

/*
 * Returns AL_TRUE if the source (src) has its AL_LOOPING attribute set to
 * AL_TRUE, AL_FALSE otherwise.
 */
ALboolean _alSourceIsLooping( AL_source *src );

/*
 * Locks the mutex guarding source sid in context cid, and passes fn and ln
 * to _alLockPrintf for debugging purposes.
 */
ALboolean FL_alLockSource( const char *fn, int ln, ALuint cid, ALuint sid );

/*
 * Unlocks the mutex guarding source sid in context cid, and passes fn and ln
 * to _alLockPrintf for debugging purposes.
 */
ALboolean FL_alUnlockSource( const char *fn, int ln, ALuint cid, ALuint sid );

/* macros */
#define _alDCGetSource(i)              _alGetSource(_alcCCId, i)
#define _alChannelify(d,srcs,size,nc)  _alChannelifyOffset(d, 0, srcs, size, nc)
#define _alMonoify(d, s, size, dc, sc) _alMonoifyOffset(d, 0, s, size, dc, sc)
#define _alLockSource(cid, sid)        FL_alLockSource(__FILE__, __LINE__, cid, sid)
#define _alUnlockSource(cid, sid)      FL_alUnlockSource(__FILE__, __LINE__, cid, sid)
#define _alDCLockSource(sid)           FL_alLockSource(__FILE__, __LINE__, _alcCCId, sid)
#define _alDCUnlockSource(sid)         FL_alUnlockSource(__FILE__, __LINE__, _alcCCId, sid)

#ifdef PARANOID_LOCKING
#define SOURCELOCK()   _alcDCLockContext()
#define SOURCEUNLOCK() _alcDCUnlockContext()
#else /* not PARANOID_LOCKING */
#define SOURCELOCK()
#define SOURCEUNLOCK()
#endif /* not PARANOID_LOCKING */

/*
 * AL_FIRST_SOURCE_ID is the first integer value at which source names are
 * created.
 */
#define AL_FIRST_SOURCE_ID  0x4000

#endif /* not AL_AL_SOURCE_H_ */
