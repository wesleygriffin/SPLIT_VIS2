#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <iostream>
#include "svQDOTmagnitudemap.h"

using namespace std;

#ifndef PI
#define PI 3.1415926
#endif

namespace __svl_lib {

svQDOTmagnitudemap::svQDOTmagnitudemap()
{

	font_color[0] = 1;
	font_color[1] = 1;
	font_color[2] = 1;
	
	dataPos = new svVector3[1];
	dataDistance =new svScalar[1];
	Z_format = new svInt[1];
	dataIn = new bool[1];
	dataSize = 0;
}

void svQDOTmagnitudemap::SetData(svVector3 *pos, svScalar *dis,
				int num, 
				svVector3 l, svVector3 r, 
				float x1, float y1, 
				float x2, float y2,
				int xnum, int ynum, 
				float z, int SCALE, double m)
{

    delete [] dataPos;
    delete [] dataDistance;
    delete [] Z_format;
    delete [] dataIn;

   scaling = SCALE;

    dataSize = num;
  dataPos = new svVector3[dataSize];
  dataDistance = new svScalar[dataSize];
  dataIn = new bool[dataSize];

 max = -9e+9;
 min = 9e+9;
 //min_zero = 9e+9;
  
 // Z_format[0] = 0;
  zlayer = 1;//1;
  
  for(int i=0;i<dataSize;i++)
  {
	////cerr<<pos[i][0]<<" ";
	////cerr<<dataSize<<" "<<i<<" "<<dis[i]<<endl;

    dataPos[i] = pos[i];
	
	

	if(dis[i] ==0)
		dataDistance[i] = 0;
	else
{
#ifdef JIAN
   		 dataDistance[i] = log10(dis[i]*pow(10., (float)SCALE));//1 - dis[i];
#else
   		 dataDistance[i] = log10(dis[i]);
#endif
}

	if(max < dataDistance[i])
		max = dataDistance[i];
 
	if(min >dataDistance[i])
		min = dataDistance[i];   

	

    if(i>0)
    {
      if(dataPos[i][2] > dataPos[i-1][2])
      {
	//Z_format[i+1] = i;
	zlayer++;
      }
    }
  }

#ifdef JIAN
 max = log10(m*pow(10., (float)SCALE));
#else
 max = log10(m);
#endif
    
  Z_format = new svInt[zlayer+1];
  startPos = new svVector3[zlayer];

 // //cerr<<zlayer<<endl;


  Z_format[zlayer] = dataSize;
  Z_format[0] = 0;

  int count = 0;
  for(int i=0;i<dataSize;i++)
  {
    
    if(i>0)
    {
      if(dataPos[i][2] > dataPos[i-1][2])
      {
	Z_format[count+1] = i;
	count++;
	////cerr<<count<<endl;
      }
    }
  } 
 
  lb = l;
  rb = r ;
  xd = x1;
  yd = y1;
  xl = x2;
  yl = y2;
  x_num = xnum;
  y_num = ynum;
  zmin = z;
    ////cerr<<zlayer<<endl;
    
 svVector3 tmpr;
  svVector3 tmpl; 
  tmpr[0]=-9e+9;tmpr[1]=-9e+9;tmpr[2]=0;
  tmpl[0]=9e+9;tmpl[1]=9e+9;tmpl[2]=0;  
  svVector3 center;
  
  int t =0;

  for(int i=0;i<zlayer;i++)
  {
    double move_y = ( yl + 2 * xd ) * (i/xnum);
    double move_x = ( xl + 2 * yd ) * (i%xnum);

	//if(i == 1)
	//	cerr<<move_x<<endl;

	startPos[i][0] = lb[0] + move_x;
	startPos[i][1] = lb[1] - move_y;

   for(int j=Z_format[i];j<Z_format[i+1];j++) 
   {
     dataPos[t][0] = dataPos[t][0] + move_x;
     dataPos[t][1] = dataPos[t][1] - move_y;
     
     svVector3 p = dataPos[t];
     
      if(p[0]>tmpr[0])tmpr[0]=p[0];
      if(p[1]>tmpr[1])tmpr[1]=p[1];
      if(p[0]<tmpl[0])tmpl[0]=p[0];
      if(p[1]<tmpl[1])tmpl[1]=p[1];

	t ++;
   }
  }

  tmpl = lb;
  tmpr[0] = lb[0]+ ( xl + 2 * xd ) * xnum;
  tmpr[1] = lb[1]- ( yl + 2 * yd ) * ynum;
    ////cerr<<zlayer<<endl;
  center = (tmpl + tmpr)/2;
  for(int i=0;i<dataSize;i++)
  {
    dataPos[i][0] = dataPos[i][0] - center[0];
    dataPos[i][1] = dataPos[i][1] - center[1];
  }

  for(int i=0;i<zlayer;i++)
  {
	startPos[i][0] = startPos[i][0] - center[0];
	startPos[i][1] = startPos[i][1] - center[1];
  }
}

void svQDOTmagnitudemap::SetInbox(bool *in)
{
	for(int i=0;i<dataSize;i++)
		dataIn[i] = in[i];
}

void svQDOTmagnitudemap::RenderLegend()
{
	glPushMatrix();

	char str[50];
	float x = 0;
	float y = 0;
	float size = 1./4.;	
	float m_length_in = 1;


	double index;
			glColor3f(font_color[0],font_color[1],font_color[2]);
			sprintf(str,"0");
			glRasterPos2f(x+size*4.4, y );
			for(int j=0;j<strlen(str);j++)
			     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);



	for(int i=0;i<100;i++)
	{

			  glColor3f(i/100., i/100., i/100.);


			glBegin(GL_QUADS);
			glVertex2f(x,y);
			glVertex2f(x+size*4,y);
			glVertex2f(x+size*4,y+size);
			glVertex2f(x,y+size);
			glEnd();


			y = y + size;
	
	}
	glColor3f(font_color[0],font_color[1],font_color[2]);
	sprintf(str,"log10(%6.2e)", pow(10.,max)/pow(10., (double)scaling));
	glRasterPos2f(x+size*4.4, y );
	for(int j=0;j<strlen(str);j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glPopMatrix();
}

void svQDOTmagnitudemap::Render(bool *in, float a, float f)
{
	//float f = 3.;
	glEnable (GL_BLEND); 
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	char str[100];
	glEnable(GL_BLEND);
	double alpha = 1.;
	int k = 0;
	glLineWidth(0.25);


	////cerr<<f<<endl;
	//int count = 0;

	if(f>0)
	{
	for(int i=0;i<zlayer;i++)
	{
		for(int j=Z_format[i];j<Z_format[i+1];j++)
		{//count++;
			bool flag = false;
			double xd_sample, yd_sample;

			if(j == Z_format[i])
			{
				flag = true;
				k = Z_format[i];
				xd_sample = xd/2. + xd * (f-1);
				yd_sample = yd/2. + yd * (f-1);
			}
			else
			{
				if(fmod((float)(abs(dataPos[j][1] - dataPos[Z_format[i]][1])),(float)(f*yd))<1e-10
				//&& abs(pos[i][j][1] - pos[i][0][1])>0.0001
				&& abs(dataPos[j][1] - dataPos[j-1][1])>1e-10
				&& abs(dataPos[j][1] - dataPos[Z_format[i+1] - 1][1])>=yd*(f-1))
				{
					//count2 = 1;
					k = j;
					flag = true;
					xd_sample = xd/2. + xd * (f-1);
					yd_sample = yd/2. + yd * (f-1);
				}
				else if(fmod((float)abs(dataPos[j][0] - dataPos[k][0]), (float)(f*xd))<1e-10
				&& abs(dataPos[j][1] - dataPos[k][1])<1e-10
				&& (dataPos[j][0] + (float)f*xd)<=dataPos[k][0]+xl)
				{
					flag = true;
					xd_sample = xd/2. + xd * (f-1);
					yd_sample = yd/2. + yd * (f-1);
				}
			}
			
			if(flag && dataIn[j])
			{
				
				if(in[j])
					alpha = 1;
				else
					alpha = a;
				
				//vel2[i][j] = 1- vel2[i][j];//cos !!!
			
				/*if(((fabs(vel2[i][j])<4&&(vel2[i][j]>0||fabs(vel2[i][j])<0.0001)&&z!=pos[i][0][2])
				|| z == pos[i][0][2])
				&& !(fabs(dataColor[0][count][0])<0.0001 
				&& fabs(dataColor[0][count][1])<0.0001 
				&& fabs(dataColor[0][count][2])<0.0001 ))
				{*/
					//glColor4f(0.,0.,0., 1.);
					glColor4f(dataDistance[j]/max,dataDistance[j]/max,dataDistance[j]/max,alpha);
					/*glBegin(GL_QUADS);
					glVertex2f(pos[i][j][0]-xd/2.+0.05,pos[i][j][1]-yd_sample+0.05);
					glVertex2f(pos[i][j][0]+xd_sample-0.05,pos[i][j][1]-yd_sample+0.05);
					glVertex2f(pos[i][j][0]+xd_sample-0.05,pos[i][j][1]+yd/2.-0.05);
					glVertex2f(pos[i][j][0]-xd/2.+0.05,pos[i][j][1]+yd/2.-0.05);
					glEnd();
*/
					double xxd, yyd;
					xxd = (xd/2.+xd_sample-0.1) /2.;//M_PI;
					yyd = (yd/2.+yd_sample-0.1) /3.;
					double y_shift;
					y_shift = yd_sample - (yd_sample + yd/2.)/2.;

					//xxd = xd/2.-0.005;
	
					/*glBegin(GL_QUADS);
					glVertex2f(pos[i][j][0]-xd/2.+0.05,pos[i][j][1]-yd_sample+0.05);
					glVertex2f(pos[i][j][0]+xd_sample-0.05,pos[i][j][1]-yd_sample+0.05);
					glVertex2f(pos[i][j][0]+xd_sample-0.05,pos[i][j][1]+yd/2.-0.05);
					glVertex2f(pos[i][j][0]-xd/2.+0.05,pos[i][j][1]+yd/2.-0.05);
					glEnd();
*/
					//glColor4f(0.,0.,0., 1.);
					glBegin(GL_QUADS);
					//glBegin(GL_LINE_LOOP);
					//glVertex2f(pos[i][j][0]-xd/2.+0.05,pos[i][j][1]-yyd/2.-y_shift);
					//glVertex2f(pos[i][j][0]+xxd,pos[i][j][1]-yyd/2.-y_shift);
					//glVertex2f(pos[i][j][0]+xxd,pos[i][j][1]+yyd/2.-y_shift);
					//glVertex2f(pos[i][j][0]-xd/2.+0.05,pos[i][j][1]+yyd/2.-y_shift);
					glVertex2f(dataPos[j][0],dataPos[j][1]-yd_sample+0.05);//-xd/2.+0.05
					glVertex2f(dataPos[j][0]+xxd,dataPos[j][1]-yd_sample+0.05);
					glVertex2f(dataPos[j][0]+xxd,dataPos[j][1]+yd/2.-0.05);
					glVertex2f(dataPos[j][0],dataPos[j][1]+yd/2.-0.05);//-xd/2.+0.05
					glEnd();
					/*glColor4f(0.,0.,0., 1.);
					glBegin(GL_LINE_LOOP);
					glVertex2f(pos[i][j][0]-xd/2.+0.05,pos[i][j][1]-yd_sample+0.05);
					glVertex2f(pos[i][j][0]+xd_sample-0.05,pos[i][j][1]-yd_sample+0.05);
					glVertex2f(pos[i][j][0]+xd_sample-0.05,pos[i][j][1]+yd/2.-0.05);
					glVertex2f(pos[i][j][0]-xd/2.+0.05,pos[i][j][1]+yd/2.-0.05);
					glEnd();
*/


					//count ++; 
				/*}
				else
				{
					//if(pos[i][j][2] == 7.)
						////cerr<<pos[i][j][0]<<" "<<pos[i][j][1]<<" "<<vel2[i][j]<<endl;
				}*/
			}

			
		}
////cerr<<decide_layer<<endl;



		//if(i%4==0){

		glColor3f(font_color[0],font_color[1],font_color[2]);
		sprintf(str,"%.2f",dataPos[Z_format[i]][2]);
		glRasterPos2f(dataPos[Z_format[i]][0],dataPos[Z_format[i]][1]);
        	for(int j=0;j<strlen(str);j++)
            		 glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
		//}
		
	}

	}

////cerr<<count<<endl;
	glLineWidth(1.);
}


void svQDOTmagnitudemap::cleanup()
{
	delete [] dataDistance;
	delete [] dataPos;
	delete [] Z_format;  
	delete [] dataIn;
}

}
