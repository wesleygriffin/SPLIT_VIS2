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
    
	//cerr<<"camera"<<" "<<pH[0]<<" "<<pH[1]<<" "<<pH[2]<<endl;
	//cerr<<"camera"<<" "<<pV[0]<<" "<<pV[1]<<" "<<pV[2]<<endl;

    pH = pHlength * pH;
    pV = pVlength * pV;

	//cerr<<"camera"<<" "<<pHlength<<" "<<pVlength<<endl;
	//cerr<<"camera"<<" "<<pCamerapos[0]<<" "<<pCamerapos[1]<<" "<<pCamerapos[2]<<endl;
  }
  
   void svPickingray::getPickingray(svScalar screenX,
				   svScalar screenY)
  {
      screenX  = screenX - (svScalar) pWidth/2.;
      screenY  = - screenY + (svScalar) pHeight/2.;
      
      screenY  = screenY /(svScalar) (pHeight/2.);
      screenX  = screenX /(svScalar) (pWidth/2.);
	//cerr<<"Screen"<<screenX<<" "<<screenY<<endl;
      
      pickingPos = pCamerapos + pView * pNearclippingdistance 
		  + pH * screenX + pV * screenY;
      //cerr<<pickingPos[0]<<" "<<pickingPos[1]<<" "<<pickingPos[2]<<endl;
      pickingDir = pickingPos - pCamerapos;
      pickingDir = normalize(pickingDir);

	//harccode
      pickingDir[0] = 0;
      pickingDir[1] = 0;
      pickingDir[2] = -1;
  }

void svPickingray::setPickingray(GLfloat *invert_tb)
{
    svVector3 newRaypos;
    svVector3 newRaydir;

   svVector3 rayPos  = pickingPos;
   svVector3 rayDir = pickingDir;

    newRaypos[0] = rayPos[0] * invert_tb[0] +
            rayPos[1] * invert_tb[4] +
            rayPos[2] * invert_tb[8] +
            invert_tb[12];
    newRaypos[1] = rayPos[0] * invert_tb[1] +
            rayPos[1] * invert_tb[5] +
            rayPos[2] * invert_tb[9] +
            invert_tb[13];
    newRaypos[2] = rayPos[0] * invert_tb[2] +
            rayPos[1] * invert_tb[6] +
            rayPos[2] * invert_tb[10] +
            invert_tb[14];

    newRaydir[0] = (rayPos[0]+rayDir[0]) * invert_tb[0] +
            (rayPos[1]+rayDir[1]) * invert_tb[4] +
            (rayPos[2]+rayDir[2]) * invert_tb[8] +
            invert_tb[12];
    newRaydir[1] = (rayPos[0]+rayDir[0]) * invert_tb[1] +
            (rayPos[1]+rayDir[1]) * invert_tb[5] +
            (rayPos[2]+rayDir[2]) * invert_tb[9] +
            invert_tb[13];
    newRaydir[2] = (rayPos[0]+rayDir[0])  * invert_tb[2] +
            (rayPos[1]+rayDir[1])  * invert_tb[6] +
            (rayPos[2]+rayDir[2])  * invert_tb[10] +
            invert_tb[14];
    newRaydir[0] = newRaydir[0] - newRaypos[0];
    newRaydir[1] = newRaydir[1] - newRaypos[1];
    newRaydir[2] = newRaydir[2] - newRaypos[2];
    newRaydir = normalize(newRaydir);

   pickingPos = newRaypos;
   pickingDir = newRaydir;  
}

svVector3 svPickingray::getRayPostion(int x,
                          int y,
                          ViewProperty &property)
{
    GLfloat winX, winY, winZ;   
    GLdouble posX, posY, posZ;

    winX = (float)x;
    winY = (float)property.viewport[3] - (float)y;

    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
    gluUnProject( winX, winY, winZ, property.mvmatrix, property.projmatrix, property.viewport, &posX, &posY, &posZ); 

    svVector3 p;
    p[0]=posX;p[1]=posY;p[2]=posZ;

    return p;
}

int svPickingray::getSelectedBar(svVector3Array *barEnd1,
                     svVector3Array *barEnd2,
                     svVector3Array *barEnd3,
                     svVector3Array *barEnd4,
                     int seed_num)
{
   svVector3 p1, p2;
   svScalar d1, d2;
   svScalar td=9e+9;
   int index = -1;
    for(int i=0;i<seed_num;i++)
    {
    //    cerr<<barEnd1[i].size()<<endl;
         for(int j=0;j<barEnd1[i].size();j++)
         { 
             svVector3 dir1 = barEnd2[i][j]-barEnd1[i][j];
             svVector3 dir2 = barEnd4[i][j]-barEnd1[i][j]; 
             svVector3 dir = cross(dir1, dir2);
             dir.normalize();
  /*           if(j==0)
             {
                cerr<<barEnd1[i][j][0]<<" "<<barEnd1[i][j][1]<<" "<<barEnd1[i][j][2]
            <<" "<<barEnd2[i][j][0]<<" "<<barEnd2[i][j][1]<<" "<<barEnd2[i][j][2]
<<" "<<barEnd3[i][j][0]<<" "<<barEnd3[i][j][1]<<" "<<barEnd3[i][j][2]
<<" "<<barEnd4[i][j][0]<<" "<<barEnd4[i][j][1]<<" "<<barEnd4[i][j][2]<<endl; 
               cerr<<dir[0]<<" "<<dir[1]<<" "<<dir[2]<<endl;
             } 
*/
             bool flag1 = rayTriangleIntersect(pickingPos, pickingDir,
                        barEnd2[i][j], barEnd1[i][j], barEnd4[i][j],
                        dir,
                        d1, p1);
             bool flag2 = rayTriangleIntersect(pickingPos, pickingDir,
                        barEnd2[i][j], barEnd3[i][j], barEnd4[i][j],
                        dir,
                        d2, p2);
         //    cerr<<d1<<" "<<d2<<" "<<td<<endl;
             if(flag1 || flag2)
             {
                  svScalar d = min(d1, d2); cerr<<d<<endl;
                  if(d < td)
                  {
                       td = d; index = j;
                  }
             }
         }     
    }
/*
    svVector3 tri[3];
    tri[0][0] = 1; tri[0][1] = 0; tri[0][2] = 0;
    tri[1][0] = 0; tri[1][1] = 1; tri[1][2] = 0;
    tri[2][0] = -1; tri[2][1] = 0; tri[2][2] = 0;
    svVector3 v;
    v[0]=0;v[1]=0;v[2]=1;
   svScalar d; svVector3 p;
   bool flag = rayTriangleIntersect(pickingPos, pickingDir,
                        tri[0], tri[2],tri[1],v,
                        d,p);
cerr<<flag<<endl;
*/
    return index;
}

bool  svPickingray::getPlanePosition(svVector3 &pos, svVector3 lb, svVector3 rb)
{
   svVector3 p[4];
   p[0] = lb;
   p[2] = rb;
   p[1] = lb;
   p[3] = rb;
   p[1][0] = rb[0];
   p[3][1] = lb[1];

   svVector3 n = cross(p[1]-p[0],p[3]-p[0]);
   bool flag;
   svScalar distance;
   flag = rayPlaneIntersect(pickingPos, pickingDir,
                p[0], p[1], p[3], p[3],
                n, distance, pos);

   return flag;
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
