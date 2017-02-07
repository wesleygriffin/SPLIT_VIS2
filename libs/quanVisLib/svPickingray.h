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

  void render();
			  
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
