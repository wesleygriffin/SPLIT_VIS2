#include "GLUTDebugGLCalls.h"

int glBeginDepth = 0;

#undef glBegin
#undef glEnd

#include <GL/freeglut.h>
#include <stdio.h>

void debugGLBegin(GLenum mode, char *fileName, int fileLine)
	{
	printf("Entering glBegin() level %d at %s:%d.\n", ++glBeginDepth, fileName, fileLine);
	glBegin(mode);
	}

void debugGLEnd(char *fileName, int fileLine)
	{
	printf("Entering glEnd()   level %d at %s:%d.\n", glBeginDepth--, fileName, fileLine);
	glEnd();
	}

