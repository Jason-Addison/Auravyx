/****************************************************************************

   FILE: switches.h

	DESCRIPTION: header for xldemo switches

****************************************************************************/

// OS-selection switches
#ifdef _WIN32
# define WINDOWS
#endif
#if defined(__APPLE__) & defined(__MACH__) // check for OS X
#define MAC_OS_X
#else
#ifdef __MACOS__
# define MACOS
#endif
#endif
#ifdef unix
# ifndef LINUX
# define LINUX
# endif /* LINUX */
#endif

// general switches
#define BLENDING                   // turn on GL blending?
#define LIGHTING
//#define FOG                        // turn on GL fog?
