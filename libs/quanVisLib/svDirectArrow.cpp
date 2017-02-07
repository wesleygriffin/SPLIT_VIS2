

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include "svDirectArrow.h"
#include "svException.h"
#include "svUtil.h"

// should change later - add to renderproperty


using namespace std;

namespace __svl_lib {
svDirectArrow::svDirectArrow(svVectorField *f):svGlyph()//svVectorField* f, int numPlane)
{
  field = f;
  // only handle internal format
  ENABLE_COLOR=false;
  ENABLE_LINEWIDTH = false;

  display_mode = SV_STORE;
  field->GetPhysicalDimension(&lbbox, &rbbox);

//  glyphScale = DEFAULT_GLYPH_SCALE;
//  glyphSize= DEFAULT_GLYPH_SIZE;
//  glyphRadius = DEFAULT_GLYPH_RADIUS;
//  display_list = DEFAULT_DISPLAYLIST;
}


void svDirectArrow::Generate() // this read specific data format
{
 // svGlyph::STILL_UPDATE = true;

  //numOfCriticalPoints = GetNumberOfCriticalPoints(infile);
  //seed_num = numOfCriticalPoints;

  //cerr <<"num of criticalpoints = " <<  numOfCriticalPoints << endl;
  //eigen_vectors = new svVector3[numOfCriticalPoints*3];
  //critical_point_type = new svUint[numOfCriticalPoints];
  //glyph= new svVector3Array[numOfCriticalPoints]; 

  //readCriticalPointInfo(infile, glyph, 
   //      eigen_values_r, eigen_values_i, eigen_vectors,
    //     critical_point_type, seed_num);

//cerr << "====================================" << endl;
//cerr << "SVCRTICAL POINT GLYPH: NUM OF CRTICAL POINTS ==>  " << seed_num << endl;
//cerr << "====================================" << endl;
  // write to display list
  //cleanDisplayList(SV_DISPLAYLIST);
  //BuildDisplayListFromStore();
  //cleanStreamLine();

 // if(isContour)
 // {
      //ComputeContours();
  
      /*for(int i=0;i<seed_num;i++)
      {
           if(contourLabel[i])
           {
                   SetContourData(i);
           }
      }*/
 // }

  display_mode = SV_DISPLAYLIST;
  //display_list = 1;
  svGlyph::STILL_UPDATE = false;

	if(glIsList(display_list))
		glDeleteLists(display_list, 1); 
    glNewList(display_list, GL_COMPILE);
    
    for(int i =0;i<seed_num;i++)
    {		
		//cerr<<i<<" "<<glyph[i].size()<<endl;
		for(int j=0;j<glyph[i].size();j++)
		{
			if(roiLabel[i][j])
			{
			glColor4f(glyphColors[i][j][0],glyphColors[i][j][1],glyphColors[i][j][2],alpha);

			double radius = mag[i][j] * glyphRadius;
			

			//-----------------rotation------------------------------
			double angle_x = acos(dir[i][j][2]);
			if(dir[i][j][1] > 0)
			{
				angle_x = - angle_x;
			}
			double xy_project = dir[i][j][0] * dir[i][j][0] + dir[i][j][1] * dir[i][j][1];
			xy_project = sqrt(xy_project);
			double angle_z = acos(dir[i][j][1]/xy_project);
			if(angle_x < 0)
			{	
					if (dir[i][j][0] > 0)
					 {
						angle_z =  -angle_z;
					  }
			}
			else
			{
					angle_z = 3.1415926 - angle_z;
					if(dir[i][j][0] < 0)
						angle_z =  -angle_z;		
			}

			//-----------------endpoint-----------------------
			svVector3 end;
			end[0] = glyph[i][j][0]+mag[i][j]*glyphScale*dir[i][j][0];
			end[1] = glyph[i][j][1]+mag[i][j]*glyphScale*dir[i][j][1];
			end[2] = glyph[i][j][2]+mag[i][j]*glyphScale*dir[i][j][2];
			
			//cerr<<mag[i][j]<<" "<<glyphScale<<" "<<dir[i][j][0]<<" "<<end[0]<<" "<<glyph[i][j][0]<<endl;

			//------------------render -------------------
			glPushMatrix();

			glTranslatef(end[0],end[1],end[2]);
			glRotatef(angle_z/PI * 180.,0,0,1);
			glRotatef(angle_x/PI * 180.,1,0,0);

			double base = radius * 1.5;
			 glutSolidCone(base, base*3, 4, 1);
			glPopMatrix();

			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);

			//-------------------straight line or string-------------

			glLineWidth(3.);

			glBegin(GL_LINES);
			glVertex3f(glyph[i][j][0],glyph[i][j][1],glyph[i][j][2]);
			glVertex3f(end[0],end[1],end[2]);
			glEnd();
					
			glLineWidth(6.);
			glColor4f(0.,0.,0.,alpha);
			glBegin(GL_LINES);
			glVertex3f(glyph[i][j][0],glyph[i][j][1],glyph[i][j][2]);
			glVertex3f(end[0],end[1],end[2]);
			glEnd();
			
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			}
		}	
	}	

    //glDisable(GL_LIGHTING);
    //glDisable(GL_LIGHT0);

	glLineWidth(1.);
	
	glEndList();
}

void svDirectArrow::Render()
{
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
 // glColor3fv(render_property.color.getValue());
  
   
  //if(display_mode == SV_IMMEDIATE) {
    //Generate();
  //}
  /*else*/ 
  if(display_mode == SV_DISPLAYLIST && glIsList(display_list)) {
    glCallList(display_list);
   // cerr<<display_list<<endl;
  }
  else if (display_mode == SV_STORE) {
    RenderStore();
  }
  glEnable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
}

void svDirectArrow::RenderStore() const
{
  if(svPrimitive::STILL_UPDATE ) 
  {
    return;
  }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);     
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    glEnable( GL_LINE_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
  for(int i=0; i<seed_num; i++)
  {
     glEnable(GL_LIGHTING);
     glEnable(GL_NORMALIZE);

     glLineWidth(1.0);
     glColor3f(0.9, 0.9f, 0.2);
     svVector3 vel=glyph[i][0].getValue();
     if(vel[0]>lbbox[0] && vel[0]<rbbox[0] &&
        vel[1]>lbbox[1] && vel[1]<rbbox[1] &&
        vel[2]>lbbox[2] && vel[2]<rbbox[2])
     {
       glPushMatrix();
       glTranslatef(vel[0],vel[1],vel[2]);
//       glutSolidSphere(glyphSize,10,10);
       glPopMatrix();
     }; 
     glDisable(GL_LIGHTING);
  };
}


}
