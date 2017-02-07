#ifndef __SV_MAGNITUDE_H
#define __SV_MAGNITUDE_H
#endif
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <vector>
#include "svType.h"
#include "svVectorField.h"
#include "svVectorMatrix.h"
#include "svArray.h"
//#include "trackball.h"

using namespace std;

namespace __svl_lib{


struct POINT {
    GLfloat x;
    GLfloat y;
    GLfloat z;
};


class svMagnitude
{
public:
	svMagnitude();
	~svMagnitude(){cleanup();}

	void SetMag(svScalar m1, svScalar m2, int w, int h); //mag range


	void RenderMagnitude(int w, int h);


	void Find_Nearest(int x, int y);
	bool Find_Near(int x, int y);
	void move_ctrlpoint(int x, int y, int w, int h);

	void SetLocation(int w, int h);


	int SELECTED;
	int width;
	int NEAR;

	int GetLevel(){return level;}
	svScalar *GetMag(svScalar *den,int datasize,int w, int h);


	int SCALE;


        GLboolean invert_matrix(const GLfloat *m, GLfloat *out);

	void SetMagScalar(float s){magscale = s;}
	void SetControlPoints(POINT *c, int n, int w, int h);
	
	int GetSelectedPoint(){return ni;}
	POINT* GetControlPoints(){return CtrlPoints;}

	void SetFontColor(svVector3 c){font_color = c;}

private: 
	int level;
	int llevel, rlevel;
	svScalar middle;
	svScalar a1, b1, a2, b2;
	svScalar maxmag[2];
	svScalar * MagWidget;
	svScalar lmag, rmag; //automatically updated with the region of insterest;
	svVector3 * WidgetH;
	svVector3 * WidgetV;
	struct POINT CtrlPoints[5];
	int W;
	int H;

	
	int ni,nj;
	GLdouble wz[5];
	GLdouble wx[5];
	GLdouble wy[5];

	float magscale;
	svVector3 font_color;
};


}

