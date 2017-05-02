#ifndef __SV_TYPE_H
#define __SV_TYPE_H

#include "svVectorMatrix.h"

#define SV_TRUE   1
#define SV_FALSE  0

namespace __svl_lib {
typedef char            svChar;
typedef unsigned char	svUchar;
typedef short           svShort;
typedef unsigned short  svUshort;
typedef int             svInt;
typedef unsigned int    svUint;
typedef float           svScalar;
typedef float           svFloat;
typedef double          svDouble;
typedef unsigned char   svByte;
typedef unsigned short  svWord;
typedef bool             svBool;

typedef svVector3		svColor3;
typedef svVector4		svColor4;

typedef struct{
  float pos[3];
  float norm[3];
  float color[4]; 
} VBOVertex;

typedef struct{
  float pos[3];
} VBOVertex1;
}

#endif // __SV_TYPE_H
