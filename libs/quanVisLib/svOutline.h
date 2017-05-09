#ifndef __SV_OUTLINE_H
#define __SV_OUTLINE_H


#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifndef PI
#define PI 3.1415926
#endif 

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"
#include "svUtil.h"
namespace __svl_lib{

class svOutline
{
public:
	svOutline(){}
	~svOutline(){}	

        void DrawLayers(svIntArray index, svVector3 pos, svVector3 dir, svScalar distance);
  	void DrawAxis(svVector3 lbbox, svVector3 rbbox);
	void DrawXYZ(svVector3 lbbox, svVector3 rbbox,ViewProperty &property);
	void DrawAxis_scale(svVector3 lbbox, svVector3 rbbox, int width);
        void DrawXYZFont(svVector3 lbbox, svVector3 rbbox, ViewProperty &property);

	void SetFontColor(svVector3 c){font_color = c;}
private:
	svVector3 font_color;
};



}
#endif
