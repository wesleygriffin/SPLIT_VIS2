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

#define SCALE_WIDGETS_NUM 3
#define WIDGETS_DEFAULT 0
#define WIDGETS_SELECT 1
#define WIDGETS_TOUCH 2

using namespace std;

namespace __svl_lib{


struct POINT {
    GLfloat x;
    GLfloat y;
    GLfloat z;
};


typedef struct MagnitudeWidgetProperty{
        int segment;
        int scalex;
        int scaley;
        int width;
        int height;
        int transx;
        int transy;
        int pointsize;
        int xaxis;
        int yaxis;
        svScalarArray xticks;
        svScalar maxscale;
        svVector3 widgets_default_color;
        svVector3 widgets_select_color;
        svVector3 widgets_touch_color;
        svVector3 font_color;
} MagnitudeWidgetProperty;

class svMagnitude
{
public:
	svMagnitude(){}
	~svMagnitude(){cleanup();}

        void Init(const MagnitudeWidgetProperty & widgetproperty);

        void SetProperty(const MagnitudeWidgetProperty & widgetproperty){property = widgetproperty;}

        void Render();
        void RenderScaleFont( svScalar shift);
        void RenderWidgetFont(svScalarArray & font, svScalar shift);

        void Click(int x, int y);
        void Motion(int x, int y);
        void Movement(int x, int y);

        svScalar getValue(svScalarArray &widgets_values, svScalar &value);
/*	void SetMag(svScalar m1, svScalar m2, int w, int h); //mag range

	void RenderMagnitude(int w, int h);

	void Find_Nearest(int x, int y);
	bool Find_Near(int x, int y);
	void move_ctrlpoint(int x, int y, int w, int h);

	void SetLocation(int w, int h);

	int SELECTED;
//	int width;
	int NEAR;
        void cleanup();
	int GetLevel(){return level;}
	svScalar *GetMag(svScalar *den,int datasize,int w, int h);

	int SCALE;

        GLboolean invert_matrix(const GLfloat *m, GLfloat *out);

	void SetMagScalar(float s){magscale = s;}
	void SetControlPoints(POINT *c, int n, int w, int h);
	
	int GetSelectedPoint(){return ni;}
	POINT* GetControlPoints(){return CtrlPoints;}
*/
protected:
       void cleanup();
private: 
/*	int level;
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
*/
        MagnitudeWidgetProperty property;
        
        svVector3Array widgets_pos;
        svVector3Array widgets_scale_pos;
        svScalar widgets_max_value;
        int widgets_scale_status;
        svIntArray widgets_status; 
/*	
	int ni,nj;
	GLdouble wz[5];
	GLdouble wx[5];
	GLdouble wy[5];

	float magscale;
	svVector3 font_color;
*/
};


}

