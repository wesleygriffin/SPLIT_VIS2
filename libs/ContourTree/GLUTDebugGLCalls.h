//#include <MSLCarbonPrefix.h>

#ifndef DEBUG_GL_CALLS

//#define DEBUG_GL_BEGIN_END 1

#ifdef DEBUG_GL_BEGIN_END
extern int glBeginDepth;

#include <GL/gl.h>

void debugGLBegin(GLenum mode, char *fileName, int fileLine);
void debugGLEnd(char *fileName, int fileLine);

#define glEnd() debugGLEnd(__FILE__, __LINE__)
#define glBegin(x) debugGLBegin(x, __FILE__, __LINE__)


#endif
#endif