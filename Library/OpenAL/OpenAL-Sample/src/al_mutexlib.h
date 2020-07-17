/* -*- mode: C; tab-width:8; c-basic-offset:8 -*-
 * vi:set ts=8:
 *
 * mutexlib.h
 *
 * Include that sorts out which mutex library we're using.
 */
#ifndef AL_AL_MUTEXLIB_H_
#define AL_AL_MUTEXLIB_H_

#include "al_siteconfig.h"

#if defined(USE_EMPTY_LOCKS)

#define _alCreateMutex()
#define _alDestroyMutex(m)
#define _alLockMutex(m)
#define _alTryLockMutex(m)
#define _alUnlockMutex(m)

#else /* not defined(USE_EMPTY_LOCKS) */

#if defined(USE_POSIXTHREADING)

#include <pthread.h>
typedef pthread_mutex_t *MutexID;

#elif defined(USE_WINDOWSTHREADING)

#include <windows.h>
typedef LPCRITICAL_SECTION MutexID;

#else /* not defined(USE_WINDOWSTHREADING) */

#error "No mutex package"

#endif /* not defined(USE_WINDOWSTHREADING) */

/* Creates a new mutex. Returns NULL on error. */
extern MutexID _alCreateMutex( void );

/* Destroys the given mutex, which must be unlocked. */
extern void _alDestroyMutex( MutexID mutex );

/* Locks the given mutex, blocking if it is already locked. */
extern void _alLockMutex( MutexID mutex );

/* Try to lock the given mutex, returning zero if it succeeded, non-zero
   otherwise. Non-blocking. */
extern int _alTryLockMutex( MutexID mutex );

/* Unlocks the given mutex, which must be locked by the same thread. */
extern void _alUnlockMutex( MutexID mutex );

#endif /* not defined(USE_EMPTY_LOCKS) */

#endif /* AL_AL_MUTEXLIB_H_ */
