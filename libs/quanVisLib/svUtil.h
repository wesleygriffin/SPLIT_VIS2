
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
   GLfloat rotatetm[16];
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

void ProgressBar(int percentage);


svVector3 GetVertical(svVector3 pos, svVector3 dir,  ViewProperty &property);
svVector3 GetVerticalByeye(svVector3 pos, svVector3 v, ViewProperty &property);
bool rayPlaneIntersect(svVector3 rPos, svVector3 rDir,
svVector3 pPos0, svVector3 pPos1, svVector3 pPos2,svVector3 pPos3,
svVector3 pDir, svScalar &distance, svVector3 &pos);
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


void GetCylinder(svVector3 glyph, svVector3 dir,
                svScalar radius, svScalar height,
                int segment,
                svVector3 *cylinder_seg_norm,
                 svVector3 *cylinder_seg_pos);
void GetCone(svVector3 glyph, svVector3 dir,
                svScalar radius, svScalar height,
                int segment,
              svVector3 *cone_seg_norm, svVector3 *cone_seg_pos);
void GetRotateFont(svVector3 dir, double &angle_x,  double &angle_z,
                   ViewProperty &property);
svVector3 GetNewVector(svVector3 v, GLfloat *m);
void GetRotateAngleX(svVector3 dir, double &angle_x,  double &angle_z);
void GetRotateAngle(svVector3 dir, double &angle_x,  double &angle_z);
svVector3 GetRotateVector(svVector3 v, GLfloat *m);
//===========Rendering==================
void RenderSpring(svVector3 glyph, svScalar den, svScalar height, svScalar increment);
void RenderCone(svVector3 glyph, svVector3 dir,
                svScalar radius, svScalar height,
                int segment);
void RenderCylinder(svVector3 glyph, svVector3 dir,
                svScalar radius, svScalar height,
                int segment); 
void RenderCylinderTexture(svVector3 glyph, svVector3 dir,
                svScalar radius, svScalar height,svVector4 col,
                svScalar ratio, int texture_num, int segment);
void RenderSphere(svVector3 glyph, svScalar radius,svVector3 dir, 
                   int segment1, int segment2);
void RenderButtonUp(svScalar width, svScalar height,
                 int segment);
void RenderButtonDown(svScalar width, svScalar height,
                 int segment);

//https://en.wikibooks.org/wiki/OpenGL_Programming/Intermediate/Textures
//GLuint loadTexture(GLuint texture, const string filename, int &width, int &height);
//GLuint loadBMP(const char* filename);
//GLuint loadBMP2( const char * filename );
void SOILTexture(const char *filename, GLuint &texture);
//void makeCheckImage();
//=======================================
svInt  getNumOfIntegerDigits(svScalar num);
GLboolean invert_matrix( const GLfloat *m, GLfloat *out );
svScalar GetEntropy(svVector3Array vec);
}


#endif // __SV_UTIL_H
