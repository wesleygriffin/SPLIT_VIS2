
#include "svPickingray.h"

namespace __svl_lib {
  
  void svPickingray::initCamera(svVector3 cameraPos,
		  svVector3 cameraLookat,
		  svVector3 cameraUp,
		  svScalar fovy,
		  int width,
		  int height,
		  svScalar nearDistance)
  {
    pCamerapos = cameraPos;
    pNearclippingdistance = nearDistance;
    pWidth = width;
    pHeight = height;
    
    pView = cameraLookat - cameraPos;
    pH = cross(pView, cameraUp);
    pV = cross(pH, pView);
    
    pView = normalize(pView);
    pH = normalize(pH);
    pV = normalize(pV);
    
    svScalar rad = fovy * PI/180.;
    pVlength = tan(rad/2.) * nearDistance;
    pHlength = pVlength *  ((svScalar)width / (svScalar)height);
    
	cerr<<"camera"<<" "<<pH[0]<<" "<<pH[1]<<" "<<pH[2]<<endl;
	cerr<<"camera"<<" "<<pV[0]<<" "<<pV[1]<<" "<<pV[2]<<endl;

    pH = pHlength * pH;
    pV = pVlength * pV;

	cerr<<"camera"<<" "<<pHlength<<" "<<pVlength<<endl;
	cerr<<"camera"<<" "<<pCamerapos[0]<<" "<<pCamerapos[1]<<" "<<pCamerapos[2]<<endl;
  }
  
   void svPickingray::getPickingray(svScalar screenX,
				   svScalar screenY)
  {
      screenX  = screenX - (svScalar) pWidth/2.;
      screenY  = - screenY + (svScalar) pHeight/2.;
      
      screenY  = screenY /(svScalar) (pHeight/2.);
      screenX  = screenX /(svScalar) (pWidth/2.);
	cerr<<"Screen"<<screenX<<" "<<screenY<<endl;
      
      pickingPos = pCamerapos + pView * pNearclippingdistance 
		  + pH * screenX + pV * screenY;
      pickingDir = pickingPos - pCamerapos;
      pickingDir = normalize(pickingDir);
  }

void svPickingray::render()
{
	//glDisable(GL_LIGHTING);
	glColor3f(1,0,0);

	/*glLineWidth(10.);
	glBegin(GL_LINES);
	glVertex3f(pickingPos[0], pickingPos[1], pickingPos[2]);
	glVertex3f(pickingPos[0]+100*pickingDir[0], pickingPos[1]+100*pickingDir[1], pickingPos[2]+100*pickingDir[2]);
	glEnd();*/

	glPushMatrix();
	glTranslatef(pickingPos[0]+100*pickingDir[0], pickingPos[1]+100*pickingDir[1], pickingPos[2]+100*pickingDir[2]);//27.7584, 25.2209, -42.8168);
	glutSolidSphere(0.5, 10, 10);
	glPopMatrix();


	//glLineWidth(1.);
}

}
