/**
  by Henan Zhao
  10/26/2014
  Elements
 */

//#ifndef __SV_QDOT_distancemap_H
//#define __SV_QDOT_distancemap_H

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"
#include "svUtil.h"

#include "../jcUtil/MGL.h"
#include "../jcUtil/MArray.h"
#include "../jcUtil/MVectorMatrix.h"

#ifndef M_PI
#define M_PI 3.1415926
#endif
using namespace  std;

namespace __svl_lib {

class svQDOTmagnitudemap
{
  public:
    svQDOTmagnitudemap();
    ~svQDOTmagnitudemap(){cleanup();}

	void SetData(svVector3 *pos, svScalar *dis, int num, 
		     svVector3 l, svVector3 r, 
		     float x1, float y1, 
		     float x2, float y2,
		     int xnum, int ynum, 
		     float z, int SCALE, double m); //raw in ROI / whole
	void SetInbox(bool *in);

	void SetColors(svVector4 *col );
	
	void RenderLegend();
	void Render( bool *in,float a, float f); 

	void SetFontColor(svVector3 c){font_color = c;}

	void cleanup();
private:

	svScalar *dataDistance;
	svVector3 *dataPos;
	svInt *Z_format;
	svVector3 *startPos;
	bool *dataIn;
	
	int dataSize;
	int zlayer;
	int scaling;

	svVector3 lb;
	svVector3 rb;
	double max;
	double min;

	svVector3 font_color;
	
	float xd;
	float yd;
	float xl;
	float yl;
	float zmin;
	int x_num;
	int y_num;
    //GLint list;

};

}

//#endif // __SV_QDOT_distancemap_H
