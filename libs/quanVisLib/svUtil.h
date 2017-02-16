
#ifndef __SV_UTIL_H
#define __SV_UTIL_H
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "svVectorField.h"

namespace __svl_lib {
#define SWAP(T, a, b)   { T t; t=a; a=b; b=t;  }

#define SV_PI	  (3.14159265358979323846)
#define SV_2_PI   (3.14159265358979323846 * 2.0)
#define SV_SMALL  (1e-6)

#define svToDegree(x)             ((x)*(180.)/SV_PI)
#define svToRadian(x)	           ((x)*SV_PI/(180.))
#define svMax(a,b)                (((a) > (b)) ? (a) : (b))
#define svMin(a,b)                (((a) < (b)) ? (a) : (b))
#define svClamp(v, min_v, max_v)  { v = svMax(svMin(v, max_v), min_v); }


typedef struct ViewProperty{
   GLint viewport[4];
   GLdouble mvmatrix[16];
   GLdouble projmatrix[16];
   GLfloat tm[16];
   svVector3 eye;
}ViewProperty;
	
template <class T> inline
  void svSwap(T & a, T & b)
{  T t = a; a = b; b = t;  }

svVector3 svGetPerpendicularVector(const svVector3 & v);
svVector3 svGetRotatePoint(const svVector3& start, const svVector3& org, 
                           const svVector3& axis, svScalar rotate_degree);
svVector3 svGetNorm(const svVector3& v1, const svVector3& v0,
                    const svVector3& v2);
svVector3 svAverage(const svVector3& v1, const svVector3& v2);


//---------the following by Henan----------------

svVector3 GetVertical(svVector3 pos, svVector3 dir,  ViewProperty &property);
svVector3 GetVerticalByeye(svVector3 pos, svVector3 v, ViewProperty &property);

bool rayTriangleIntersect(svVector3 rPos, svVector3 rDir, 
			svVector3 pPos0, svVector3 pPos1, svVector3 pPos2,
			svVector3 pDir, svScalar &distance, svVector3 &pos) ;
bool raySphereIntersect(svVector3 rPos, svVector3 rDir, 
				svVector3 sPos, svScalar sRadius,
				svScalar &distance);
double GetDot(svVector3 a, svVector3 b);
bool linePlane(svVector3 a, svVector3 b, svVector3 p, svVector3 n);
bool planeBox(svVector3 lb, svVector3 rb, svVector3 p, svVector3 n);
float PointtoLine(svVector3 p1, svVector3 p2, svVector3 p);
float PointtoLine2(svVector3 p1, svVector3 n, svVector3 p);
bool LinetoPlane(svVector3 p0, svVector3 p, svVector3 l0, svVector3 l, svVector3 &newp);
bool LessAngle(svVector3 p1, svVector3 p2, svVector3 p);
double pointtoPlane(svVector3 a, svVector3 p, svVector3 n);
svVector3 CrossProduct(svVector3 u, svVector3 v);

svInt  getNumOfIntegerDigits(svScalar num);
GLboolean invert_matrix( const GLfloat *m, GLfloat *out );

}


#endif // __SV_UTIL_H
