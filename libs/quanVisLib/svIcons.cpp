#include "svIcons.h"

namespace __svl_lib{

//parallel
void DrawArrow1(float arrowbase,
                float arrowheight, 
                float height,
                svVector3 pos)
{//cerr<<pos[0]<<" "<<pos[1]<<" "<<height<<endl;
    glBegin(GL_LINES);
    glVertex2f(pos[0], pos[1]);
    glVertex2f(pos[0], pos[1]+height);
    glEnd();

    svVector3 arrowpos[3];
    arrowpos[0][0] = pos[0]-arrowbase/2.;
    arrowpos[0][1] = pos[1]+height;
    arrowpos[1][0] = pos[0]+arrowbase/2.;
    arrowpos[1][1] = pos[1]+height;
    arrowpos[2][0] = pos[0];
    arrowpos[2][1] = pos[1]+height+arrowheight;
    glBegin(GL_TRIANGLES);
    glVertex2f(arrowpos[0][0],arrowpos[0][1]);
    glVertex2f(arrowpos[1][0],arrowpos[1][1]);
    glVertex2f(arrowpos[2][0],arrowpos[2][1]);
    glEnd();
}

//anti-symmetry right 
void DrawArrow2(float arrowbase,
                float arrowheight,
                float height,
                svVector3 pos)
{
    glBegin(GL_LINES);
    glVertex2f(pos[0], pos[1]);
    glVertex2f(pos[0], pos[1]-height);
    glEnd();

    svVector3 arrowpos[3];
    arrowpos[0][0] = pos[0]-arrowbase/2.;
    arrowpos[0][1] = pos[1]-height;
    arrowpos[1][0] = pos[0]+arrowbase/2.;
    arrowpos[1][1] = pos[1]-height;
    arrowpos[2][0] = pos[0];
    arrowpos[2][1] = pos[1]-height-arrowheight;
    glBegin(GL_TRIANGLES);
    glVertex2f(arrowpos[0][0],arrowpos[0][1]);
    glVertex2f(arrowpos[2][0],arrowpos[2][1]);
    glVertex2f(arrowpos[1][0],arrowpos[1][1]);
    glEnd();
}

void DrawArrowButton(float w, float h,
                     svVector3 pos,
                     int type)
{
//    glBegin(GL_QUADS);
//    glVertex2f(pos[0],pos[1]);
//    glVertex2f(pos[0]+w,pos[1]);
//    glVertex2f(pos[0]+w,pos[1]+h);
//    glVertex2f(pos[0],pos[1]+h);
//    glEnd();
    float onew = w;
    glColor4f(0,0,0,0.5);
    float ratio =3./4.;
    float rh = 1./4.;
    float arrowbase = w/5.;
    float arrowheight = 3.73;
    float arroww = (onew/2.) * ratio;
    float arrowh = h * rh; 
    float height = (h - arrowh*2.) -  arrowbase/2. * arrowheight;
    switch(type){
       case 0:
               {
                  svVector3 p;
                  p[0] = pos[0] + w - (w/2.)*ratio;
                  p[1] = pos[1] + arrowh;
                  arrowheight = arrowbase/2. * arrowheight;
                  DrawArrow1(arrowbase, arrowheight, height, p);
                  p[0] = pos[0] + (w/2.)*ratio;
                  DrawArrow1(arrowbase, arrowheight, height, p);
               }
               break;
       case 1:
               {
               //   rh = 1./6.;
               //   arrowh = h/2.;
               //   height = (h/2. - rh*h) - arrowbase/2. * arrowheight;
               //   svVector3 p;
               //   p[0] = pos[0] + w - (w/2.)*ratio;
               //   p[1] = pos[1] + arrowh;
               //   arrowheight = arrowbase/2. * arrowheight;
                  svVector3 p;//todo !!!!!!!!!!!!!!!!
                  p[0] = pos[0] + w - (w/2.)*ratio;
                  p[1] = pos[1] + arrowh;
                  arrowheight = arrowbase/2. * arrowheight;
                  DrawArrow1(arrowbase, arrowheight, height, p);
                  p[0] = pos[0] + (w/2.)*ratio;
                  p[1] = pos[1] + h - arrowh;
                  DrawArrow2(arrowbase, arrowheight, height, p);
               }
               break;
    };

//    glBegin(GL_LINES);
//    glVertex2f(pos[0]+arroww, pos[1]+arrowh);
//    glVertex2f(pos[0]+arroww, pos[1]+arrowh+height);
//    glEnd();        

//    arroww = onew - (onew/2.) * ratio;
//    glBegin(GL_LINES);
//    glVertex2f(pos[0]+arroww, pos[1]+arrowh);
//    glVertex2f(pos[0]+arroww, pos[1]+arrowh+height);
//    glEnd(); 

//    arroww = (onew/2.) * ratio;
//    svVector3 arrowpos[3];
//    arrowpos[0][0] = pos[0]+arroww-arrowbase/2.;
//    arrowpos[0][1] = pos[1]+arrowh+height;
//    arrowpos[1][0] = pos[0]+arroww+arrowbase/2.;
//    arrowpos[1][1] = pos[1]+arrowh+height;
//    arrowpos[2][0] = pos[0]+arroww;
//    arrowpos[2][1] = pos[1]+arrowh+height+arrowheight*arrowbase/2.;
//    glBegin(GL_TRIANGLES);
//    glVertex2f(arrowpos[0][0],arrowpos[0][1]);
//    glVertex2f(arrowpos[1][0],arrowpos[1][1]);
//    glVertex2f(arrowpos[2][0],arrowpos[2][1]);
//    glEnd();

//    arroww = onew - (onew/2.) * ratio;
//    arrowpos[0][0] = pos[0]+arroww-arrowbase/2.;
//    arrowpos[0][1] = pos[1]+arrowh+height;
//    arrowpos[1][0] = pos[0]+arroww+arrowbase/2.;
//    arrowpos[1][1] = pos[1]+arrowh+height;
//    arrowpos[2][0] = pos[0]+arroww;
//    arrowpos[2][1] = pos[1]+arrowh+height+arrowheight*arrowbase/2.;
//    glBegin(GL_TRIANGLES);
//    glVertex2f(arrowpos[0][0],arrowpos[0][1]);
//    glVertex2f(arrowpos[1][0],arrowpos[1][1]);
//    glVertex2f(arrowpos[2][0],arrowpos[2][1]);
//    glEnd();
}

void DrawStackBars(svScalar level, float w, float h, svVector3 *pos, 
                  svVector3 end,
                  int tophisto, svScalarArray *histovalues)
{
  svVector3 colors[4];
  colors[0][0]=215.;colors[0][1]=25.; colors[0][2]=28.;
  colors[1][0]=253.;colors[1][1]=174.;colors[1][2]=97.;
  colors[2][0]=171.;colors[2][1]=217.;colors[2][2]=233.;
  colors[3][0]=44.; colors[3][1]=123.;colors[3][2]=182.;

  float boxh = h;
  float boxw = w/10.;
  float shiftw = w/2. - boxw/2.;
  float seg = boxh/tophisto;
  for(int i=0;i<histovalues[0].size();i++)
  {
    svScalar height;
    svScalar preheight = 0;
    svScalar y = pos[i][1]- h*1.5;
    for(int j=3;j>=0;j--)
    {
      glColor3f(colors[j][0]/255., colors[j][1]/255., colors[j][2]/255.);
      height = preheight+seg * histovalues[j][i];
      glBegin(GL_QUADS);
      glVertex2f(pos[i][0]+shiftw, y+preheight);
      glVertex2f(pos[i][0]+shiftw+boxw, y+preheight);
      glVertex2f(pos[i][0]+shiftw+boxw, y+height);
      glVertex2f(pos[i][0]+shiftw, y+height);
      glEnd();
      preheight =  height;
    }
  }
  svScalar lx = end[0];
  svScalar ly = end[1]-h*1.6;
  for(int i=0;i<4;i++)
  {
     glColor3f(colors[3-i][0]/255., colors[3-i][1]/255., colors[3-i][2]/255.);
     glBegin(GL_QUADS);
     glVertex2f(lx, ly);
     glVertex2f(lx+boxw, ly);
     glVertex2f(lx+boxw, ly + boxw*2);
     glVertex2f(lx, ly + boxw*2);
     glEnd();
     ly =ly + boxw*2;
  }

  glColor3f(0,0,0);
  lx = end[0];
  ly = end[1]-h*1.6;
  for(int i=0;i<4;i++)
  {
    char str[20];
    int v = level+i;//hard code!!!!!!
    sprintf(str, "1e%d",v);
    glRasterPos2f(lx+boxw,ly);
     for(int j=0;j<strlen(str);j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
     ly =ly + boxw*2;
 }
  glColor4f(0,0,0,0.5);

 glBegin(GL_LINES);
 glVertex2f(pos[0][0],pos[0][1]-h*1.5);
 glVertex2f(pos[histovalues[0].size()-1][0]+w, pos[0][1]-h*1.5);
 glEnd();

 glColor3f(0,0,0);
 char str[20];
 sprintf(str,"Magnitude");
 glRasterPos2f(pos[0][0]-90,pos[0][1]+h*3./4.-h*1.5);
     for(int j=0;j<strlen(str);j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
 sprintf(str,"distribution (%)");
 glRasterPos2f(pos[0][0]-90,pos[0][1]+h/4.-h*1.5);
     for(int j=0;j<strlen(str);j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
}

void DrawHorizonBars(float w, float h, svVector3 *pos,
                   int tophisto, int * histovalues, int num)
{
  float seg = h/(float)tophisto;
  float barw = w/10.;
  float shifth = h+h/3.;
  float shiftw = w/2. - barw/2.;

  glColor4f(0,0,0,0.5);
  for(int i=0;i<num;i++)
  {
     float barh = seg * (float)histovalues[i];
     glBegin(GL_QUADS);
     glVertex2f(pos[i][0]+shiftw, pos[i][1]+shifth);
     glVertex2f(pos[i][0]+shiftw+barw, pos[i][1]+shifth);
     glVertex2f(pos[i][0]+shiftw+barw, pos[i][1]+shifth+barh);
     glVertex2f(pos[i][0]+shiftw, pos[i][1]+shifth+barh);
     glEnd();
 
     if(barh<2.&&histovalues[i]>0)
     {
       char str[20];
        sprintf(str, "%d",histovalues[i]);
       glRasterPos2f(pos[i][0]+barw,pos[i][1]+shifth+barh+barw);
     for(int j=0;j<strlen(str);j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
     }
  }

 glColor3f(0,0,0);
 char str[20];
 sprintf(str,"# of spins");
 glRasterPos2f(pos[0][0]-90.,pos[0][1]+h*3./4.+h);
     for(int j=0;j<strlen(str);j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

 glBegin(GL_LINES);
 glVertex2f(pos[0][0],pos[0][1]+shifth);
 glVertex2f(pos[num-1][0]+w, pos[0][1]+shifth);
 glEnd();

}

void DrawDownButtons(float w, float h,
                     svVector3 pos)
{
    float boundary = w/15.;
    svVector3 p[8];
    p[0][0]=pos[0]+boundary;  p[0][1]=pos[1];
    p[1][0]=pos[0]+w-boundary;p[1][1]=pos[1];
    p[2][0]=pos[0]+w;         p[2][1]=pos[1]+boundary;
    p[3][0]=pos[0]+w;         p[3][1]=pos[1]+h-boundary;
    p[4][0]=pos[0]+w-boundary;p[4][1]=pos[1]+h;
    p[5][0]=pos[0]+boundary;  p[5][1]=pos[1]+h;
    p[6][0]=pos[0];           p[6][1]=pos[1]+h-boundary;
    p[7][0]=pos[0];           p[7][1]=pos[1]+boundary;

    float barh = h/40.;

    glBegin(GL_POLYGON);
    for(int i=0;i<8;i++)
    {
       glVertex2f(p[i][0], p[i][1]);
    }
    glEnd();

    svVector3 barp[4];
    barp[0][0]=p[4][0]-barh; barp[0][1]=p[4][1]+barh;
    barp[1][0]=p[5][0];      barp[1][1]=p[5][1]+barh;
    barp[2][0]=p[6][0]-barh; barp[2][1]=p[6][1];
    barp[3][0]=p[7][0]-barh; barp[3][1]=p[7][1]+barh;

    glColor4f(0,0,0,0.5);
    glBegin(GL_QUAD_STRIP);
    for(int i=0;i<4;i++)
    {
         glVertex2f(p[i+4][0], p[i+4][1]);
         glVertex2f(barp[i][0], barp[i][1]);
    }
    glEnd();
//    glBegin(GL_QUADS);
//    glVertex2f(pos[0]-barh,pos[1]+h);
//    glVertex2f(pos[0]+w,pos[1]+h);
//    glVertex2f(pos[0]+w,pos[1]+h+barh);
//    glVertex2f(pos[0]-barh,pos[1]+h+barh);
//    glEnd();

//    glBegin(GL_QUADS);
//    glVertex2f(pos[0]-barh,pos[1]);
//    glVertex2f(pos[0],pos[1]);
//    glVertex2f(pos[0],pos[1]+h);
//    glVertex2f(pos[0]-barh,pos[1]+h);
//    glEnd();

    float highth = h/8.;

    glColor4f(204./255., 76./255., 2./255., 0.5);

    svVector3 newpos;
    newpos[0] = pos[0] - highth-barh;
    newpos[1] = pos[1] - highth;
    float neww, newh;
    neww = w + highth * 2.+barh;
    newh = h + highth * 2.+barh;
    boundary = neww/15.;
    svVector3 newp[8];
    newp[0][0]=newpos[0]+boundary;  newp[0][1]=newpos[1];
    newp[1][0]=newpos[0]+neww-boundary;newp[1][1]=newpos[1];
    newp[2][0]=newpos[0]+neww;         newp[2][1]=newpos[1]+boundary;
    newp[3][0]=newpos[0]+neww;         newp[3][1]=newpos[1]+newh-boundary;
    newp[4][0]=newpos[0]+neww-boundary;newp[4][1]=newpos[1]+newh;
    newp[5][0]=newpos[0]+boundary;  newp[5][1]=newpos[1]+newh;
    newp[6][0]=newpos[0];           newp[6][1]=newpos[1]+newh-boundary;
    newp[7][0]=newpos[0];           newp[7][1]=newpos[1]+boundary;


    glBegin(GL_QUAD_STRIP);
    for(int i=0;i<4;i++)
    {
       glVertex2f(p[i][0], p[i][1]);
       glVertex2f(newp[i][0], newp[i][1]);
    }
    for(int i=4;i<8;i++)
    {
       glVertex2f(barp[i-4][0], barp[i-4][1]);
       glVertex2f(newp[i][0], newp[i][1]);
    }
    glVertex2f(p[0][0], p[0][1]);
    glVertex2f(newp[0][0], newp[0][1]);
    glEnd();

//    glBegin(GL_QUADS);
//    glVertex2f(pos[0]-barh-highth,pos[1]);
//    glVertex2f(pos[0]-barh,pos[1]);
//    glVertex2f(pos[0]-barh,pos[1]+h+barh);
//    glVertex2f(pos[0]-barh-highth,pos[1]+h+barh);
//    glEnd();
//    glBegin(GL_QUADS);
//    glVertex2f(pos[0]-barh-highth,pos[1]+h+barh);
//    glVertex2f(pos[0]+w,pos[1]+h+barh);
//    glVertex2f(pos[0]+w,pos[1]+h+barh+highth);
//    glVertex2f(pos[0]-barh-highth,pos[1]+h+barh+highth);
//    glEnd();
//    glBegin(GL_QUADS);
//    glVertex2f(pos[0]+w,pos[1]);
//    glVertex2f(pos[0]+w+highth,pos[1]);
//    glVertex2f(pos[0]+w+highth,pos[1]+h+barh+highth);
//    glVertex2f(pos[0]+w,pos[1]+h+barh+highth);
//    glEnd();
//    glBegin(GL_QUADS);
//    glVertex2f(pos[0]-barh-highth,pos[1]-highth);
//    glVertex2f(pos[0]+w+highth,pos[1]-highth);
//    glVertex2f(pos[0]+w+highth,pos[1]);
//    glVertex2f(pos[0]-barh-highth,pos[1]);
//    glEnd();
}
void DrawUpButtons(float w, float h,
                     svVector3 pos)
{
//    float barh = h/20.;
//    glColor4f(0,0,0,0.5);
//    glBegin(GL_QUADS);
//    glVertex2f(pos[0],pos[1]-barh);
//    glVertex2f(pos[0]+w,pos[1]-barh);
//    glVertex2f(pos[0]+w,pos[1]);
//    glVertex2f(pos[0],pos[1]);
//    glEnd(); 
//    glBegin(GL_QUADS);
//    glVertex2f(pos[0]+w,pos[1]-barh);
//    glVertex2f(pos[0]+w+barh,pos[1]-barh);
//    glVertex2f(pos[0]+w+barh,pos[1]+h);
//   glVertex2f(pos[0]+w,pos[1]+h);
//    glEnd();
    float boundary = w/15.;
    svVector3 p[8];
    p[0][0]=pos[0]+boundary;  p[0][1]=pos[1];
    p[1][0]=pos[0]+w-boundary;p[1][1]=pos[1];
    p[2][0]=pos[0]+w;         p[2][1]=pos[1]+boundary;
    p[3][0]=pos[0]+w;         p[3][1]=pos[1]+h-boundary;
    p[4][0]=pos[0]+w-boundary;p[4][1]=pos[1]+h;
    p[5][0]=pos[0]+boundary;  p[5][1]=pos[1]+h;
    p[6][0]=pos[0];           p[6][1]=pos[1]+h-boundary;
    p[7][0]=pos[0];           p[7][1]=pos[1]+boundary;

    float barh = h/20.;

    glBegin(GL_POLYGON);
    for(int i=0;i<8;i++)
    {
       glVertex2f(p[i][0], p[i][1]);
    }
    glEnd();

    svVector3 barp[4];
    barp[0][0]=p[0][0]+barh; barp[0][1]=p[0][1]-barh;
    barp[1][0]=p[1][0];      barp[1][1]=p[1][1]-barh;
    barp[2][0]=p[2][0]+barh; barp[2][1]=p[2][1];
    barp[3][0]=p[3][0]+barh; barp[3][1]=p[3][1]-barh;

    glColor4f(0,0,0,0.5);
    glBegin(GL_QUAD_STRIP);
    for(int i=0;i<4;i++)
    {
         glVertex2f(barp[i][0], barp[i][1]);
         glVertex2f(p[i][0], p[i][1]);
    }
    glEnd();
}

void DrawArrowDownButton(int type,
                     float w, float h,
                     svVector3 pos,
                     svVector4 color)
{
    float barh = h/20.;
    float highth = h/5.;

    glColor4f(color[0],color[1],color[2],color[3]);
    DrawDownButtons(w, h, pos);

    int value = type;
    vector<int> t;
    for(int i=0;i<3;i++)
    {
        t.push_back(value%2);
        value = value/2;
    }


    float tmpw, tmph;
    svVector3 tmppos;
    tmpw = w/3.;
    tmph = h;
    tmppos[0] = pos[0];
    tmppos[1] = pos[1];
    glColor4f(color[0],color[1],color[2],color[3]);
    DrawArrowButton(tmpw, tmph, tmppos,t[2]);
    tmppos[0] = pos[0]+w/3.;
    glColor4f(color[0],color[1],color[2],color[3]);
    DrawArrowButton(tmpw, tmph, tmppos,t[1]);
    tmppos[0] = pos[0]+w/3.*2.;
    glColor4f(color[0],color[1],color[2],color[3]);
    DrawArrowButton(tmpw, tmph, tmppos,t[0]);
}

void DrawArrowUpButton(int type,
                     float w, float h,
                     svVector3 pos,
                     svVector4 color)
{
    float barh = h/20.;
    glColor4f(color[0],color[1],color[2],color[3]);
    DrawUpButtons(w, h, pos);

    int value = type;
    vector<int> t;
    for(int i=0;i<3;i++)
    {
        t.push_back(value%2);
        value = value/2;
    }

    float tmpw, tmph;
    svVector3 tmppos;
    tmpw = w/3.;
    tmph = h;
    tmppos[0] = pos[0];
    tmppos[1] = pos[1];
    glColor4f(color[0],color[1],color[2],color[3]);
    DrawArrowButton(tmpw, tmph, tmppos,t[2]);
    tmppos[0] = pos[0]+w/3.;
    glColor4f(color[0],color[1],color[2],color[3]);
    DrawArrowButton(tmpw, tmph, tmppos,t[1]);
    tmppos[0] = pos[0]+w/3.*2.;
    glColor4f(color[0],color[1],color[2],color[3]);
    DrawArrowButton(tmpw, tmph, tmppos,t[0]);

}

void DrawEncodeDownButton(char **text, 
                     float w, float h,
                     svVector3 pos)
{
        glColor4f(0.5,0.5,0.5,0.5);

    DrawDownButtons(w, h, pos);
    
  
//    glBegin(GL_QUADS);
//    glVertex2f(pos[0],pos[1]);
//    glVertex2f(pos[0]+w,pos[1]);
//    glVertex2f(pos[0]+w,pos[1]+h);
//    glVertex2f(pos[0],pos[1]+h);
//    glEnd();

    glColor3f(0.,0.,0.);
 
    float font_w = 5.5;
    int font_size = (w/2.)/font_w; 
    int l = strlen(text[0]);
    float shift_x = (w/2. - (float)l * font_w)/2.;
    float shift_y = h/20.;
    float x, y;
    x = pos[0] + shift_x;
    y = pos[1] + h/2. + shift_y;

    glRasterPos2f(x,y);
    for(int j=0;j<strlen(text[0]);j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[0][j]);

    l = strlen(text[1]);
    shift_x = (w/2. - (float)l * font_w)/2.;
    x = pos[0] + shift_x;
    y = pos[1] + h/2.-font_w-shift_y;
    glRasterPos2f(x,y);
    for(int j=0;j<strlen(text[1]);j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[1][j]); 

    l = strlen(text[2]);
    shift_x = (w/2. - (float)l * font_w)/2.;   
    x = pos[0] + w/2. + shift_x;
    y = pos[1] + h/2. + shift_y;
    glRasterPos2f(x,y);
    for(int j=0;j<strlen(text[2]);j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[2][j]); 

    l = strlen(text[3]);
    shift_x = (w/2. - (float)l * font_w)/2.;
    x = pos[0] + w/2. + shift_x;
    y = pos[1] + h/2. - font_w - shift_y;
    glRasterPos2f(x,y);
    for(int j=0;j<strlen(text[3]);j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[3][j]);
}

void DrawEncodeUpButton(char **text,
                     float w, float h,
                     svVector3 pos)
{
    glColor4f(0.5,0.5,0.5,0.5);

    DrawUpButtons(w, h, pos);


//    glBegin(GL_QUADS);
//    glVertex2f(pos[0],pos[1]);
//    glVertex2f(pos[0]+w,pos[1]);
//    glVertex2f(pos[0]+w,pos[1]+h);
//    glVertex2f(pos[0],pos[1]+h);
//    glEnd();

    glColor3f(0.,0.,0.);

    float font_w = 5.5;
    int font_size = (w/2.)/font_w;
    int l = strlen(text[0]);
    float shift_x = (w/2. - (float)l * font_w)/2.;
    float shift_y = h/20.;
    float x, y;
    x = pos[0] + shift_x;
    y = pos[1] + h/2. + shift_y;

    glRasterPos2f(x,y);
    for(int j=0;j<strlen(text[0]);j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[0][j]);

    l = strlen(text[1]);
    shift_x = (w/2. - (float)l * font_w)/2.;
    x = pos[0] + shift_x;
    y = pos[1] + h/2.-font_w-shift_y;
    glRasterPos2f(x,y);
    for(int j=0;j<strlen(text[1]);j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[1][j]); 

    l = strlen(text[2]);
    shift_x = (w/2. - (float)l * font_w)/2.; 
    x = pos[0] + w/2. + shift_x;
    y = pos[1] + h/2. + shift_y;
    glRasterPos2f(x,y);
    for(int j=0;j<strlen(text[2]);j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[2][j]); 

    l = strlen(text[3]);
    shift_x = (w/2. - (float)l * font_w)/2.;
    x = pos[0] + w/2. + shift_x;
    y = pos[1] + h/2. - font_w - shift_y;
    glRasterPos2f(x,y);
    for(int j=0;j<strlen(text[3]);j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[3][j]);

}

void DrawTextUpButton(char *text,
                     float w, float h,
                     svVector3 pos)
{
    glColor4f(0.5,0.5,0.5,0.5);

    DrawUpButtons(w, h, pos);
    glColor3f(0.,0.,0.);

    float font_w = 5.5;
    int font_size = (w/2.)/font_w;
    int l = strlen(text);
    float shift_x = (w - (float)l * font_w)/2.;
    float shift_y = h/3.;
    float x, y;
    x = pos[0] + shift_x;
    y = pos[1] +  shift_y;

    glRasterPos2f(x,y);
    for(int j=0;j<strlen(text);j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[j]);

}
void DrawTextDownButton(char *text,
                     float w, float h,
                     svVector3 pos)
{
    glColor4f(0.5,0.5,0.5,0.5);

    DrawDownButtons(w, h, pos);
    glColor3f(0.,0.,0.);

    float font_w = 5.5;
    int font_size = (w/2.)/font_w;
    int l = strlen(text);
    float shift_x = (w - (float)l * font_w)/2.;
    float shift_y = h/3.;
    float x, y;
    x = pos[0] + shift_x;
    y = pos[1] +  shift_y;

    glRasterPos2f(x,y);
    for(int j=0;j<strlen(text);j++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[j]);

}

}
