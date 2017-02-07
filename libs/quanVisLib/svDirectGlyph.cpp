

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
#define DEFAULT_GLYPH_SCALE  1
#define DEFAULT_GLYPH_SIZE  0.3

using namespace std;

namespace __svl_lib {
svDirectArrow::svDirectArrow(svChar *infname)
           : svGlyph(infname, 0)
{
  // only handle internal format
  ENABLE_COLOR=false;
  ENABLE_LINEWIDTH = false;

  display_mode = SV_STORE;
  lbbox=svVector3(-2.,-2.,-2.);
  rbbox=svVector3(2.,2.,2.);

  glyph = NULL;
  glyphColors = NULL;
  glyphWidth = NULL;
  glyphScale = DEFAULT_GLYPH_SCALE;
  glyphSize= DEFAULT_GLYPH_SIZE;
}

/*
svDirectArrow::svDirectArrow(svVector3 _lbbox, 
   svVector3 _rbbox, svChar *inf) : svGlyph(inf, 0)
{
  display_mode = SV_STORE;

  show_eigen_vector = false;
  lbbox=_lbbox;
  rbbox=_rbbox;

  glyph = NULL;
  glyphColors = NULL;
  glyphWidth = NULL;
  glyphScale = DEFAULT_GLYPH_SCALE;
  glyphSize=DEFAULT_GLYPH_SIZE;
}
*/

svDirectArrow::~svDirectArrow()
{ 
}

void svDirectArrow::BuildDisplayListFromStore()
{ 
}

// only handle internal format
//void svDirectArrow::Generate() // this read specific data format
//{
//  char lineptr[256];
//  svGlyph::STILL_UPDATE = true;

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
  //cleanDisplayList();
  //BuildDisplayListFromStore();
  //cleanStreamLine();
  //display_mode = SV_DISPLAYLIST;
 // svGlyph::STILL_UPDATE = false;
//}

void svDirectArrow::geometryDraw(double base)
{

		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		  glBegin(GL_POLYGON);
		 // glNormal3f(0,0,1);
		  //glNormal3f(0,0,-1);
		  for (int i=0; i<=360; i++)
		  {
		     float degInRad = (float)i/180. * PI;
		     glVertex3f(cos(degInRad)*base,sin(degInRad)*base,0);
		  }
		  glEnd();
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);


}

void svDirectArrow::Generate(svVector3 *pos3in, svVector3 *vec3in, svScalar *denin,
						svVector4 *color4in, svInt dataSize, svInt *dataAtom,
						float alpha)
{
	clean();

	seed_num = 1;

	glyph = new svVector3Array[2];
	glyphColors = new svVector4Array[1];
	dir = new svVector3Array[1];
	mag = new svScalarArray[1];

	double magmax =-1;

	for(int i=0;i<1;i++)
	{
		for(int j=0;j<dataSize;j++)
		{
			glyph[i].add(pos3in[j]);
			glyphColors[i].add(color4in[j]);
			dir[i].add(vec3in[j]);
			mag[i].add(denin[j]);

			if(denin[j]>magmax)magmax = denin[j];
		}
	}

	//-----------------------------display-list------------------------------
    if(glIsList(display_list))
		glDeleteLists(display_list, 1); 
    glNewList(display_list, GL_COMPILE);
    
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

    int i;
    int count = 0;
	
    for(int ii =0;ii<dataSize;ii++)
    {		
		i = ii;

		glColor4f(glyphColors[0][i][0],glyphColors[0][i][1],glyphColors[0][i][2],alpha);			  
			
		double radius = ARROW_BASE;

		radius = radius * (mag[0][i]/magmax);

		//-----------------rotation------------------------------
		double angle_x = acos(dir[0][i][2]);
		if(dir[0][i][1] > 0)
		{
			angle_x = - angle_x;
		}
		double xy_project = dir[0][i][0] * dir[0][i][0] + dir[0][i][1] * dir[0][i][1];
		xy_project = sqrt(xy_project);
		double angle_z = acos(dir[0][i][1]/xy_project);
		if(angle_x < 0)
		{	
				if (dir[0][i][0] > 0)
				 {
					angle_z =  -angle_z;
				  }
		}
		else
		{
				angle_z = 3.1415926 - angle_z;
				if(dir[0][i][0] < 0)
					angle_z =  -angle_z;		
		}

		//-----------------endpoint-----------------------
		svVector3 end;
		end[0] = glyph[0][i][0]+mag[0][i]*glyphScale*dir[0][i][0];
		end[1] = glyph[0][i][1]+mag[0][i]*glyphScale*dir[0][i][1];
		end[2] = glyph[0][i][2]+mag[0][i]*glyphScale*dir[0][i][2];

		//------------------render -------------------
		glPushMatrix();

		glTranslatef(end[0],end[1],end[2]);
		glRotatef(angle_z/3.1415926 * 180.,0,0,1);
		glRotatef(angle_x/3.1415926 * 180.,1,0,0);

		geometryDraw(radius*1.5); 

		glPopMatrix();

		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);

		//-------------------straight line or string-------------
		if(dataAtom[i] == 1)
		{
				glLineWidth(3.);
				//glColor4f(glyphColors[0][i][0],glyphColors[0][i][1],glyphColors[0][i][2],alpha);

				glBegin(GL_LINES);
				glVertex3f(glyph[0][i][0],glyph[0][i][1],glyph[0][i][2]);
				glVertex3f(end[0],end[1],end[2]);
				glEnd();
				
				glLineWidth(6.);
				glColor4f(0.,0.,0.,alpha);
				glBegin(GL_LINES);
				glVertex3f(glyph[0][i][0],glyph[0][i][1],glyph[0][i][2]);
				glVertex3f(end[0],end[1],end[2]);
				glEnd();
		}
		else if(dataAtom[i] == 2)
		{
				glPushMatrix();

				glTranslatef(glyph[0][i][0],glyph[0][i][1],glyph[0][i][2]);
				glRotatef(angle_z/3.1415926 * 180.,0,0,1);
				glRotatef(angle_x/3.1415926 * 180.,1,0,0);

				double coil;
				coil = mag[0][i]/pow(10.,(double)(SCALE-6));
				if(coil<1) coil = 10;				
				glLineWidth(3.);
				DrawSpring(glyph[0][i], end, radius/2., radius/2. , 10);

				glLineWidth(5.);
				glColor4f(0.,0.,0.,alpha);
				DrawSpring(glyph[0][i], end, radius/2., radius/2., 10);
				
				glPopMatrix();
		}


		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}	

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

	glLineWidth(1.);
	
	glEndList();
}

void svDirectArrow::Render()
{
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glColor3fv(render_property.color.getValue());
  //if(display_mode == SV_IMMEDIATE) {
    //Generate();
  //}
  /*else*/ 
  if(display_mode == SV_DISPLAYLIST && glIsList(display_list)) {
    glCallList(display_list);
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
       glutSolidSphere(glyphSize,10,10);
       glPopMatrix();
     }; 
     glDisable(GL_LIGHTING);
  };
}


}
