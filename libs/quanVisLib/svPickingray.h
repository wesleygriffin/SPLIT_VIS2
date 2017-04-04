/**
http://schabby.de/picking-opengl-ray-tracing/
Pickying array 
**/

#ifndef __SV_PICKING_RAY_H
#define __SV_PICKING_RAY_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <vector>

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"
#include "svUtil.h"

#ifndef PI
#define PI 3.1415926
#endif

using namespace std;

namespace __svl_lib {

class svPickingray 
{
 public:
  svPickingray(){}
  ~svPickingray(){}
  
  void initCamera(svVector3 cameraPos,
		  svVector3 cameraLookat,
		  svVector3 cameraUp,
		  svScalar fovy,
		  int width,
		  int height,
		  svScalar nearDistance);
		  
  void getPickingray(svScalar screenX,
			  svScalar screenY);
  void setPickingray(GLfloat *invert_tb);
  svVector3 getRayPostion(int x, int y, ViewProperty &property);
  int getSelectedBar(svVector3Array *barEnd1,
                     svVector3Array *barEnd2,
                     svVector3Array *barEnd3,
                     svVector3Array *barEnd4,
                     int seed_num); 
  bool getPlanePosition(svVector3 &pos, svVector3 lb, svVector3 rb);

  void render();
  void renderbyPlane(svVector3 lb, svVector3 rb);
			  
  svVector3 getPickingpos(){return pickingPos;}
  svVector3 getPickingdir(){return pickingDir;}

 private:
  svVector3 pView;
  svVector3 pV;
  svVector3 pH;
  svScalar pVlength;
  svScalar pHlength;
  
  svVector3 pCamerapos;
  svScalar pNearclippingdistance;
  int pWidth;
  int pHeight;
  
  svVector3 pickingPos;
  svVector3 pickingDir;
};

}
#endif // __SV_PICKING_RAY_H
