/* -*- mode: C; tab-width:8; c-basic-offset:8 -*-
 * vi:set ts=8:
 *
 * al_debug.c
 *
 * openal custom debug messages.
 */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif /* _GNU_SOURCE */

#include "al_siteconfig.h"

#include <stdio.h>
#include <stdarg.h>

#include "al_debug.h"

/*
 * ald2str( aldEnum type )
 *
 * Returns a const char * string giving a readable representation of the debug
 * type, or NULL if type is not a valid debug message type.  This string does
 * not need to be free'd.
 */
static const char *ald2str( aldEnum l ) {
	switch( l ) {
		case ALD_INVALID:   return "INVALID";
		case ALD_CONVERT:   return "CONVERT";
		case ALD_CONFIG:    return "CONFIG";
		case ALD_SOURCE:    return "SOURCE";
		case ALD_LOOP:      return "LOOP";
		case ALD_STUB:      return "STUB";
		case ALD_CONTEXT:   return "CONTEXT";
		case ALD_MATH:      return "MATH";
		case ALD_MIXER:     return "MIXER";
		case ALD_ERROR:     return "ERROR";
		case ALD_EXT:       return "EXT";
		case ALD_LOCK:      return "LOCK";
		case ALD_MAXIMUS:   return "MAXIMUS";
		case ALD_STREAMING: return "STREAM";
		case ALD_MEM:       return "MEM";
		case ALD_QUEUE:     return "QUEUE";
		case ALD_FILTER:     return "FILTER";
		default: break;
	}

	return NULL;
}

/*
 * _alDebug( aldEnum level, const char *fn, int ln, const char *format, ... )
 *
 * If debugging messages for the type level are enabled, print the debugging
 * message specified by format, ... ( printf format ).  Otherwise, return.
 *
 */
int _alDebug( aldEnum level, const char *fn, int ln, const char *format, ... )
{
	static char formatbuf[256];
	int count;
	va_list ap;
#ifndef DEBUG_MAXIMUS /* DEBUG_MAXIMUS enables all debugging */

#ifndef DEBUG_LOOP
	if(level == ALD_LOOP) return 0;
#endif
#ifndef DEBUG_STUB
	if(level == ALD_STUB) return 0;
#endif
#ifndef DEBUG_CONVERT
	if(level == ALD_CONVERT) return 0;
#endif
#ifndef DEBUG_CONFIG
	if(level == ALD_CONFIG) return 0;
#endif
#ifndef DEBUG_MATH
	if(level == ALD_MATH) return 0;
#endif
#ifndef DEBUG_EXT
	if(level == ALD_EXT) return 0;
#endif
#ifndef DEBUG_CONTEXT
	if(level == ALD_CONTEXT) return 0;
#endif
#ifndef DEBUG_SOURCE
	if(level == ALD_SOURCE) return 0;
#endif
#ifndef DEBUG_LOCK
	if(level == ALD_LOCK) return 0;
#endif
#ifndef DEBUG_MAXIMUS
	if(level == ALD_MAXIMUS) return 0;
#endif
#ifndef DEBUG_STREAMING
	if(level == ALD_STREAMING) return 0;
#endif
#ifndef DEBUG_MEM
	if(level == ALD_MEM) return 0;
#endif
#ifndef DEBUG_BUFFER
	if(level == ALD_BUFFER) return 0;
#endif
#ifndef DEBUG_LISTENER
	if(level == ALD_LISTENER) return 0;
#endif
#ifndef DEBUG_QUEUE
	if(level == ALD_QUEUE) return 0;
#endif
#ifndef DEBUG_FILTER
	if(level == ALD_FILTER) return 0;
#endif
#ifndef DEBUG_MIXER
	if(level == ALD_MIXER) return 0;
#endif

#endif /* DEBUG_MAXIMUS */

	count = snprintf(formatbuf, sizeof(formatbuf), "%s\t[%s:%d] ", ald2str(level), fn, ln);
	if(count < 0)
	{
		return count;
	}

	va_start(ap, format);
	vsnprintf(formatbuf, sizeof formatbuf, format, ap);
	va_end(ap);

	return fprintf(stderr, "%s\t[%s:%d] %s\n",
		       ald2str(level), fn, ln, formatbuf );
}
