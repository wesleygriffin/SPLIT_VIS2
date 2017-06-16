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
void svOutline::DrawLayers(svIntArray index, svVector3 pos, svVector3 dir, svScalar distance)
{
  
}
void svOutline::DrawAxis_scale(svVector3 lbbox, svVector3 rbbox, int width)
{ 

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
//  glPointSize(2.);
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
//  glPointSize(2.);
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
//  glPointSize(2.);
  glBegin(GL_POINTS);
  for(i=0;i<times[0];i++)
  {

	glVertex3f(x,y,z);
	z = z - 1;
  }
  glEnd();

 // glPointSize(5);
  l = dd[0]/55;
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
//	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
//	for(j=0;j<len;j++)
//		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);

                  glPushMatrix();
                  glTranslatef(x,y,z);
                  glScalef(0.002,0.002,0.002);
                  for(j=0;j<len;j++)
                  glutStrokeCharacter(GLUT_STROKE_ROMAN,str[j]);
                  glPopMatrix();

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
//	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
//	for(j=0;j<len;j++)
//		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
                  glPushMatrix();
                  glTranslatef(x,y,z);
                  glScalef(0.002,0.002,0.002);
                  for(j=0;j<len;j++)
                  glutStrokeCharacter(GLUT_STROKE_ROMAN,str[j]);
                  glPopMatrix();

	glColor3f(217./255.,240./255.,163./255.);
	glBegin(GL_POINTS);
	glVertex3f(x,y,z-0.5);
	glEnd();
	x=x+l2[1];
	scale=scale+l2[1];
	}
  }

  l = dd[1]/60.;
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
//	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
//	for(j=0;j<len;j++)
//		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
                  glPushMatrix();
                  glTranslatef(x,y,z);
                  glScalef(0.002,0.002,0.002);
                  for(j=0;j<len;j++)
                  glutStrokeCharacter(GLUT_STROKE_ROMAN,str[j]);
                  glPopMatrix();

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
//	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
//	for(j=0;j<len;j++)
//		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
                  glPushMatrix();
                  glTranslatef(x,y,z);
                  glScalef(0.002,0.002,0.002);
                  for(j=0;j<len;j++)
                  glutStrokeCharacter(GLUT_STROKE_ROMAN,str[j]);
                  glPopMatrix();

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
//	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
//	for(j=0;j<len;j++)
//		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
                  glPushMatrix();
                  glTranslatef(x,y,z);
                  glScalef(0.002,0.002,0.002);
                  for(j=0;j<len;j++)
                  glutStrokeCharacter(GLUT_STROKE_ROMAN,str[j]);
                  glPopMatrix();

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
//	glRasterPos3f(x,y,z);
	sprintf(str,"%.0f",(float)scale);
	len = strlen(str);
//	for(j=0;j<len;j++)
//		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
                  glPushMatrix();
                  glTranslatef(x,y,z);
                  glScalef(0.002,0.002,0.002);
                  for(j=0;j<len;j++)
                  glutStrokeCharacter(GLUT_STROKE_ROMAN,str[j]);
                  glPopMatrix();

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

	//glEnable(GL_DEPTH_TEST);
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
	glLineWidth(1.);
}
//eye is in (0,0,z);
svScalar svOutline::GetDistance(svVector3 newv1, svVector3 newv2, ViewProperty &property)
{
    //svVector3 newv1 = GetNewVector(v1, property.tm);
    //svVector3 newv2 = GetNewVector(v2, property.tm);

    GLdouble objx, objy, objz;
    GLdouble winx, winy, winz;
    objx=newv1[0];objy=newv1[1];objz=newv1[2];
    gluProject(objx, objy, objz, property.mvmatrix,
                       property.projmatrix,property.viewport,
                       &winx, &winy, &winz);
    newv1[0]=winx;newv1[1]=winy;newv1[2]=winz;
    objx=newv2[0];objy=newv2[1];objz=newv2[2];
    gluProject(objx, objy, objz, property.mvmatrix,
                       property.projmatrix,property.viewport,
                       &winx, &winy, &winz);
    newv2[0]=winx;newv2[1]=winy;newv2[2]=winz;

    svScalar distance = sqrt((newv2[0]-newv1[0])*(newv2[0]-newv1[0])
                       +(newv2[1]-newv1[1])*(newv2[1]-newv1[1]));
    return distance;
}

void svOutline::DrawAxisFont(svVector3 lbbox, svVector3 rbbox, ViewProperty &property)
{
    glColor3f(0,0,0);

    svVector3 v1, v2, newv1, newv2;
    v1[0]=rbbox[0];v1[1]=lbbox[1];v1[2]=lbbox[2];
    v2[0]=lbbox[0];v2[1]=lbbox[1];v2[2]=lbbox[2];
    svScalar xdistance = GetDistance(v1,v2,property);
    int xmin = lbbox[0]+1;
    int seg;
    newv1 = GetNewVector(v1, property.tm);
    newv2 = GetNewVector(v2, property.tm);
    svScalar distance = GetDistance(newv1, newv2, property);
    if(fabs(distance - xdistance)<1e-3) seg = 2;
    else
    {
      seg =  xdistance/distance+2;
    }
    for(int i=xmin;i<=rbbox[0];i+=seg)
    {
        v1[0]=(svScalar)i+0.2;v1[1]=lbbox[1]-1;v1[2]=lbbox[2]-0.5;
        newv1 = GetNewVector(v1, property.tm);

        glPushMatrix();
        glTranslatef(newv1[0],newv1[1],newv1[2]);
        glScalef(0.004,0.004,0.004);
        char str[20];
        sprintf(str, "%d", i);
        for(int j=0;j<strlen(str);j++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN,str[j]);
        glPopMatrix();
    }

    v1[0]=lbbox[0];v1[1]=lbbox[1];v1[2]=lbbox[2];
    v2[0]=lbbox[0];v2[1]=rbbox[1];v2[2]=lbbox[2];
    svScalar ydistance = GetDistance(v1,v2,property);
    int ymin = lbbox[1]+1;
    newv1 = GetNewVector(v1, property.tm);
    newv2 = GetNewVector(v2, property.tm);
    distance = GetDistance(newv1, newv2, property);
    if(fabs(distance - ydistance)<1e-3) seg = 2;
    else
    {
      seg =  ydistance/distance+2;
    }
    for(int i=ymin;i<=rbbox[1];i+=seg)
    {
        v1[0]=lbbox[0]-1;v1[1]=(svScalar)i+0.2;v1[2]=lbbox[2]-0.5;
        newv1 = GetNewVector(v1, property.tm);

        glPushMatrix();
        glTranslatef(newv1[0],newv1[1],newv1[2]);
        glScalef(0.004,0.004,0.004);
        char str[20];
        sprintf(str, "%d", i);
        for(int j=0;j<strlen(str);j++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN,str[j]);
        glPopMatrix();
    }

    v1[0]=lbbox[0];v1[1]=lbbox[1];v1[2]=lbbox[2];
    v2[0]=lbbox[0];v2[1]=lbbox[1];v2[2]=rbbox[2];
    svScalar zdistance = GetDistance(v1,v2,property);
    int zmin = lbbox[2]+1;
    newv1 = GetNewVector(v1, property.tm);
    newv2 = GetNewVector(v2, property.tm);
    distance = GetDistance(newv1, newv2, property);
    if(fabs(distance - zdistance)<1e-3) seg = 2;
    else
    {
      seg =  zdistance/distance+2;
    }
    for(int i=zmin;i<=rbbox[2];i+=seg)
    {
        v1[0]=lbbox[0]-0.5;v1[1]=lbbox[1]-0.5;v1[2]=(svScalar)i-0.2;
        newv1 = GetNewVector(v1, property.tm);

        glPushMatrix();
        glTranslatef(newv1[0],newv1[1],newv1[2]);
        glScalef(0.004,0.004,0.004);
        char str[20];
        sprintf(str, "%d", i);
        for(int j=0;j<strlen(str);j++)
       glutStrokeCharacter(GLUT_STROKE_ROMAN,str[j]);
        glPopMatrix();
    }


/*
    v1[0]=lbbox[0];v1[1]=lbbox[1];v1[2]=lbbox[2];
    v2[0]=lbbox[0];v2[1]=rbbox[1];v2[2]=lbbox[2];
    svScalar ydistance = GetDistance(v1,v2,property);
    int ymin = lbbox[1]+1;
    newv1 = GetNewVector(v1, property.tm);
    newv2 = GetNewVector(v2, property.tm);
    distance = GetDistance(newv1, newv2, property);
    if(fabs(distance - ydistance)<1e-3) seg = 1;
    else
    {
      seg =  ydistance/distance+1;
    }
    glPointSize(5);
    glColor3f(8./255., 29./255., 88./255.);
    glBegin(GL_POINTS);
    for(int i=ymin;i<=rbbox[1];i+=seg)
    {
        glVertex3f(lbbox[0],(svScalar)i, lbbox[2]);
    }
    glEnd();

    v1[0]=lbbox[0];v1[1]=lbbox[1];v1[2]=lbbox[2];
    v2[0]=lbbox[0];v2[1]=lbbox[1];v2[2]=rbbox[2];
    svScalar zdistance = GetDistance(v1,v2,property);
    int zmin = lbbox[2]+1;
    newv1 = GetNewVector(v1, property.tm);
    newv2 = GetNewVector(v2, property.tm);
    distance = GetDistance(newv1, newv2, property);
    if(fabs(distance - zdistance)<1e-3) seg = 1;
    else
    {
      seg =  zdistance/distance+1;
    }
    glPointSize(5);
    glColor3f(8./255., 29./255., 88./255.);
    glBegin(GL_POINTS);
    for(int i=zmin;i<=rbbox[2];i+=seg)
    {
        glVertex3f(lbbox[0],lbbox[1],(svScalar)i);
    }
    glEnd();
    glPointSize(1);
*/
}

void svOutline::DrawAxisSticks(svVector3 lbbox, svVector3 rbbox, ViewProperty &property)
{
    svVector3 v1, v2, newv1, newv2;
    v1[0]=rbbox[0];v1[1]=lbbox[1];v1[2]=lbbox[2];
    v2[0]=lbbox[0];v2[1]=lbbox[1];v2[2]=lbbox[2];
    svScalar xdistance = GetDistance(v1,v2,property);
    int xmin = lbbox[0]+1;
    int seg;
    newv1 = GetNewVector(v1, property.tm);
    newv2 = GetNewVector(v2, property.tm);
    svScalar distance = GetDistance(newv1, newv2, property);
    if(fabs(distance - xdistance)<1e-3) seg = 2;
    else 
    {
      seg =  xdistance/distance+2; 
    }
    glPointSize(5);
    glColor3f(8./255., 29./255., 88./255.);
    glBegin(GL_POINTS);
    for(int i=xmin;i<=rbbox[0];i+=seg)
    {   
        glVertex3f((svScalar)i, lbbox[1], lbbox[2]);
    }  
    glEnd();

    v1[0]=lbbox[0];v1[1]=lbbox[1];v1[2]=lbbox[2];
    v2[0]=lbbox[0];v2[1]=rbbox[1];v2[2]=lbbox[2];
    svScalar ydistance = GetDistance(v1,v2,property);
    int ymin = lbbox[1]+1;
    newv1 = GetNewVector(v1, property.tm);
    newv2 = GetNewVector(v2, property.tm);
    distance = GetDistance(newv1, newv2, property);
    if(fabs(distance - ydistance)<1e-3) seg = 2;
    else
    {
      seg =  ydistance/distance+2;
    }
    glPointSize(5);
    glColor3f(8./255., 29./255., 88./255.);
    glBegin(GL_POINTS);
    for(int i=ymin;i<=rbbox[1];i+=seg)
    {   
        glVertex3f(lbbox[0],(svScalar)i, lbbox[2]);
    } 
    glEnd();

    v1[0]=lbbox[0];v1[1]=lbbox[1];v1[2]=lbbox[2];
    v2[0]=lbbox[0];v2[1]=lbbox[1];v2[2]=rbbox[2];
    svScalar zdistance = GetDistance(v1,v2,property);
    int zmin = lbbox[2]+1;
    newv1 = GetNewVector(v1, property.tm);
    newv2 = GetNewVector(v2, property.tm);
    distance = GetDistance(newv1, newv2, property);
    if(fabs(distance - zdistance)<1e-3) seg = 2;
    else
    {
      seg =  zdistance/distance+2;
    }
    glPointSize(5);
    glColor3f(8./255., 29./255., 88./255.);
    glBegin(GL_POINTS);
    for(int i=zmin;i<=rbbox[2];i+=seg)
    {
        glVertex3f(lbbox[0],lbbox[1],(svScalar)i);
    } 
    glEnd();
    glPointSize(1);
}
void svOutline::DrawXYZFont(svVector3 lbbox, svVector3 rbbox, ViewProperty &property)
{
	glColor3f(227./255.,26./255.,28./255.);

        svVector3 v1; v1[0]=rbbox[0]*1.2+2.;v1[1]=0;v1[2]=0;
        svVector3 newv1 = GetNewVector(v1, property.tm);

        glPushMatrix();
        glTranslatef(newv1[0],newv1[1],newv1[2]);
        glScalef(0.01,0.01,0.01);
        glutStrokeCharacter(GLUT_STROKE_ROMAN,'x');
        glPopMatrix();

        glColor3f(35./255.,139./255.,69./255.); 

        v1[0]=0;v1[1]=rbbox[1]*1.2+2.;v1[2]=0;
        newv1 = GetNewVector(v1, property.tm);

        glPushMatrix();
        glTranslatef(newv1[0],newv1[1],newv1[2]);
        glScalef(0.01,0.01,0.01);
        glutStrokeCharacter(GLUT_STROKE_ROMAN,'y');
        glPopMatrix();

        v1[0]=0;v1[2]=rbbox[2]*1.2+2.;v1[1]=0;
        newv1 = GetNewVector(v1, property.tm);

        glColor3f(33./255.,113./255.,181./255.);
        glPushMatrix();
        glTranslatef(newv1[0],newv1[1],newv1[2]);
        glScalef(0.01,0.01,0.01);
        glutStrokeCharacter(GLUT_STROKE_ROMAN,'z');
        glPopMatrix();
}
void svOutline::DrawXYZPrimeFont(svVector3 pos, svVector3 dir, svVector3 x, svVector3 y, svVector3 rbbox, ViewProperty &property)
{
        glColor3f(227./255.,26./255.,28./255.);

        svVector3 v1; 
        v1[0]=pos[0]+x[0]*(rbbox[0]*1.8+2.);
        v1[1]=pos[1]+x[1]*(rbbox[0]*1.8+2.);
        v1[2]=pos[2]+x[2]*(rbbox[0]*1.8+2.);
        svVector3 newv1 = GetNewVector(v1, property.tm);

        char str[5];
        sprintf(str,"x'");
        glPushMatrix();
        glTranslatef(newv1[0],newv1[1],newv1[2]);
        glScalef(0.02,0.02,0.02);
        for(int i=0;i<strlen(str);i++)
            glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
        glPopMatrix();

        glColor3f(35./255.,139./255.,69./255.);

        v1[0]=pos[0]+y[0]*(rbbox[0]*1.8+2.);
        v1[1]=pos[1]+y[1]*(rbbox[0]*1.8+2.);
        v1[2]=pos[2]+y[2]*(rbbox[0]*1.8+2.);
        newv1 = GetNewVector(v1, property.tm);

        sprintf(str,"y'");
        glPushMatrix();
        glTranslatef(newv1[0],newv1[1],newv1[2]);
        glScalef(0.02,0.02,0.02);
        for(int i=0;i<strlen(str);i++)
            glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
        glPopMatrix();

        v1[0]=pos[0]+dir[0]*(rbbox[0]*1.8+2.);
        v1[1]=pos[1]+dir[1]*(rbbox[0]*1.8+2.);
        v1[2]=pos[2]+dir[2]*(rbbox[0]*1.8+2.);
        newv1 = GetNewVector(v1, property.tm);

        sprintf(str,"z'");
        glColor3f(33./255.,113./255.,181./255.);
        glPushMatrix();
        glTranslatef(newv1[0],newv1[1],newv1[2]);
        glScalef(0.02,0.02,0.02);
        for(int i=0;i<strlen(str);i++)
            glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
        glPopMatrix();
}
void svOutline::DrawXYZPrime(svVector3 pos, svVector3 dir, svVector3 x, svVector3 y, svVector3 rbbox)
{
       glEnable(GL_LIGHTING);
       svVector3 p;
       p[0] = pos[0]+x[0]*rbbox[0]*1.8;
       p[1] = pos[1]+x[1]*rbbox[0]*1.8;
       p[2] = pos[2]+x[2]*rbbox[0]*1.8;
       svVector3 d;
       d = x;
       svScalar radius = 0.5;
       svScalar height = radius*3.;
       glColor3f(227./255.,26./255.,28./255.);
       RenderCone(p, d, radius, height, 4);

       p[0] = pos[0]+y[0]*rbbox[0]*1.8;
       p[1] = pos[1]+y[1]*rbbox[0]*1.8;
       p[2] = pos[2]+y[2]*rbbox[0]*1.8;
       d = y;
       glColor3f(35./255.,139./255.,69./255.);
       RenderCone(p, d, radius, height, 4);

       glColor3f(33./255.,113./255.,181./255.);
       p[0] = pos[0]+dir[0]*rbbox[0]*1.8;
       p[1] = pos[1]+dir[1]*rbbox[0]*1.8;
       p[2] = pos[2]+dir[2]*rbbox[0]*1.8;
       d = dir;
       RenderCone(p, d, radius, height, 4);

        glLineWidth(2);
//===========================================================
        glDisable(GL_LIGHTING);
	glColor3f(227./255.,26./255.,28./255.);
	glBegin(GL_LINES);
	glVertex3f(pos[0]+x[0]*rbbox[0]*1.8,
                   pos[1]+x[1]*rbbox[0]*1.8,
                   pos[2]+x[2]*rbbox[0]*1.8);
	glVertex3f(pos[0],pos[1],pos[2]);
	glEnd();

        glColor3f(35./255.,139./255.,69./255.); 
        glBegin(GL_LINES);
        glVertex3f(pos[0]+y[0]*rbbox[0]*1.8,
                   pos[1]+y[1]*rbbox[0]*1.8,
                   pos[2]+y[2]*rbbox[0]*1.8);
        glVertex3f(pos[0],pos[1],pos[2]);
        glEnd();

        glColor3f(33./255.,113./255.,181./255.);
        glBegin(GL_LINES);
        glVertex3f(pos[0]+dir[0]*rbbox[0]*1.8,
                   pos[1]+dir[1]*rbbox[0]*1.8,
                   pos[2]+dir[2]*rbbox[0]*1.8);
        glVertex3f(pos[0],pos[1],pos[2]);
        glEnd();
       glLineWidth(1);

}
void svOutline::DrawXYZ(svVector3 lbbox, svVector3 rbbox, ViewProperty &property)
{
       glEnable(GL_LIGHTING);
       svVector3 pos;
       pos[0]=rbbox[0]*1.2;pos[1]=0;pos[2]=0;
       svVector3 dir;
       dir[0]=1;dir[1]=0;dir[2]=0;
       svScalar radius = 0.25;
       svScalar height = radius*3.;
       glColor3f(227./255.,26./255.,28./255.);
       RenderCone(pos, dir, radius, height, 4);

       pos[0]=0;pos[1]=rbbox[1]*1.2;pos[2]=0;
       dir[0]=0;dir[1]=1;dir[2]=0;
       glColor3f(35./255.,139./255.,69./255.);
       RenderCone(pos, dir, radius, height, 4);

       pos[0]=0;pos[1]=0;pos[2]=rbbox[2]*1.2;
       dir[0]=0;dir[1]=0;dir[2]=1;
       glColor3f(33./255.,113./255.,181./255.);
       RenderCone(pos, dir, radius, height, 4);


       glDisable(GL_LIGHTING);
	//draw x,y,z
	glColor3f(227./255.,26./255.,28./255.);
	glBegin(GL_LINES);
	glVertex3f(rbbox[0]*1.2,0,0);
	glVertex3f(0,0,0);
	glEnd();
/*	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(rbbox[0]*1.5,0,0);
	glVertex3f(rbbox[0]*1.5-1,0.5,0);
	glVertex3f(rbbox[0]*1.5-1,0,0.5);
	glVertex3f(rbbox[0]*1.5-1,-0.5,0);
	glVertex3f(rbbox[0]*1.5-1,0,-0.5);
	glVertex3f(rbbox[0]*1.5-1,0.5,0);
	glEnd();
*/
        /*svVector3 v1; v1[0]=rbbox[0]*1.5+2.;v1[1]=0;v1[2]=0;
        svVector3 v2; v2[0]=rbbox[0]*1.5+2.;v2[1]=0;v2[2]=1;
        svVector3 newv1 = GetNewVector(v1, property.tm);
        svVector3 newv2 = GetNewVector(v2, property.tm);
        svVector3 v = normalize(newv2 - newv1);
        double angle_z;
        double  angle_x;
        GetRotateAngle(v, angle_x, angle_z); 
        glPushMatrix();
        glTranslatef(rbbox[0]*1.5+2.,0,0);
        glRotatef(-angle_x/PI * 180.,0,0,1);
        glRotatef(-angle_z/PI * 180.,1,0,0);
        cerr<<angle_x/PI * 180.<<" "<<angle_z/PI * 180.<<endl;
        glScalef(0.025,0.025,0.025);
        glutStrokeCharacter(GLUT_STROKE_ROMAN,'x');
        glPopMatrix();
*/
  	//glColor3f(1., 1, 1);
//  	glRasterPos3f(rbbox[0]*1.5+2.,0,0);
//  	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'x');
        glColor3f(35./255.,139./255.,69./255.); 
        glBegin(GL_LINES);
        glVertex3f(0,rbbox[1]*1.2,0);
        glVertex3f(0,0,0);
        glEnd();
/*        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0,rbbox[1]*1.5,0);
        glVertex3f(-0.5,rbbox[1]*1.5-1,0);
        glVertex3f(0,rbbox[1]*1.5-1,0.5);
        glVertex3f(0.5,rbbox[1]*1.5-1,0);
        glVertex3f(0,rbbox[1]*1.5-1,-0.5);
        glVertex3f(-0.5,rbbox[1]*1.5-1,0);
	glEnd();
*/
  	//glColor3f(1., 1, 1);
  //	glRasterPos3f(0.,rbbox[1]*1.5+2.,0);
 // 	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'y');
/*
                  glPushMatrix();
                  glTranslatef(0.,rbbox[1]*1.5+2.,0);
        glRotatef(angle_x/3.1415926 * 180.,1,0,0);
        glRotatef(angle_z/3.1415926 * 180.,0,0,1);
        glRotatef(-90,1,0,0);
                  glScalef(0.005,0.005,0.005);
                  glutStrokeCharacter(GLUT_STROKE_ROMAN,'y');
                  glPopMatrix();

*/
        glColor3f(33./255.,113./255.,181./255.);
        glBegin(GL_LINES);
        glVertex3f(0,0,rbbox[2]*1.2);
        glVertex3f(0,0,0);
        glEnd();
/*	glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0,0,rbbox[2]*1.5);
        glVertex3f(0.5,0,rbbox[2]*1.5-1);
        glVertex3f(0,0.5,rbbox[2]*1.5-1);
        glVertex3f(-0.5,0,rbbox[2]*1.5-1);
        glVertex3f(0,-0.5,rbbox[2]*1.5-1);
        glVertex3f(0.5,0,rbbox[2]*1.5-1);
	glEnd();
*/
  	//glColor3f(1., 1, 1);
  //	glRasterPos3f(0.,0.,rbbox[2]*1.5+2.);
  //	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'z');
        /*          glPushMatrix();
                  glTranslatef(0.,0.,rbbox[2]*1.5+2.);
        glRotatef(angle_x/3.1415926 * 180.,1,0,0);
        glRotatef(angle_z/3.1415926 * 180.,0,0,1);
        glRotatef(-90,1,0,0);
                  glScalef(0.005,0.005,0.005);
                  glutStrokeCharacter(GLUT_STROKE_ROMAN,'z');
                  glPopMatrix();
*/
}




}
