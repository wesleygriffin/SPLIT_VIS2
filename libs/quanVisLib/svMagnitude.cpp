#include "svMagnitude.h"
#include <string.h>
#include <fstream>
#include <algorithm>
#include <cmath>
//#include "parameters.h"
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifndef PI
#define PI 3.1415926
#endif

#ifndef PARAMETERMAX
#define PARAMETERMAX 5.
#endif

#ifndef PARAMETERMIN
#define PARAMETERMIN 0.1
#endif


#define TIMES 10
float xscale=18.;
float yscale=10.;
float xratio = 9./10.;
float yratio = 7./8.;
float slice;
using namespace std;

namespace __svl_lib
{
///======if(segment == 2): w is a widget and f is a fixed point=========
/// ^
/// |                     w
/// |      w              |
/// |      |              |
/// |w     |              |
/// -f-----w--------------f---->
/*svMagnitude::svMagnitude()
{
	level = 0;
	SELECTED = GL_FALSE;
}*/


void svMagnitude::Init(const MagnitudeWidgetProperty & widgetproperty)
{
    cleanup();

    svVector3 pos;
    pos[0]=0;pos[1]=0;pos[2]=0;
                                                                      
    for(int i=0;i<(widgetproperty.segment-1)*2;i++)
    {
          widgets_pos.add(pos);
    }
    widgets_pos.add(pos);
    widgets_pos.add(pos);

    for(int i=0;i<SCALE_WIDGETS_NUM;i++)
    {
        widgets_scale_pos.add(pos);
    }

    widgets_scale_status = WIDGETS_DEFAULT;
    
    property = widgetproperty;

///===================================================================
    svScalar xsegment = (svScalar)(1./(widgets_pos.size()-2));
    svScalar ysegment = (svScalar)(1./(widgets_pos.size()-2));
    widgets_pos[0][0]=0;widgets_pos[0][1]=0;widgets_pos[0][2]=0;
 
    for(int i=1;i<widgets_pos.size()-1;i++)
    {
       widgets_pos[i][0]= xsegment * i;
       if(i%2 == 0)
            widgets_pos[i][1] = 0;
       else
            widgets_pos[i][1] = ysegment * i;
       widgets_pos[i][2] = 0;
    }    
    int j = widgets_pos.size()-1;
    widgets_pos[j][0]=1;widgets_pos[j][1]=1;widgets_pos[j][2]=0;

    for(int i=0;i<widgets_pos.size();i++)
    {
         widgets_pos[i][0] = widgets_pos[i][0] * property.scalex + property.transx;
         widgets_pos[i][1] = widgets_pos[i][1] * property.scaley + property.transy;
    }
///
///---
/// |
/// |
/// w
/// |
/// |
///---
    widgets_scale_pos[0][0]=-0.5;
    widgets_scale_pos[0][1]=0;
    widgets_scale_pos[0][2]=0;
    widgets_scale_pos[1][0]=-0.5;//5 pixel left
    widgets_scale_pos[1][1]=0.5;
    widgets_scale_pos[1][2]=0;
    widgets_scale_pos[2][0]=-0.5;//5 pixel left
    widgets_scale_pos[2][1]=1;
    widgets_scale_pos[2][2]=0;

    for(int i=0;i<SCALE_WIDGETS_NUM;i++)
    {
         widgets_scale_pos[i][0] = widgets_pos[i][0] * property.scalex + property.transx;
         widgets_scale_pos[i][1] = widgets_pos[i][1] * property.scaley + property.transy;
    }

    widgets_max_value = property.maxscale / 2.;
}

void svMagnitude::Render()
{
     glDisable(GL_LIGHTING);
     glDisable(GL_LIGHT0);
     glDisable(GL_TEXTURE_2D);

     glPointSize(property.pointsize);
     
     if(widgets_scale_status==WIDGETS_DEFAULT)
          glColor3f(property.widgets_default_color[0], 
                  property.widgets_default_color[1], 
                  property.widgets_default_color[2]);
     else if(widgets_scale_status==WIDGETS_SELECT)
          glColor3f(property.widgets_select_color[0],
                  property.widgets_select_color[1],
                  property.widgets_select_color[2]);
     else if(widgets_scale_status==WIDGETS_TOUCH)
          glColor3f(property.widgets_touch_color[0],
                  property.widgets_touch_color[1],
                  property.widgets_touch_color[2]);
     glBegin(GL_POINTS);
     glVertex2f(widgets_scale_pos[1][0],
               widgets_scale_pos[1][1]);
     glEnd();

     glBegin(GL_POINTS);
     for(int i=0;i<widgets_pos.size();i++)
     {
          if(widgets_status[i]==WIDGETS_DEFAULT)
              glColor3f(property.widgets_default_color[0],
                  property.widgets_default_color[1],
                  property.widgets_default_color[2]);
         else if(widgets_status[i]==WIDGETS_SELECT)
              glColor3f(property.widgets_select_color[0],
                  property.widgets_select_color[1],
                  property.widgets_select_color[2]);
         else if(widgets_status[i]==WIDGETS_TOUCH)
              glColor3f(property.widgets_touch_color[0],
                  property.widgets_touch_color[1],
                  property.widgets_touch_color[2]);

         glVertex2f(widgets_pos[i][0],
               widgets_pos[i][1]);
     }
     glEnd();

//================================
     glColor3f(property.font_color[0], 
            property.font_color[1], 
            property.font_color[2]);
     glBegin(GL_POINTS);
     glVertex2f(widgets_scale_pos[0][0],
               widgets_scale_pos[0][1]);
     glVertex2f(widgets_scale_pos[2][0],
               widgets_scale_pos[2][1]);
     glEnd();

     glPointSize(1);

//================================
     glBegin(GL_LINES);
     glVertex2f(widgets_scale_pos[0][0],
               widgets_scale_pos[0][1]);
     glVertex2f(widgets_scale_pos[2][0],
               widgets_scale_pos[2][1]);
     glVertex2f(widgets_pos[0][0],
               widgets_pos[0][1]);
     glVertex2f(widgets_pos[1][0],
               widgets_pos[1][1]);
     for(int i=1;i<widgets_pos.size()-2;i++)
     {
        if(i%2 == 1)
        {
           glVertex2f(widgets_pos[i][0],
               widgets_pos[i][1]);
           glVertex2f(widgets_pos[i+2][0],
               widgets_pos[i+2][1]);
        }
     }
     for(int i=1;i<widgets_pos.size()-2;)
     {
           glVertex2f(widgets_pos[i][0],
               widgets_pos[i][1]);
           glVertex2f(widgets_pos[i+1][0],
               widgets_pos[i+1][1]);
           i += 2;
     }

     glVertex2f(widgets_pos[0][0],
               widgets_pos[0][1]);
     glVertex2f(widgets_pos[0][0]+property.xaxis,
               widgets_pos[0][1]);

     glVertex2f(widgets_pos[0][0],
               widgets_pos[0][1]);
     glVertex2f(widgets_pos[0][0],
               widgets_pos[0][1]+property.yaxis);

     glEnd();
}

void svMagnitude::RenderScaleFont(svScalar shift)
{
   for(int i=0;i<widgets_scale_pos.size();i++)
   { 
     char str[10]; 
     if(i==0)
          sprintf(str, "0");
     else if(i==1)
          sprintf(str, "%0.0f", widgets_max_value);
     else if(i==2)
          sprintf(str, "%0.0f", property.maxscale);
     glRasterPos2f(widgets_scale_pos[i][0]-shift,widgets_scale_pos[i][1]);
     for(int j=0;j<strlen(str);j++)
         glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
   }
}

void svMagnitude::RenderWidgetFont(svScalarArray & font, svScalar shift)
{
   char str[10];
   int format;
   if(fabs(font[0])>0 && fabs(font[0])<1) 
         format = 0;//%6.2e
   else 
         format = 1;//%0.0f

   if(format)
        sprintf(str, "%0.0f", font[0]);
   else
        sprintf(str, "%6.2e", font[0]);
   glRasterPos2f(widgets_pos[0][0],widgets_pos[0][1]-shift);
   for(int j=0;j<strlen(str);j++)
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

   int i=0;
   int count = 1;
   for(i=1;i<font.size()-1;i++)
   {
     if(i%2 == 0)
     {
       if(format)
          sprintf(str, "%0.0f", font[count]);
       else
          sprintf(str, "%6.2e", font[count]);
       glRasterPos2f(widgets_pos[i][0],widgets_pos[i][1]-shift);
       for(int j=0;j<strlen(str);j++)
           glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
       count++;
     }
   }

   int index = widgets_pos.size()-1;
   if(format)
        sprintf(str, "%0.0f", font[count]);
   else
        sprintf(str, "%6.2e", font[count]);
   glRasterPos2f(widgets_pos[index][0],widgets_pos[index][1]-shift);
   for(int j=0;j<strlen(str);j++)
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
}
void svMagnitude::Movement(int x, int y)
{
    y = property.height - y;
    double td, dd;
    dd=9e+9;
    int ni = -1;
    for(int i=0;i<widgets_pos.size();i++)
    {
       td = (x - widgets_pos[i][0]) * (x - widgets_pos[i][0])
        + (y - widgets_pos[i][1]) * (y - widgets_pos[i][1]);
       td = sqrt(td);
       if(td < dd)
       {
           dd = td; 
           ni = i;
       }
    }

    td = (x - widgets_scale_pos[1][0]) * (x - widgets_scale_pos[1][0])
        + (y - widgets_scale_pos[1][1]) * (y - widgets_scale_pos[1][1]);    
    td = sqrt(td);

    if(td > dd)
    {
      if(dd < property.pointsize)
      {
          widgets_status[ni] = WIDGETS_TOUCH;
      }
      else
      {
         for(int i=0;i<widgets_pos.size();i++)
         {
             widgets_status[i] = WIDGETS_DEFAULT;
         }
      }
    }
    else
    {
      dd = td;
      if(dd < property.pointsize)
      {
          widgets_scale_status = WIDGETS_TOUCH;
      }
      else
      {
          widgets_scale_status = WIDGETS_DEFAULT;
      }
    }
}
void svMagnitude::Click(int x, int y)
{
    y = property.height - y;
    double td, dd;
    dd=9e+9;
    int ni = -1;
    for(int i=0;i<widgets_pos.size();i++)
    {
       td = (x - widgets_pos[i][0]) * (x - widgets_pos[i][0])
        + (y - widgets_pos[i][1]) * (y - widgets_pos[i][1]);
       td = sqrt(td);
       if(td < dd)
       {
           dd = td;
           ni = i;
       }
    }

    td = (x - widgets_scale_pos[1][0]) * (x - widgets_scale_pos[1][0])
        + (y - widgets_scale_pos[1][1]) * (y - widgets_scale_pos[1][1]); 
    td = sqrt(td);

    if(td > dd)
    {
      if(dd < property.pointsize)
      {
          widgets_status[ni] = WIDGETS_SELECT;
      }
      else
      {
         for(int i=0;i<widgets_pos.size();i++)
         {
             widgets_status[i] = WIDGETS_DEFAULT;
         }
      }
    }
    else
    {
      dd = td;
      if(dd < property.pointsize)
      {
          widgets_scale_status = WIDGETS_SELECT;
      }
      else
      {
          widgets_scale_status = WIDGETS_DEFAULT;
      }
    }

}
void svMagnitude::Motion(int x, int y)
{
    y = property.height-y;

    if(widgets_scale_status == WIDGETS_SELECT)
    {
       int y1 = property.transy;  
       int y2 = property.transy + property.scaley;

       if(y<y1)  widgets_scale_pos[1][1] = y1;
       else if(y<y2) widgets_scale_pos[1][1] = y2;
       else widgets_scale_pos[1][1] = y; 
    }
    else
    {
        int ni=-1;
        for(int i=0;i<widgets_pos.size();i++)
        {
            if(widgets_status[i] == WIDGETS_SELECT) 
            {
                 ni = i;break;
            }
        }
        if(ni>=0)
        {
           if(ni == 0 || ni%2==1)
           {
              if(ni < widgets_pos.size()-1 && ni >0)
              {
                   int y1, y2;
       //            if(ni < i)
              } 
           } 
        }
    }
}
/*
void svMagnitude::SetMag(svScalar m1, svScalar m2, int w, int h) //min, max
{
	//cerr<<"mag "<<m1<<" "<<m2<<endl;
	yscale = h * 0.012;

	bool flag1 = true;
	if(level!=0)
	{
		cleanup();flag1 = false;
	}
	level = 0;


	svScalar *NumofWidget = new svScalar[100];
	svInt ex;
	svScalar tmp;
	svBool flag = false;

	int datasize = 100;

		ex = 0;
		tmp = m2;
           	for(int j=0;j<datasize;j++)
                {
                	if(tmp<10)break;
                	else                                
                        { ex ++;tmp = tmp/10;}
                }
		if(ex==0&&m2!=0)
		{
			tmp = m2;
			for(int j=0;j<datasize;j++)
			{
				if(tmp>=1)break;
				else
				{
					ex --;tmp = tmp * 10;
				}
			}
		}

			NumofWidget[0] = ex;
			level ++;

		tmp = m1;
		ex= 0;
           	for(int j=0;j<datasize;j++)
                {
                	if(tmp<10)break;
                	else                                
                        { ex ++;tmp = tmp/10;}
                }
		if(ex==0&&m1!=0)
		{
			tmp = m1;
			for(int j=0;j<datasize;j++)
			{
				if(tmp>=1)break;
				else
				{
					ex --;tmp = tmp * 10;
				}
			}
		}//cerr<<ex<<" "<<NumofWidget[0] <<endl;

		int gap = ex - NumofWidget[0] + 1;
		level = gap+1;
		for(int i=1;i<level;i++)
		{
			NumofWidget[i] = NumofWidget[0] + i;
		}


	int tmp1 = NumofWidget[0];
	int tmp2 = NumofWidget[level-1];	
	//cerr<<tmp1<<" "<<tmp2<<endl;

	for(int i=0;i<tmp2-tmp1+1;i++)
	{
		NumofWidget[i] = tmp1 + i;
	}
	level = tmp2 - tmp1 + 1;

	if(level <=5)
	{
		rlevel = 2;
	}		
	else
	{
		if(level<=8)
			rlevel = 3;
		else
			rlevel = level/3;
	}
	llevel = level - rlevel;

	//cerr<<level<<endl;
	WidgetH = new svVector3[level];
	MagWidget = new svScalar[level];

	for(int i=0;i<level;i++) {MagWidget[i] = NumofWidget[i]-SCALE;}


	middle = MagWidget[llevel];

	for(int i=level-1;i>=0;i--)
	{
		//WidgetH[i][0] = w*xratio + (float)i*xscale;
		//WidgetH[i][1] = h*yratio;
		WidgetH[i][0] = w*xratio - (float)(level-1-i)*xscale;
		WidgetH[i][1] = h*yratio;
		WidgetH[i][2] = 0;
	}

	CtrlPoints[0].x = WidgetH[0][0];
        CtrlPoints[0].y = h*yratio;
        CtrlPoints[0].z = 0;

        CtrlPoints[1].x = WidgetH[llevel][0];
        CtrlPoints[1].y = h*yratio + yscale;
        CtrlPoints[1].z = 0;

        CtrlPoints[2].x = WidgetH[llevel][0];
        CtrlPoints[2].y = h*yratio+ 2*yscale;
        CtrlPoints[2].z = 0;

        CtrlPoints[3].x = WidgetH[level-1][0];
        CtrlPoints[3].y = h * yratio + 6*yscale;
        CtrlPoints[3].z = 0;

        CtrlPoints[4].x = WidgetH[llevel][0];
        CtrlPoints[4].y = WidgetH[llevel][1];
        CtrlPoints[4].z = WidgetH[llevel][2];


	a1 =( CtrlPoints[1].y - CtrlPoints[0].y)/(CtrlPoints[1].x - CtrlPoints[0].x);
	b1 = CtrlPoints[1].y - a1 * CtrlPoints[1].x;
	a2 =( CtrlPoints[3].y - CtrlPoints[2].y)/(CtrlPoints[3].x - CtrlPoints[2].x);
	b2 = CtrlPoints[3].y - a2 * CtrlPoints[3].x;

	W = w;
	H = h;
	
	//cerr<<level<<endl;

	delete [] NumofWidget;
}

void svMagnitude::RenderMagnitude(int  w, int h)
{

	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(0.2);


	if(w!=W||h!=H)
	{
		for(int i=0;i<level;i++)
		{
			//WidgetH[i][0] = w*xratio + (float)i*xscale;
			WidgetH[i][0] = w*xratio - (float)(level-1-i)*xscale;
			WidgetH[i][1] = h*yratio;
			WidgetH[i][2] = 0;
		}

		for(int i=0;i<5;i++)
		{
			CtrlPoints[i].x = CtrlPoints[i].x + (w - W)*xratio;//*xratio
			CtrlPoints[i].y = CtrlPoints[i].y + (h - H)*yratio;
		}

		W = w;
		H = h;
	}

	char str[50];
	glColor3f(font_color[0],font_color[1],font_color[2]);
	

	int count = 0;

	for(int i=0;i<level;i++)
	{
		glRasterPos3f(WidgetH[i][0]-xscale/2.,WidgetH[i][1]-2*yscale,WidgetH[i][2]);
		
		//if(str[0] != '0')
		//	for(int j=0;j<50;j++)
		//	{
		//		if(str[j]!='.')
		//			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
		//		else
		//			break;
		//	}
		//else
		//{
		count = 0;

		if(i==0||i==level-1||i==level-rlevel)
		{
			sprintf(str, "%0.0f", MagWidget[i]);
		
			for(int j=0;j<strlen(str);j++)
			{
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
	
			}	
		}
			
	}

	char str2[50];

	sprintf(str2, "Linear(Magnitude)");

	
	glRasterPos3f(WidgetH[0][0]-xscale/2.,WidgetH[0][1]+4*yscale,WidgetH[0][2]);
	for(int j=0;j<strlen(str2);j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str2[j]);

	glPointSize(4);
	for(int i=0;i<5;i++)
	{
		if(SELECTED == GL_TRUE || NEAR == GL_TRUE)
		{
			if(i==ni)	
				glColor3f(0,1,0);
			else
				glColor3f(1,0,0);
		}
		else
			glColor3f(1,0,0);
		glBegin(GL_POINTS);
		glVertex3f(CtrlPoints[i].x, CtrlPoints[i].y, CtrlPoints[i].z);
		glEnd();
	}	
	
	glColor3f(font_color[0],font_color[1],font_color[2]);
	glPointSize(1);
	

	float tmpdis = (CtrlPoints[0].y - WidgetH[0][1])/10.;
	for(int i=1;i<=10;i++)
        {
		glBegin(GL_POINTS);			
		glVertex3f(WidgetH[0][0],WidgetH[0][1]+tmpdis*i,WidgetH[0][2]);
		glEnd();
	}
	tmpdis = (CtrlPoints[3].y - WidgetH[level-1][1])/10.;
	for(int i=1;i<=10;i++)
        {
		glBegin(GL_POINTS);			
		glVertex3f(WidgetH[level-1][0],WidgetH[level-1][1]+tmpdis*i,WidgetH[level-1][2]);
		glEnd();
	}	
	glPointSize(1);

	char str3[3] = {'D','O', 'I'};
	glRasterPos3f((CtrlPoints[1].x+CtrlPoints[3].x)/2.-xscale/2., yscale*5+h*yratio, CtrlPoints[1].z);
	for(int j=0;j<3;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str3[j]);
	

        for(int i=0;i<3;i++)
        {
	glColor3f(font_color[0],font_color[1],font_color[2]);
                glBegin(GL_LINES);
                glVertex3f(CtrlPoints[i].x, CtrlPoints[i].y, CtrlPoints[i].z);
		glVertex3f(CtrlPoints[i+1].x, CtrlPoints[i+1].y, CtrlPoints[i+1].z);
                glEnd();
        }

	glColor3f(font_color[0],font_color[1],font_color[2]);
	for(int i=0;i<level;i++)
	{
		if(i<level-1)
                {
			glBegin(GL_LINES);
                	glVertex3f(WidgetH[i][0],WidgetH[i][1],WidgetH[i][2]);
                	glVertex3f(WidgetH[i+1][0],WidgetH[i+1][1],WidgetH[i+1][2]);
               		glEnd();
		}
		
		if((i==0||i==level-1||i==level-rlevel))
		{
			glPointSize(4);
			glBegin(GL_POINTS);
			glVertex3f(WidgetH[i][0],WidgetH[i][1],WidgetH[i][2]);
			glEnd();
			glPointSize(1);
		}
	}

	glLineWidth(1.);

}

void svMagnitude::move_ctrlpoint(int x, int y, int w, int h) // need to be improved and the trackball thing.
{
    int whichAxis = -1;
    
	if(ni==4)whichAxis = 1;
	else whichAxis = 0;	


	    if(whichAxis == 1)
		{
	      CtrlPoints[ni].x = x;
		}
		else if(whichAxis == 0)
		{
	      CtrlPoints[ni].y = H-y;
		}

		SetLocation(w, h);

}

void svMagnitude::Find_Nearest(int x, int y)
{
    int i, j;
    GLdouble td, dd;

    dd=9e+9;

    ni = 0;
    
        for(i=4;i>=0;i--) {
            td = ((GLdouble)x-CtrlPoints[i].x) *  ((GLdouble)x-CtrlPoints[i].x) +
                 (H-(GLdouble)y-CtrlPoints[i].y) * (H-(GLdouble)y-CtrlPoints[i].y);
            if(td<dd) {
                dd=td;
                ni = i;
            }
        }
    if(dd<25.) SELECTED = GL_TRUE;
}

bool svMagnitude::Find_Near(int x, int y)
{
    int i, j;
    GLdouble td, dd;

    dd=9e+9;

    ni = 0;
    
        for(i=4;i>=0;i--) {
            td = ((GLdouble)x-CtrlPoints[i].x) *  ((GLdouble)x-CtrlPoints[i].x) +
                 (H-(GLdouble)y-CtrlPoints[i].y) * (H-(GLdouble)y-CtrlPoints[i].y);
            if(td<dd) {
                dd=td;
                ni = i;

            }
        }
    if(dd<25.) return true;
	else return false;
}

void svMagnitude::SetControlPoints(POINT *c, int n, int w, int h)
{
	for(int i=0;i<5;i++)
	{
		CtrlPoints[i] = c[i];
	}
	ni = n;
	SetLocation(w, h);
}

void svMagnitude::SetLocation(int w, int h)
{
	int i,j;


		if(ni==4)
		{
			if(CtrlPoints[ni].x>=CtrlPoints[3].x) CtrlPoints[ni].x=CtrlPoints[3].x;
			else  if(CtrlPoints[ni].x<=CtrlPoints[0].x) CtrlPoints[ni].x=CtrlPoints[0].x;
	 		//if(Linear == false)
				middle = MagWidget[0] + (CtrlPoints[ni].x - CtrlPoints[0].x)/(xscale);
			//else
			//	middle = MagWidget[0] + (slice/pow(10., 12.))*(CtrlPoints[ni].x - CtrlPoints[0].x)/(xscale);
			CtrlPoints[1].x	 = CtrlPoints[ni].x;
			CtrlPoints[2].x = CtrlPoints[ni].x;
		                a1 =( CtrlPoints[1].y - CtrlPoints[0].y)/(CtrlPoints[1].x - CtrlPoints[0].x);
		                b1  = CtrlPoints[1].y - a1 * CtrlPoints[1].x;
		                a2 =( CtrlPoints[3].y - CtrlPoints[2].y)/(CtrlPoints[3].x - CtrlPoints[2].x);
		                b2 = CtrlPoints[3].y - a2 * CtrlPoints[3].x;
		}
		else
		{
			if(ni==0)
			{
				if(CtrlPoints[ni].y>=CtrlPoints[ni+1].y)
					CtrlPoints[ni].y = CtrlPoints[ni+1].y;
				else
					if(CtrlPoints[ni].y<=h*yratio)
						CtrlPoints[ni].y = h*yratio;
				a1 =( CtrlPoints[1].y - CtrlPoints[0].y)/(CtrlPoints[1].x - CtrlPoints[0].x);
				b1  = CtrlPoints[1].y - a1 * CtrlPoints[1].x;
			}
			if(ni==1)
			{
		                if(CtrlPoints[ni].y<=CtrlPoints[ni-1].y)
		                        CtrlPoints[ni].y = CtrlPoints[ni-1].y+0.1;
		                else
		                        if(CtrlPoints[ni].y>=CtrlPoints[ni+1].y)
		                                CtrlPoints[ni].y = CtrlPoints[ni+1].y;
		                a1 =( CtrlPoints[1].y - CtrlPoints[0].y)/(CtrlPoints[1].x - CtrlPoints[0].x);
		                b1  = CtrlPoints[1].y - a1 * CtrlPoints[1].x;
			}
			if(ni==2)
			{
		                if(CtrlPoints[ni].y>=CtrlPoints[ni+1].y)
		                        CtrlPoints[ni].y = CtrlPoints[ni+1].y;
		                else
		                        if(CtrlPoints[ni].y<=CtrlPoints[ni-1].y)
		                                CtrlPoints[ni].y = CtrlPoints[ni-1].y;
	      		        a2 =( CtrlPoints[3].y - CtrlPoints[2].y)/(CtrlPoints[3].x - CtrlPoints[2].x);
				b2 = CtrlPoints[3].y - a2 * CtrlPoints[3].x;
			}
		        if(ni==3)
		        {
		                if(CtrlPoints[ni].y<=CtrlPoints[ni-1].y)
		                        CtrlPoints[ni].y = CtrlPoints[ni-1].y+0.1;
				else
					if(CtrlPoints[ni].y>=h)
						CtrlPoints[ni].y = h;
		                a2 =( CtrlPoints[3].y - CtrlPoints[2].y)/(CtrlPoints[3].x - CtrlPoints[2].x);
		                b2 = CtrlPoints[3].y - a2 * CtrlPoints[3].x;
		        }
		}
		if(a1==0.)b1 = CtrlPoints[1].y;
		if(a2==0.)b2 = CtrlPoints[3].y;	
}

svScalar *svMagnitude::GetMag(svScalar *den, int datasize, int w, int h)
{

                        a1 =( CtrlPoints[1].y - CtrlPoints[0].y)/(CtrlPoints[1].x - CtrlPoints[0].x);
                        b1  = CtrlPoints[1].y - a1 * CtrlPoints[1].x;
                        a2 =( CtrlPoints[3].y - CtrlPoints[2].y)/(CtrlPoints[3].x - CtrlPoints[2].x);
                        b2 = CtrlPoints[3].y - a2 * CtrlPoints[3].x;

	svFloat l;
	svScalar *size = new svScalar[datasize];
	svFloat lx;
	svScalar maxsize;

	for(int i=0;i<datasize;i++)
	{

			float pow_middle = pow(10.,middle);
			float pow_m0 = pow(10.,MagWidget[0]);
			float block_l1 = (pow_middle - pow_m0 )/(middle-MagWidget[0]);
			float block_l2 = (-pow_middle + pow(10.,MagWidget[level-1]))/(MagWidget[level-1]-middle);
			//cerr<<block_l<<endl;
			l = den[i]/pow(10.,SCALE);//cerr<<l<<endl;!!!!!!!!!!!!!!
			//if(l>pow(10.,-4))
			//cerr<<lx<<" "; 
			//cerr<<middle<<" "<<MagWidget[0]<<endl;
			if(l<pow_middle)
			{
				lx = CtrlPoints[0].x + (l-pow_m0 )/block_l1 * (xscale);
				size[i] = abs(lx * a1 + b1 - h*yratio);
			}
			else
			{	
				lx = CtrlPoints[2].x + (l-pow_middle)/block_l2 * (xscale);
				size[i] = abs(lx * a2 + b2 - h*yratio);
			}
			size[i] = size[i]*magscale;// /yscale;	
		
	}

	return size;
}

void svMagnitude::cleanup()
{
	delete [] MagWidget;	
	delete [] WidgetH;


	level = 0;	
}

*/
void svMagnitude::cleanup()
{
    if(widgets_pos.size()>0)
    {
        widgets_pos.free();
        widgets_scale_pos.free();
    }
}
}
