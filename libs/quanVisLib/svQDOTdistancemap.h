/**
  by Henan Zhao
  10/26/2014
  Elements
 */

#ifndef __SV_QDOT_distancemap_H
#define __SV_QDOT_distancemap_H

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"
#include "svUtil.h"
#include "svMap.h"
#include "../jcUtil/MGL.h"
#include "../jcUtil/MArray.h"
#include "../jcUtil/MVectorMatrix.h"

#ifndef M_PI
#define M_PI 3.1415926
#endif
using namespace  std;

namespace __svl_lib {

class svQDOTdistancemap : public svMap
{
  public:
    svQDOTdistancemap(svVector3 *pos, 
		     int num, 
		     svVector3 l, svVector3 r, 
		     float x1, float y1, 
		     float x2, float y2,
		     int xn, int yn, float z);
    virtual ~svQDOTdistancemap(){cleanup();}

	void New(svVector3 *pos, 
		     int num, 
		     svVector3 l, svVector3 r, 
		     float x1, float y1, 
		     float x2, float y2,
		     int xn, int yn, float z);
	void SetDistance(svScalar *dis); //raw in ROI / whole

	void SetColors(svVector4 *col );
	
	void Render(float a, float f); 

	int SelectPoint(float x, float y,float f);
	int SelectBox(int x, int y, float scale_x, float scale_y, float zoom, float xpan, float ypan);
	
	void SetSelectedLayer(int n);
	int GetSelectedLayer(){return select_layer;}
	double GetSelectedZ(){return dataPos[dataIndex[select_layer][0]][2];}
	void DecideSelectedPoint(){decide_layer = select_layer;}

	void SetFontColor(svVector3 c){font_color = c;}
	int GetSelectPoint(){return select_index;}

	void cleanup();
private:

	svScalar *dataDistance;
	svVector4 *dataColor;
	svVector3 *startPos;
	
	bool Selected;

	int ni;
	int nj;

	int selected_layer;
	int select_index;
	int select_layer;
	int decide_layer;
	//svVector3 select_col;

	svVector3 font_color;
	

	float zmin;


};

}

#endif // __SV_QDOT_distancemap_H
