#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <iostream>
#include "svQDOTdistancemap.h"

using namespace std;

#ifndef PI
#define PI 3.1415926
#endif

namespace __svl_lib {

svQDOTdistancemap::svQDOTdistancemap(svVector3 *pos, 
		     int num, 
		     svVector3 l, svVector3 r, 
		     float x1, float y1, 
		     float x2, float y2,
		     int xn, int yn, float z):svMap(pos, num, l, r, x1, y1, x2, y2, xn, yn,z)
{
	selected_layer = 0;
	Selected= false;

	select_index = -1;
	select_layer = 0;
	decide_layer = 0;

	font_color[0] = 1;
	font_color[1] = 1;
	font_color[2] = 1;
	
	dataColor = new svVector4[dataSize];
	dataDistance =new svScalar[dataSize];
	startPos = new svVector3[mapPos.size()];

  	svVector3 center;
  	center[0] = mapPos[0][0][0] - dataPos[0][0];
  	center[1] = mapPos[0][0][1] - dataPos[0][1];
  	for(int i=0;i<mapPos.size();i++)
  	{
    		double move_y = ( yl + 2 * yd ) * (i/xnum);
    		double move_x = ( xl + 2 * xd ) * (i%xnum);
		startPos[i][0] = lbbox[0] + move_x;
		startPos[i][1] = lbbox[1] - move_y;
		startPos[i][0] = startPos[i][0] + center[0];
		startPos[i][1] = startPos[i][1] + center[1];
  	}

	ni = -1;
	nj = -1;
}

void svQDOTdistancemap::New(svVector3 *pos, 
		     int num, 
		     svVector3 l, svVector3 r, 
		     float x1, float y1, 
		     float x2, float y2,
		     int xn, int yn, float z)
{
	setData(pos, num, l, r, x1, y1, x2, y2, xn, yn,z);

	Selected= false;

	if(selected_layer >= mapPos.size())
	{
		selected_layer = 0;

		select_index = -1;
		select_layer = 0;
		decide_layer = 0;
	}

	delete [] dataColor;
	delete [] dataDistance;
	delete [] startPos;
	
	dataColor = new svVector4[dataSize];
	dataDistance =new svScalar[dataSize];
	startPos = new svVector3[mapPos.size()];

  	svVector3 center;
  	center[0] = mapPos[0][0][0] - dataPos[0][0];
  	center[1] = mapPos[0][0][1] - dataPos[0][1];
  	for(int i=0;i<mapPos.size();i++)
  	{
    		double move_y = ( yl + 2 * yd ) * (i/xnum);
    		double move_x = ( xl + 2 * xd ) * (i%xnum);
		startPos[i][0] = lbbox[0] + move_x;
		startPos[i][1] = lbbox[1] - move_y;
		startPos[i][0] = startPos[i][0] + center[0];
		startPos[i][1] = startPos[i][1] + center[1];
  	}

	ni = -1;
	nj = -1;
}

void svQDOTdistancemap::SetDistance(svScalar *dis)
{


  for(int i=0;i<dataSize;i++)
  {
    dataDistance[i] = dis[i];//1 - dis[i];
    
  }

}


void svQDOTdistancemap::SetColors(svVector4 *col)
{

	int count = 0;
	  for(int i=0;i<dataSize;i++)
	  {
	    dataColor[i] = col[i];
		if(!(fabs(dataColor[i][0] -1)<1e-18 && fabs(dataColor[i][1] -1)<1e-18 &&fabs(dataColor[i][2] -1)<1e-18))
			count++;
	  }
	cerr<<"count"<<count<<endl;
	
}

void svQDOTdistancemap::Render(float a, float f)
{
	//float f = 3.;
	glEnable (GL_BLEND); 
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	char str[20];
	double alpha = 1.;
	int k = 0;
	glLineWidth(0.25);


	//cerr<<decide_layer<<endl;

	if(f>0)
	{
		for(int i=0;i<mapPos.size();i++)
		{
			//cerr<<i<<endl;
			for(int j=0;j<mapPos[i].size();j++)
			{//count++;
				bool flag = false;
				double xd_sample, yd_sample;

				if(j == 0)
				{
					flag = true;
					k = 0;
					xd_sample = xd/2. + xd * (f-1);
					yd_sample = yd/2. + yd * (f-1);
				}
				else
				{
					if(fmod((float)(abs(mapPos[i][j][1] - mapPos[i][0][1])),(float)(f*yd))<1e-10
					&& abs(mapPos[i][j][1] - mapPos[i][j-1][1])>1e-10
					&& abs(mapPos[i][j][1] - mapPos[i][mapPos[i].size()-1][1])>=yd*(f-1))
					{
						//count2 = 1;
						k = j;
						flag = true;
						xd_sample = xd/2. + xd * (f-1);
						yd_sample = yd/2. + yd * (f-1);
					}
					else if(fmod((float)abs(mapPos[i][j][0] - mapPos[i][k][0]), (float)(f*xd))<1e-10
					&& abs(mapPos[i][j][1] - mapPos[i][k][1])<1e-10
					&& (mapPos[i][j][0] + (float)f*xd)<=mapPos[i][k][0]+xl)
					{
						flag = true;
						xd_sample = xd/2. + xd * (f-1);
						yd_sample = yd/2. + yd * (f-1);
					}
				}
				if(flag)
				{
					if(dataROI[dataIndex[i][j]])
						alpha = 1;
					else
						alpha = a;

					int jj = dataIndex[i][j];

					glColor4f(dataColor[jj][0],dataColor[jj][1],dataColor[jj][2],alpha);

					double xxd, yyd;
					xxd = (xd/2.+xd_sample-0.1)* dataDistance[jj]/2.;//M_PI;
					yyd = (yd/2.+yd_sample-0.1) /3.;
					double y_shift;
					y_shift = yd_sample - (yd_sample + yd/2.)/2.;

					glBegin(GL_QUADS);
					glVertex2f(mapPos[i][j][0],mapPos[i][j][1]-yd_sample+0.05);//-xd/2.+0.05
					glVertex2f(mapPos[i][j][0]+xxd,mapPos[i][j][1]-yd_sample+0.05);
					glVertex2f(mapPos[i][j][0]+xxd,mapPos[i][j][1]+yd/2.-0.05);
					glVertex2f(mapPos[i][j][0],mapPos[i][j][1]+yd/2.-0.05);//-xd/2.+0.05
					glEnd();
					//if(Selected)
						//cerr<<"selected"<<Selected<<" "<<ni<<" "<<nj<<endl;
					if(i == ni && j == nj)
					{
						//cerr<<ni<<" "<<nj<<endl;
						glColor3f(0.,0.,0.);
						glBegin(GL_LINE_LOOP);
						glVertex2f(mapPos[i][j][0],mapPos[i][j][1]-yd_sample+0.05);//-xd/2.+0.05
						glVertex2f(mapPos[i][j][0]+xxd,mapPos[i][j][1]-yd_sample+0.05);
						glVertex2f(mapPos[i][j][0]+xxd,mapPos[i][j][1]+yd/2.-0.05);
						glVertex2f(mapPos[i][j][0],mapPos[i][j][1]+yd/2.-0.05);//-xd/2.+0.05
						glEnd();
					}

				}

			
			}

			glLineWidth(5.);

			if(i == decide_layer)
			{
				glColor3f(1.,1.,0.);
				glBegin(GL_LINE_LOOP);
				glVertex2f(startPos[i][0], startPos[i][1]);
				glVertex2f(startPos[i][0]+(rbbox[0] - lbbox[0]), startPos[i][1]);
				glVertex2f(startPos[i][0]+(rbbox[0] - lbbox[0]), startPos[i][1]+(rbbox[1]-lbbox[1]));
				glVertex2f(startPos[i][0], startPos[i][1]+(rbbox[1]-lbbox[1]));
				glEnd();			
			}
			else if(i == select_layer)
			{
				glColor3f(1.,0.,0.);
				glBegin(GL_LINE_LOOP);
				glVertex2f(startPos[i][0], startPos[i][1]);
				glVertex2f(startPos[i][0]+(rbbox[0] - lbbox[0]), startPos[i][1]);
				glVertex2f(startPos[i][0]+(rbbox[0] - lbbox[0]), startPos[i][1]+(rbbox[1]-lbbox[1]));
				glVertex2f(startPos[i][0], startPos[i][1]+(rbbox[1]-lbbox[1]));
				glEnd();			
			}

			glLineWidth(1.);


			glColor3f(font_color[0],font_color[1],font_color[2]);
			sprintf(str,"%.2f",splitPos[i][0][2]);
			glRasterPos2f(mapPos[i][0][0],mapPos[i][0][1]);
			for(int j=0;j<strlen(str);j++)
		    		 glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
		
		}							

	}
	glLineWidth(1.);


}


int svQDOTdistancemap::SelectPoint(float x, float y, float f)
{
	int count = 0;
	int count2 = 0;
	int k = 0;

	Selected = false;

	//cerr<<x<<" "<<y<<" "<<f<<endl;

	if(f > 0)
	{
		for(int i=0;i<mapPos.size();i++)
		{
			for(int j=0;j<mapPos[i].size();j++)
			{
				bool flag = false;
				double xd_sample, yd_sample;

				if(j == 0)
				{
					flag = true;
					k = 0;
					xd_sample = xd/2. + xd * (f-1);
					yd_sample = yd/2. + yd * (f-1);
				}
				else
				{
					if(fmod((float)(abs(mapPos[i][j][1] - mapPos[i][0][1])),(float)(f*yd))<1e-10
					&& abs(mapPos[i][j][1] - mapPos[i][j-1][1])>1e-10
					&& abs(mapPos[i][j][1] - mapPos[i][mapPos[i].size()-1][1])>=yd*(f-1))
					{
						//count2 = 1;
						k = j;
						flag = true;
						xd_sample = xd/2. + xd * (f-1);
						yd_sample = yd/2. + yd * (f-1);
					}
					else if(fmod((float)abs(mapPos[i][j][0] - mapPos[i][k][0]), (float)(f*xd))<1e-10
					&& abs(mapPos[i][j][1] - mapPos[i][k][1])<1e-10
					&& (mapPos[i][j][0] + (float)f*xd)<=mapPos[i][k][0]+xl)
					{
						flag = true;
						xd_sample = xd/2. + xd * (f-1);
						yd_sample = yd/2. + yd * (f-1);
					}

				}

				if(flag)// && dataROI[dataIndex[i][j]])
				{
					double xxd, yyd;
					xxd = (xd/2.+xd_sample-0.1) * dataDistance[dataIndex[i][j]]/2.;//M_PI;
					yyd = (yd/2.+yd_sample-0.1) /3.;

					//if(i == 0)
						//cerr<<x<<" "<<y<<" "<<mapPos[i][j][0]<<" "<<mapPos[i][j][1]<<" "<<Selected<<" "<<xxd<<" "<<yd_sample<<" "<<yd<<endl;
						if(x>=mapPos[i][j][0]
						&& x<=mapPos[i][j][0]+xxd
						&& y>=mapPos[i][j][1]-yd_sample+0.05
						&& y<=mapPos[i][j][1]+yd/2.-0.05)
						{

							Selected = true;
							select_index = dataIndex[i][j];
							ni =i;nj=j;

							//cerr<<"!"<<endl;
							break;
						}

				}

			
			}
			if(Selected)
			{
				break;
			}
		}
	}

	//cerr<<"s"<<Selected<<" "<<ni<<" "<<nj<<endl;
	return select_index;
}


int svQDOTdistancemap::SelectBox(int x, int y, float scale_x, float scale_y, float zoom, float xpan, float ypan)
{
	float mx = (float)x;
	float my = (float)y;
	int j1,j2;
	
	svVector3 p;
	p = mapPos[0][0];
	p[1] = mapPos[0][0][1] +  yl + yd*2;
	svVector3 l = lbbox;
	svVector3 r = rbbox;

	p[0] = p[0] * zoom + xpan * zoom+ scale_x;
	p[1] = p[1] * zoom + ypan* zoom + scale_y;
	r[0] = r[0] * zoom + xpan* zoom + scale_x;
	r[1] = r[1] * zoom + ypan* zoom + scale_y;
	l[0] = l[0] * zoom + xpan* zoom + scale_x;
	l[1] = l[1] * zoom + ypan* zoom + scale_y;


	j1 = -1;
	j2 = -1;

	for(int i=0;i<xnum;i++)
	{
		if(mx>p[0] + (r[0] - l[0] + xd*2) * i
			&& mx <p[0] + (r[0] - l[0]+ xd*2) * (i+1))
		{
			j1 = i;
		}
	}

	//p[1] = p[1] + (r[1] - l[1]);
	for(int i=0;i<ynum+1;i++)
	{
		if(my < p[1] - (r[1] - l[1]+ yd*2) * (i-1)
			&& my > p[1]-(r[1] - l[1]+ yd*2) * (i))
		{
			j2 = i;
			j2 = j2 - 1;
		}		
	}

	/////cerr<<j1<<" "<<j2<<endl;

	float j;
	if(j1 >= 0 && j2>=0)
	{
		select_layer = j1 + j2 * (xnum);
		//j = l[2] + zmin * select_layer;
	}
	else
	{
		//j = l[2] - 1;
	}
	////cerr<<j<<" "<<zmin<<endl;
	return select_layer ;
}


void svQDOTdistancemap::SetSelectedLayer(int n)
{
    if(n < mapPos.size())
    {
      select_layer = n;
    }
    else
    {
     select_layer =  mapPos.size()-1;
    }
}

void svQDOTdistancemap::cleanup()
{
	delete [] dataDistance;
	delete [] dataColor;
}

}
