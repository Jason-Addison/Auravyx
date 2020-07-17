/****************************************************************************

   FILE: xlheader.h

	DESCRIPTION: loads OS-specific xL headers

****************************************************************************/

#ifdef WINDOWS
#include <windows.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <al/alut.h>
#include <sstream>
#endif

#ifdef MACOS
#include <Windows.h>
#include <ToolUtils.h>
# include "gl.h"
#include "glu.h"
#include "glut.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "alut.h"
#include <sstream>
#endif

#ifdef MAC_OS_X
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <al.h>
#include <alc.h>
#include <alut.h>
#include <glut.h>
#endif


#ifdef LINUX
#include "GL/glut.h"
#include <stdio.h>
#include <math.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <sstream>
#endif
