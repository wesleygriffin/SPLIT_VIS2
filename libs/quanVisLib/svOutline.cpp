#include "svOutline.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <string.h>
using namespace std;

namespace __svl_lib
{

void svOutline::DrawAxis_scale(svVector3 lbbox, svVector3 rbbox, int width)
{ 
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

  GLint viewport[4];
  GLdouble mvmatrix[16];
  GLdouble projmatrix[16];

    int i, j;

    svVector3 p[4];
  GLdouble wx[4];
  GLdouble wy[4];
  GLdouble wz[4];

    glGetIntegerv (GL_VIEWPORT, viewport);
    glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
    glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);

	for(i=0;i<4;i++)
	{
		p[i][0]=0;p[i][1]=0;p[i][2]=0;	
	}

	svVector3 widgetp[4];
	widgetp[0] = rbbox;
	widgetp[1] = rbbox;widgetp[1][0]=lbbox[0];
	widgetp[2] = rbbox;widgetp[2][1]=lbbox[1];
	widgetp[3] = rbbox;widgetp[3][2]=lbbox[2];

	for(i=0;i<4;i++)
	{
		/*p[i][0]  = widgetp[i][0] * tb_transform[0][0] +
			    widgetp[i][1] * tb_transform[1][0] +
			    widgetp[i][2] * tb_transform[2][0] +
			    tb_transform[3][0] + p[i][0];
                p[i][1]  = widgetp[i][0] * tb_transform[0][1] +
                            widgetp[i][1] * tb_transform[1][1] +
                            widgetp[i][2] * tb_transform[2][1] +
                            tb_transform[3][1] + p[i][1];
                p[i][2]  = widgetp[i][0] * tb_transform[0][2] +
                            widgetp[i][1] * tb_transform[1][2] +
                            widgetp[i][2] * tb_transform[2][2] +
                            tb_transform[3][2] + p[i][2];*/
		p[i] = widgetp[i];
	}
	
	for(i=0;i<4;i++)
	{
		gluProject((GLdouble)p[i][0],
                        (GLdouble)p[i][1],
                        (GLdouble)p[i][2],
                        mvmatrix, projmatrix, viewport,
                        &(wx[i]), &(wy[i]), &(wz[i]));
		wy[i] = (GLdouble)width-wy[i];
	}

        float dd[3];

	for(i=1;i<4;i++)
	{
		dd[i-1] = sqrt((wy[i]-wy[0])*(wy[i]-wy[0])+
				(wx[i]-wx[0])*(wx[i]-wx[0]));
	}

  double x,y,z;
  char value;
  int times[2];
  int scale;
  char str[4],temp[3];
  int len;
  float l;
  int l2[2];

  times[0] = rbbox[0] - lbbox[0];
  x=rbbox[0];y=rbbox[1];z=rbbox[2];
  glColor3f(font_color[0],font_color[1],font_color[2]);
  glPointSize(2.);
  glBegin(GL_POINTS);
  for(i=0;i<times[0];i++)
  {

	glVertex3f(x,y,z);
	x = x - 1;	
  }
  glEnd();

  times[0] = rbbox[1] - lbbox[1];
  x=rbbox[0];y=rbbox[1];z=rbbox[2];
  glColor3f(font_color[0],font_color[1],font_color[2]);
  glPointSize(2.);
  glBegin(GL_POINTS);
  for(i=0;i<times[0];i++)
  {
	glVertex3f(x,y,z);
	y = y - 1;
  }
  glEnd();
 
  times[0] = rbbox[2] - lbbox[2];
  x=rbbox[0];y=rbbox[1];z=rbbox[2];
  glColor3f(font_color[0],font_color[1],font_color[2]);
  glPointSize(2.);
  glBegin(GL_POINTS);
  for(i=0;i<times[0];i++)
  {

	glVertex3f(x,y,z);
	z = z - 1;
  }
  glEnd();

  glPointSize(5);
  l = dd[0]/50.;
  //l2 = (rbbox[0]-lbbox[0])/l;
  l2[0] = (rbbox[0] - lbbox[0])/l;
  l2[1] = (rbbox[0] - lbbox[0])/l;
  if(l2[0]==0) l2[0] = 1;
  if(l2[1]==0) l2[1] = 1;
  //scale = rbbox[0];
  //x=rbbox[0];y=rbbox[1];z=rbbox[2]+0.5;
  glColor3f(font_color[0],font_color[1],font_color[2]);
  //times = fabs(rbbox[0] - lbbox[0])/l2;
  times[0] = fabs(0 - lbbox[0])/l2[0];
  times[1] = fabs(rbbox[0] - 0)/l2[1];
  /*for(i=0;i<times;i++)
  {
	glColor3f(1.,1.,1.);
	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
	x=x-l2;
	scale=scale-l2;
	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_POINTS);
	glVertex3f(x,y,z-0.5);
	glEnd();
  }*/
  x=0;y=rbbox[1];z=rbbox[2]+0.5;
  scale = 0;
  for(i=0;i<=times[0];i++)
  {
	if(x>=lbbox[0])
	{
  glColor3f(font_color[0],font_color[1],font_color[2]);
	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_POINTS);
	glVertex3f(x,y,z-0.5);
	glEnd();
	x=x-l2[0];
	scale=scale-l2[0];
	}
  }
  x=0;y=rbbox[1];z=rbbox[2]+0.5;
  scale = 0;
  for(i=0;i<=times[1];i++)
  {
	if(x<=rbbox[0])
	{
  glColor3f(font_color[0],font_color[1],font_color[2]);
	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_POINTS);
	glVertex3f(x,y,z-0.5);
	glEnd();
	x=x+l2[1];
	scale=scale+l2[1];
	}
  }

  l = dd[1]/50.;
  //l2 = (rbbox[1]-lbbox[1])/l;
  l2[0] = (rbbox[1] - lbbox[1])/l;
  l2[1] = (rbbox[1] - lbbox[1])/l;
  //if(l2==0) l2 = 1;
  if(l2[0]==0)l2[0]=1;
  if(l2[1]==0)l2[1]=1;
  //scale = rbbox[1];
 // x=rbbox[0];y=rbbox[1];z=rbbox[2]+0.5;
  glColor3f(font_color[0],font_color[1],font_color[2]);
  //times = fabs(rbbox[1] - lbbox[1])/l2;
  times[0] = fabs(0 - lbbox[1])/l2[0];
  times[1] = fabs(rbbox[1] - 0)/l2[1];
  /*for(i=0;i<times;i++)
  {
	glColor3f(1.,1.,1.);
	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
	y=y-l2;
	scale=scale-l2;
	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_POINTS);
	glVertex3f(x,y,z-0.5);
	glEnd();
  }*/
  x=rbbox[0];y=0;z=rbbox[2]+0.5;
  scale = 0;
  for(i=0;i<=times[0];i++)
  {
	if(y>=lbbox[1])
	{
  glColor3f(font_color[0],font_color[1],font_color[2]);
	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_POINTS);
	glVertex3f(x,y,z-0.5);
	glEnd();
	y=y-l2[0];
	scale=scale-l2[0];
	}
  }
  x=rbbox[0];y=0;z=rbbox[2]+0.5;
  scale = 0;
  for(i=0;i<=times[1];i++)
  {
	if(y<=rbbox[1])
	{
  glColor3f(font_color[0],font_color[1],font_color[2]);
	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_POINTS);
	glVertex3f(x,y,z-0.5);
	glEnd();
	y=y+l2[1];
	scale=scale+l2[1];
	}
  }

  l = dd[2]/50.;
  //l2 = (rbbox[2]-lbbox[2])/l;
  l2[0] = (rbbox[2] - lbbox[2])/l;
  l2[1] = (rbbox[2] - lbbox[2])/l;
 // if(l2==0) l2 = 1;
  if(l2[0]==0)l2[0]=1;
  if(l2[1]==0)l2[1]=1;
  //scale = rbbox[2];
  //x=rbbox[0]+0.5;y=rbbox[1]+0.5;z=rbbox[2];
  glColor3f(font_color[0],font_color[1],font_color[2]);
 // times = fabs(rbbox[2] - lbbox[2])/l2;
  times[0] = fabs(0 - lbbox[2])/l2[0];
  times[1] = fabs(rbbox[2] - 0)/l2[1];
 /* for(i=0;i<times;i++)
  {
	glColor3f(1.,1.,1.);
	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
	z=z-l2;
	scale=scale-l2;
	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_POINTS);
	glVertex3f(x-0.5,y-0.5,z);
	glEnd();
  }*/
  x=rbbox[0]+0.5;y=rbbox[1]+0.5;z=0;
  scale = 0;
  for(i=0;i<=times[0];i++)
  {
	if(z>=lbbox[2])
	{

  glColor3f(font_color[0],font_color[1],font_color[2]);
	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_POINTS);
	glVertex3f(x-0.5,y-0.5,z);
	glEnd();

	z=z-l2[0];
	scale=scale-l2[0];
	}
  }
  x=rbbox[0]+0.5;y=rbbox[1]+0.5;z=0;
  scale = 0;
  for(i=0;i<=times[1];i++)
  {
	if(z<=rbbox[2])
	{
  glColor3f(font_color[0],font_color[1],font_color[2]);
	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
	for(j=0;j<len;j++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_POINTS);
	glVertex3f(x-0.5,y-0.5,z);
	glEnd();

	z=z+l2[1];
	scale=scale+l2[1];
	}
  }



 /* glColor3f(1., 1, 1);
  glRasterPos3f(lbbox[0]-1,rbbox[1],rbbox[2]+1);
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'x');
  glColor3f(1., 1, 1);
  glRasterPos3f(rbbox[0],lbbox[1]-1,rbbox[2]+1);
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'y');
  glColor3f(1., 1, 1);
  glRasterPos3f(rbbox[0],rbbox[1]+1,lbbox[2]-1);
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'z');*/

  glPointSize(1);

}

void svOutline::DrawAxis(svVector3 lbbox, svVector3 rbbox)
{

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glLineWidth(0.5);

	char str[50];

	//bounding box
	glColor3f(227./255.,26./255.,28./255.);//x
	glBegin(GL_LINES);
	glVertex3f(lbbox[0],lbbox[1],lbbox[2]);
	glVertex3f(rbbox[0],lbbox[1],lbbox[2]);
	glEnd();
	glColor3f(35./255.,139./255.,69./255.);//y
	glBegin(GL_LINES);
	glVertex3f(lbbox[0],lbbox[1],lbbox[2]);
	glVertex3f(lbbox[0],rbbox[1],lbbox[2]);
	glEnd();
	glColor3f(33./255.,113./255.,181./255.);//z
	glBegin(GL_LINES);
	glVertex3f(lbbox[0],lbbox[1],lbbox[2]);
	glVertex3f(lbbox[0],lbbox[1],rbbox[2]);
	glEnd();

       /* glColor4f(1,1,1,1);
        glRasterPos3f(AxisWidgetPos[0][0],AxisWidgetPos[0][1],AxisWidgetPos[0][2]);
        sprintf(str, "%f", AxisWidgetPos[0][0]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
        glRasterPos3f(AxisWidgetPos[3][0],AxisWidgetPos[3][1],AxisWidgetPos[3][2]);
        sprintf(str, "%f", AxisWidgetPos[3][0]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
        glRasterPos3f(AxisWidgetPos[7][0],AxisWidgetPos[7][1],AxisWidgetPos[7][2]);
        sprintf(str, "%f", AxisWidgetPos[7][0]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

        glRasterPos3f(AxisWidgetPos[1][0],AxisWidgetPos[1][1],AxisWidgetPos[1][2]);
        sprintf(str, "%f", AxisWidgetPos[1][1]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
        glRasterPos3f(AxisWidgetPos[4][0],AxisWidgetPos[4][1],AxisWidgetPos[4][2]);
        sprintf(str, "%f", AxisWidgetPos[4][1]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
        glRasterPos3f(AxisWidgetPos[8][0],AxisWidgetPos[8][1],AxisWidgetPos[8][2]);
        sprintf(str, "%f", AxisWidgetPos[8][1]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

        glRasterPos3f(AxisWidgetPos[2][0],AxisWidgetPos[2][1],AxisWidgetPos[2][2]);
        sprintf(str, "%f", AxisWidgetPos[2][2]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
        glRasterPos3f(AxisWidgetPos[5][0],AxisWidgetPos[5][1],AxisWidgetPos[5][2]);
        sprintf(str, "%f", AxisWidgetPos[5][2]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
        glRasterPos3f(AxisWidgetPos[9][0],AxisWidgetPos[9][1],AxisWidgetPos[9][2]);
        sprintf(str, "%f", AxisWidgetPos[9][2]);
        for(int j=0;j<strlen(str);j++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
*/

	
	glColor3f(227./255.,26./255.,28./255.);//x
	glBegin(GL_LINES);
	glVertex3f(lbbox[0],rbbox[1],lbbox[2]);
	glVertex3f(rbbox[0],rbbox[1],lbbox[2]);
	glEnd();

	glColor3f(35./255.,139./255.,69./255.); //y
        glBegin(GL_LINES);
        glVertex3f(rbbox[0],rbbox[1],lbbox[2]);
	glVertex3f(rbbox[0],lbbox[1],lbbox[2]);
	glEnd();

	glColor3f(35./255.,139./255.,69./255.); //y
        glBegin(GL_LINES);
        glVertex3f(lbbox[0],lbbox[1],rbbox[2]);
        glVertex3f(lbbox[0],rbbox[1],rbbox[2]);
	glEnd();

	glColor3f(227./255.,26./255.,28./255.);//x
        glBegin(GL_LINES);
        glVertex3f(lbbox[0],rbbox[1],rbbox[2]);
        glVertex3f(rbbox[0],rbbox[1],rbbox[2]);
	glEnd();

	glColor3f(35./255.,139./255.,69./255.); //y
        glBegin(GL_LINES);
        glVertex3f(rbbox[0],rbbox[1],rbbox[2]);
        glVertex3f(rbbox[0],lbbox[1],rbbox[2]);
        glEnd();

	glColor3f(227./255.,26./255.,28./255.);//x
        glBegin(GL_LINES);
        glVertex3f(lbbox[0],lbbox[1],rbbox[2]);
        glVertex3f(rbbox[0],lbbox[1],rbbox[2]);
        glEnd();

	glColor3f(33./255.,113./255.,181./255.);//z
        glBegin(GL_LINES);
        glVertex3f(lbbox[0],rbbox[1],lbbox[2]);
        glVertex3f(lbbox[0],rbbox[1],rbbox[2]);
        glEnd();

	glColor3f(33./255.,113./255.,181./255.);//z
        glBegin(GL_LINES);
        glVertex3f(rbbox[0],lbbox[1],lbbox[2]);
        glVertex3f(rbbox[0],lbbox[1],rbbox[2]);
        glEnd();

	glColor3f(33./255.,113./255.,181./255.);//z
        glBegin(GL_LINES);
        glVertex3f(rbbox[0],rbbox[1],lbbox[2]);
        glVertex3f(rbbox[0],rbbox[1],rbbox[2]);
        glEnd();
	//---

	//ROI bounding box
	/*svVector3 tmpl;
	svVector3 tmpr;
	tmpl[0]=AxisWidgetPos[0][0];tmpl[1]=AxisWidgetPos[1][1];tmpl[2]=AxisWidgetPos[2][2];
	tmpr[0]=AxisWidgetPos[3][0];tmpr[1]=AxisWidgetPos[4][1];tmpr[2]=AxisWidgetPos[5][2];

	glColor3f(1.,1.,1.);
	glBegin(GL_LINE_LOOP);
	glVertex3f(tmpl[0],tmpl[1],tmpl[2]);
	glVertex3f(tmpr[0],tmpl[1],tmpl[2]);
	glVertex3f(tmpr[0],tmpl[1],tmpr[2]);
	glVertex3f(tmpl[0],tmpl[1],tmpr[2]);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(tmpl[0],tmpr[1],tmpl[2]);
	glVertex3f(tmpr[0],tmpr[1],tmpl[2]);
	glVertex3f(tmpr[0],tmpr[1],tmpr[2]);
	glVertex3f(tmpl[0],tmpr[1],tmpr[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(tmpr[0],tmpl[1],tmpl[2]);
	glVertex3f(tmpr[0],tmpr[1],tmpl[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(tmpr[0],tmpl[1],tmpr[2]);
	glVertex3f(tmpr[0],tmpr[1],tmpr[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(tmpl[0],tmpl[1],tmpl[2]);
	glVertex3f(tmpl[0],tmpr[1],tmpl[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(tmpl[0],tmpl[1],tmpr[2]);
	glVertex3f(tmpl[0],tmpr[1],tmpr[2]);
	glEnd();*/

	
	//----------------------------


	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glLineWidth(1.);
}

void svOutline::DrawXYZ(svVector3 lbbox, svVector3 rbbox)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	//draw x,y,z
	glColor3f(227./255.,26./255.,28./255.);
	glBegin(GL_LINES);
	glVertex3f(rbbox[0]*1.5,0,0);
	glVertex3f(0,0,0);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(rbbox[0]*1.5,0,0);
	glVertex3f(rbbox[0]*1.5-1,0.5,0);
	glVertex3f(rbbox[0]*1.5-1,0,0.5);
	glVertex3f(rbbox[0]*1.5-1,-0.5,0);
	glVertex3f(rbbox[0]*1.5-1,0,-0.5);
	glVertex3f(rbbox[0]*1.5-1,0.5,0);
	glEnd();
  	//glColor3f(1., 1, 1);
  	glRasterPos3f(rbbox[0]*1.5+2.,0,0);
  	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'x');

        glColor3f(35./255.,139./255.,69./255.); 
        glBegin(GL_LINES);
        glVertex3f(0,rbbox[1]*1.5,0);
        glVertex3f(0,0,0);
        glEnd();
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0,rbbox[1]*1.5,0);
        glVertex3f(-0.5,rbbox[1]*1.5-1,0);
        glVertex3f(0,rbbox[1]*1.5-1,0.5);
        glVertex3f(0.5,rbbox[1]*1.5-1,0);
        glVertex3f(0,rbbox[1]*1.5-1,-0.5);
        glVertex3f(-0.5,rbbox[1]*1.5-1,0);
	glEnd();
  	//glColor3f(1., 1, 1);
  	glRasterPos3f(0.,rbbox[1]*1.5+2.,0);
  	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'y');

        glColor3f(33./255.,113./255.,181./255.);
        glBegin(GL_LINES);
        glVertex3f(0,0,rbbox[2]*1.5);
        glVertex3f(0,0,0);
        glEnd();
	glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0,0,rbbox[2]*1.5);
        glVertex3f(0.5,0,rbbox[2]*1.5-1);
        glVertex3f(0,0.5,rbbox[2]*1.5-1);
        glVertex3f(-0.5,0,rbbox[2]*1.5-1);
        glVertex3f(0,-0.5,rbbox[2]*1.5-1);
        glVertex3f(0.5,0,rbbox[2]*1.5-1);
	glEnd();
  	//glColor3f(1., 1, 1);
  	glRasterPos3f(0.,0.,rbbox[2]*1.5+2.);
  	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'z');
}




}
