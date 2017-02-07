/**
Function: render 2D maps to show regions of interests
Shooting ray on each xy plane to find the boundary

input parameters:
	positions
	size of data
	format of data:
		lbbox, rbbox, min_x_distance, min_y_distance
	threshold of mininum distances in case the "holes" inside the boundary
		(multiple boundary detection)
	which roiBox belongs to (int instead of bool) //please update the similar arrays in other functions!
output:
	render 2D maps
mid parameters:
	2D postion
	boundary position
	boundary color
	binary flag if a data is on the boundary
	binary flat if the data is already detected
**/

#ifndef __SV_Bubble_Set_H
#define __SV_Bubble_Set_H

#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>      // std::setprecision

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

class svMap
{
  public:
    svMap(svVector3 *pos,
		     int num, 
		     svVector3 l, svVector3 r, 
		     float x1, float y1, 
		     float x2, float y2,
		     int xn, int yn, float z);
    ~svMap(){cleanup();}

	/**
	Data:	input sorted data from min z to max z
		data are sorted on each y = a lines from negative to positive
	ROI file: 
		including all roi each data belongs to
	**/
	void setData(svVector3 *pos,
		     int num, 
		     svVector3 l, svVector3 r, 
		     float x1, float y1, 
		     float x2, float y2,
		     int xn, int yn, float z);
	void setROI(svBool *in);

protected:
	void cleanup();


	svVector3 *dataPos;
	vector< vector<svVector3> > splitPos;
	svBool* dataROI;
	svInt dataSize;

	svVector3 lbbox;
	svVector3 rbbox;
	float xd;
	float yd;
	float xl;
	float yl;
	int xnum;
	int ynum;

	vector< vector<int> > dataIndex;
	vector< vector<svVector3> > mapPos;
};

}

#endif // __SV_QDOT_distancemap_H
