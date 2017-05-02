
#include "svSummaryGlyph.h"


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include "svException.h"
#include "svUtil.h"

using namespace std;

namespace __svl_lib {

/*svSummaryGlyph::svSummaryGlyph():svGlyph()
{
      denDistribute = NULL;
      visibleDistribute = NULL;
      numPower = 0;
}
*/
svSummaryGlyph::svSummaryGlyph(svVectorField *f):svArrowGlyph(f)
{
      denDistribute = NULL;
      visibleDistribute = NULL;
      numPower = 0;
}
void svSummaryGlyph::RenderAnnulus(svVector3 pos, svVector3 vel,
                                svScalar left, svScalar x, svScalar H, svScalar r)
{
        double degree =1;
        double d1 = x/degree;
        double d2 = (360-x-left)/degree;
        double d3 = left/degree;
        int times1;// = x/degree;  
        int times2;// = (360-x-left)/degree;
        int times3;// = left/degree;

        times1 = d1; times2 = d2; times3 = d3;
        double degree1 = x/times1;
        double degree2 = (360-x-left)/times2;
        double degree3 = left/times3;

        if(d1 < 1&&x>0)
        {
           times1 = 1;
           degree1 = d1;
        }
        if(d3 < 1&&left>0)
        {
           times3 = 1;
           degree3 = d3;
        }
        if(d2 < 1&&(360-x-left)>0)
        {
           times2 = 1;
           degree2 = d2;
        }

        svVector3 norm = svGetPerpendicularVector(vel);
        svVector3 head_out, head_in;
        head_out = pos + H * norm;
        head_in  = pos + r * norm;

        glDisable(GL_LIGHTING);
        glBegin(GL_QUAD_STRIP);
   //     glNormal3f(vel[0], vel[1], vel[2]);
   //     glNormal3f(-vel[0], -vel[1], -vel[2]);
        for(int ii = 0; ii <= times1; ii++)
        {
                float i = (float)ii * degree1;//(x/(float)times1);

                svVector3 draw_out = svGetRotatePoint(head_out, pos, vel, i);
                svVector3 draw_in = svGetRotatePoint(head_in, pos, vel, i);

                glVertex3f(draw_out[0], draw_out[1], draw_out[2]);
                glVertex3f(draw_in[0], draw_in[1], draw_in[2]);
        }
        glEnd();

        glColor4f(0.25,0.25,0.25,0.25);
        glBegin(GL_QUAD_STRIP);
        for(int ii = 0; ii <= times3; ii++)
        {
                float i = x + (float)ii * degree3;//(left/(float)times3);

                svVector3 draw_out = svGetRotatePoint(head_out, pos, vel, i);
                svVector3 draw_in = svGetRotatePoint(head_in, pos, vel, i);

                glVertex3f(draw_out[0], draw_out[1], draw_out[2]);
                glVertex3f(draw_in[0], draw_in[1], draw_in[2]);
        }
        glEnd();


        glEnable(GL_LIGHTING);
        glColor4f(0.8,0.8,0.8,0.5);
        glBegin(GL_QUAD_STRIP);
        glNormal3f(vel[0], vel[1], vel[2]);
    //    glNormal3f(-vel[0], -vel[1], -vel[2]);
        for(int ii = 0; ii <= times2; ii++)
        {
                float i = x + left+ (float)ii * degree2;//((360-x-left)/(float)times2);

                svVector3 draw_out = svGetRotatePoint(head_out, pos, vel, i);
                svVector3 draw_in = svGetRotatePoint(head_in, pos, vel, i);

                glVertex3f(draw_out[0], draw_out[1], draw_out[2]);
                glVertex3f(draw_in[0], draw_in[1], draw_in[2]);
        }
        glEnd();
}

void svSummaryGlyph::Generate(float alpha)
{
  cleanup(); 

//cerr<<"cleanup"<<endl; 
  denDistribute = new svScalarArray[maxClusterLabel+2];//+1: size; +1: clusterLabel = -1
  visibleDistribute = new svScalarArray[maxClusterLabel+2];
//cerr<<maxClusterLabel<<endl;
//  svScalarArray count;
//cerr<<glyphScale<< " "<<numPower<<endl;
  for(int i=0;i<maxClusterLabel+2;i++)
  {
      svVector3 p;
      p[0]=0;p[1]=0;p[2]=0;
      summaryPos.add(p);
      summaryDir.add(p);
      summaryDen.add(0);
      summaryCount.add(0);
      summaryMaxDen.add(-9e+9);
      summaryLabel.add(i-1);
     for(int j=0;j<numPower;j++)
      {
        denDistribute[i].add(0);
        visibleDistribute[i].add(0);
      }
  }
//cerr<<"done1"<<endl;
  for(int i=0;i<seed_num;i++)
  {//cerr<<i<<endl;
     for(int j=0;j<glyph[i].size();j++)
     {
     //  if(visibleLabel[i][j])
     //  {

         int in = clusterLabel[i][j]+1;
         
         summaryPos[in][0] = summaryPos[in][0] + glyph[i][j][0];  
         summaryPos[in][1] = summaryPos[in][1] + glyph[i][j][1];
         summaryPos[in][2] = summaryPos[in][2] + glyph[i][j][2];  
         summaryDir[in][0] = summaryDir[in][0] + dir[i][j][0];
         summaryDir[in][1] = summaryDir[in][1] + dir[i][j][1];
         summaryDir[in][2] = summaryDir[in][2] + dir[i][j][2];
         summaryDen[in] = summaryDen[in] + mag[i][j];


         int num = getNumOfIntegerDigits(mag[i][j]);
         if(numPower-num-scaling-1>=0) // when num == 0??????????
         {
                 denDistribute[in][numPower-num-scaling-1]++;
//         if(numPower-num-scaling-1<0)cerr<<numPower<<" "<<num<<" "<<scaling<<endl;
               if(num > summaryMaxDen[in]) 
                 summaryMaxDen[in] = num;
         }
         summaryCount[in] ++;
    //    }
         if(visibleLabel[i][j])
         {
           if(numPower-num-scaling-1>=0)
             visibleDistribute[in][numPower-num-scaling-1]++;
         }
     } 
  } 
   //cerr<<summaryPos.size()<<endl;

  for(int i=0;i<summaryPos.size();i++)
  {
      if(summaryCount[i]>0)
      {
         summaryPos[i][0] = summaryPos[i][0]/summaryCount[i];
         summaryPos[i][1] = summaryPos[i][1]/summaryCount[i];
         summaryPos[i][2] = summaryPos[i][2]/summaryCount[i];
         summaryDir[i][0] = summaryDir[i][0]/summaryCount[i];
         summaryDir[i][1] = summaryDir[i][1]/summaryCount[i];
         summaryDir[i][2] = summaryDir[i][2]/summaryCount[i];
         summaryDen[i] = summaryDen[i]/summaryCount[i];
        // cerr<<i<<" "<<summaryDen[i]<<" "<<glyphScale<<" "<<count[i]<<" ";
         for(int j=0;j<numPower;j++)
         {
             denDistribute[i][j] = denDistribute[i][j]/summaryCount[i];
             visibleDistribute[i][j]=visibleDistribute[i][j]/summaryCount[i];
	    // cerr<<denDistribute[i][j]<<" ";
         }//cerr<<endl;
//         cerr<<summaryMaxDen[i]<<" "<<scaling<<endl;
         summaryMaxDen[i] = summaryMaxDen[i] + scaling;
         summaryDir[i].normalize();
      }
    //  cerr<<summaryLabel[i]<<" "<<count[i]<<endl;;
  }//cerr<<endl;

 SetColors();
//==================display list===================================

  display_mode = SV_DISPLAYLIST;
  svGlyph::STILL_UPDATE = false;


//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//        glEnable(GL_COLOR_MATERIAL);
//        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
/*
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
*/
        if(glIsList(display_list))
                glDeleteLists(display_list, 1);
    glNewList(display_list, GL_COMPILE);

     //   glEnable(GL_COLOR_MATERIAL);
      //  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);

 //   glEnable(GL_DEPTH_TEST);

    for(int i=0;i<summaryPos.size();i++)
    {
         if(summaryCount[i]>0)
         {
  //          cerr<<summaryMaxDen[i]<<" "<<numPower<<endl;
                double radius = glyphRadius/3.;//*summaryDen[i];

               double H = radius;
               double r= 0;
                double rotate_x = acos(summaryDir[i][2]);
                if(summaryDir[i][1] > 0)
                {
                        rotate_x = -rotate_x;
                }
                double rotate_z = acos(summaryDir[i][1]/sqrt(summaryDir[i][0]*summaryDir[i][0] + summaryDir[i][1]*summaryDir[i][1]));
                if(rotate_x < 0)
                {
                        if(summaryDir[i][0] > 0)
                        {
                                rotate_z = -rotate_z;
                        }
                }
                else
                {
                        rotate_z = PI - rotate_z;
                        if (summaryDir[i][0] <0)
                        {
                                rotate_z = - rotate_z;
                        }
                } 

               glColor4f(summaryColor[i][0],
                         summaryColor[i][1],
                         summaryColor[i][2],
                         1);

               svVector3 end;
               svScalar s = summaryDen[i] * glyphScale;
               end[0] = summaryPos[i][0]+s*summaryDir[i][0];
               end[1] = summaryPos[i][1]+s* summaryDir[i][1];
               end[2] = summaryPos[i][2]+s* summaryDir[i][2];

                glEnable(GL_LIGHTING);
                glEnable(GL_LIGHT0);
                glEnable(GL_LIGHT1);

/*               glPushMatrix();
               glTranslatef(end[0], end[1], end[2]);
                glRotatef(rotate_z/M_PI*180,0,0,1);
                glRotatef(rotate_x/M_PI*180,1,0,0);
                glutSolidCone(radius, radius*3, 5, 1);
                glPopMatrix();
*/
                RenderCone(end, summaryDir[i], glyphRadius, glyphRadius*3, 8);
                RenderCylinder(summaryPos[i], summaryDir[i], glyphRadius/3, s, 10);
//                glDisable(GL_LIGHTING);
//                glDisable(GL_LIGHT0);

/*                glLineWidth(3.);
                glBegin(GL_LINES);
                glVertex3f(end[0], end[1], end[2]);
                glVertex3f(summaryPos[i][0],summaryPos[i][1],summaryPos[i][2]);
                glEnd();

                glLineWidth(6);
                glColor4f(0,0,0,alpha);
                glBegin(GL_LINES);
                glVertex3f(end[0], end[1], end[2]);
                glVertex3f(summaryPos[i][0],summaryPos[i][1],summaryPos[i][2]);
                glEnd();

                glEnable(GL_LIGHTING);
                glEnable(GL_LIGHT0);
*/
                glEnable(GL_POLYGON_OFFSET_FILL);
                glPolygonOffset(1.0, 2);

                svVector3 mid;
                mid[0] = summaryPos[i][0];
                mid[1] = summaryPos[i][1];
                mid[2] = summaryPos[i][2];

                //mid[0]  = mid[0]/2;
               // mid[1] = mid[1]/2;
               // mid[2] = mid[2]/2;

               glPushMatrix();
               glTranslatef(mid[0], mid[1], mid[2]);
                glRotatef(rotate_z/M_PI*180,0,0,1);
                glRotatef(rotate_x/M_PI*180,1,0,0);
                glNormal3f(summaryDir[i][0], summaryDir[i][1], summaryDir[i][2]);
                glNormal3f(-summaryDir[i][0], -summaryDir[i][1], -summaryDir[i][2]);
                for(int j=0;j<=summaryMaxDen[i];j++)//numPower;j++)
                {
                        glColor4f(summaryDenColor[numPower-j-1][0],
                                 summaryDenColor[numPower-j-1][1],
                                 summaryDenColor[numPower-j-1][2],
                                 alpha);
                       svVector3 p;
                      p[0]=0;p[1]=0;p[2]=0;
                       svVector3 v;
                       v[0]=0;v[1]=0;v[2]=1;
                  RenderAnnulus(p,v,
                  (denDistribute[i][numPower-j-1]-visibleDistribute[i][numPower-j-1])*360,
                       visibleDistribute[i][numPower-j-1]*360,//denDistribute[i][j]*360,
                                        H, r);
                        H = H + radius;
                        r = r + radius;
                }

                glDisable(GL_LIGHTING);
                glDisable(GL_LIGHT0);
                glDisable(GL_LIGHT1);
                glDisable(GL_TEXTURE_2D);

                glLineWidth(0.5);

                glColor4f(0.5,0.5,0.5,1);
                svVector3 norm;// = svGetPerpendicularVector(summaryDir[i]);
                svVector3 head_out;
               // head_out = mid + (svScalar)(radius * numPower) * norm;
                head_out[0]=radius*(summaryMaxDen[i]+1);head_out[1]=0;head_out[2]=0;
                norm[0] = 0;norm[1]=0;norm[2]=1;
                svVector3 p;
                p[0]=0;p[1]=0;p[2]=0;
                for(int iii = 0; iii < 360; )
                {
                        glBegin(GL_LINES);
                        svVector3 draw_out = svGetRotatePoint(head_out, p, norm, (float)iii);
                        glVertex3f(draw_out[0], draw_out[1], draw_out[2]);
                        glVertex3f(p[0], p[1], p[2]);
                        glEnd();

                        iii = iii + 45;
                }

                for(int jjj=0;jjj<summaryMaxDen[i];jjj++)
                {
                  head_out[0] = (jjj+1) * radius;
                  glBegin(GL_LINE_STRIP);
                  for(int iii = 0; iii <= 360; )
                  {
                        svVector3 draw_out = svGetRotatePoint(head_out, p, norm, (float)iii);
                        glVertex3f(draw_out[0], draw_out[1], draw_out[2]);

                        iii = iii + 5;
                  }
                  glEnd();
                }
               

                glColor3f(0,0,0);


                for(int jjj=0;jjj<=summaryMaxDen[i];)
                {
                  svVector3 vv =  svGetPerpendicularVector(norm);
                  head_out = (svScalar)(jjj * radius) * vv;head_out[2] = 0.25;
                  char str[10];
                  sprintf(str, "%d", jjj-scaling);
                  glPushMatrix();
                  glTranslatef(head_out[0],head_out[1],head_out[2]);
                  glScalef(0.002,0.002,0.002);
          //        glRasterPos3f(head_out[0],head_out[1],head_out[2]);
                  for(int j=0;j<strlen(str);j++)
                      glutStrokeCharacter(GLUT_STROKE_ROMAN,str[j]);
                  glPopMatrix();
          //            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
                  jjj+=2;
                }


                glPointSize(2.);
                glBegin(GL_POINTS);
                for(int jjj=0;jjj<=summaryMaxDen[i];)
                {
                  svVector3 vv =  svGetPerpendicularVector(norm);
                  head_out = (svScalar)(jjj * radius) * vv;
                  glVertex3f(head_out[0],head_out[1],head_out[2]);
                  jjj+=2;
                }
                glEnd();
                glPointSize(1.);
 
                glPopMatrix();
                glLineWidth(1.);
                glDisable(GL_POLYGON_OFFSET_FILL);
                glEnable(GL_LIGHTING);
                glEnable(GL_LIGHT0);
                glEnable(GL_LIGHT1);
//                glEnable(GL_TEXTURE_2D);
         }
    }

   glEndList();

   //count.free(); 
}
void svSummaryGlyph::RenderFont()
{
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING); 
    glDisable(GL_LIGHT0);
    glDisable(GL_TEXTURE_2D);
    for(int i=0;i<summaryPos.size();i++)
    {
         if(summaryCount[i]>0)
         {

                double radius = glyphRadius;
                double rotate_x = acos(summaryDir[i][2]);
                if(summaryDir[i][1] > 0)
                {
                        rotate_x = -rotate_x;
                }
                double rotate_z = acos(summaryDir[i][1]/sqrt(summaryDir[i][0]*summaryDir[i][0] + summaryDir[i][1]*summaryDir[i][1]));
                if(rotate_x < 0)
                {
                        if(summaryDir[i][0] > 0)
                        {
                                rotate_z = -rotate_z;
                        }
                }
                else
                {
                        rotate_z = PI - rotate_z;
                        if (summaryDir[i][0] <0)
                        {
                                rotate_z = - rotate_z;
                        }
                }
               glPushMatrix();
               glTranslatef(summaryPos[i][0], summaryPos[i][1], summaryPos[i][2]);
                glRotatef(rotate_z/M_PI*180,0,0,1);
                glRotatef(rotate_x/M_PI*180,1,0,0);

                svVector3 norm;// = svGetPerpendicularVector(summaryDir[i]);
                svVector3 head_out;
                norm[0] = 0;norm[1]=0;norm[2]=1;

                glColor3f(0,0,0);
                for(int jjj=0;jjj<=summaryMaxDen[i];)
                {
                  svVector3 vv =  svGetPerpendicularVector(norm);
                  head_out = (svScalar)(jjj * radius) * vv;head_out[2] = 0.25;
                  char str[10];
                  sprintf(str, "%d", jjj-scaling);
                  glRasterPos3f(head_out[0],head_out[1],head_out[2]);
                  for(int j=0;j<strlen(str);j++)
                      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
                  jjj+=2;
                }


                glPopMatrix();
          }
     }

     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
    // glEnable(GL_LIGHT1);
}
void svSummaryGlyph::RenderColor()
{/*
        ViewProperty viewproperty;
       glGetDoublev (GL_MODELVIEW_MATRIX, viewproperty.mvmatrix);
    glGetDoublev (GL_PROJECTION_MATRIX, viewproperty.projmatrix);
    glGetIntegerv( GL_VIEWPORT, viewproperty.viewport );

        glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(0.0,(GLfloat) viewproperty.viewport[2], 0.0, (GLfloat) viewproperty.viewport[3]);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glDisable(GL_LIGHTING);

*/
        glPushMatrix();
        glScalef(10,20,1);

        char str[50];
        float x = 1;
        float y = 1;
        float size = 1;
        float m_length_in = 1;

        glColor3f(0,0,0);

        double index;

     svScalarArray value;


        sprintf(str, "Magnitude");
         glRasterPos2f(x ,(numPower+2)*y*(size) );
                        for(int j=0;j<strlen(str);j++)
                             glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
    for(int i=numPower-1;i>=0;i--)
     {
          value.add((svScalar)i);
     }
     for(int i=numPower-1;i>=0;i--)

        {
            glColor3f(summaryDenColor [i][0],
                                    summaryDenColor[i][1],
                                   summaryDenColor[i][2]);

                        glBegin(GL_QUADS);
                        glVertex2f(x,y);
                        glVertex2f(x+size,y);
                        glVertex2f(x+size,y+size);
                        glVertex2f(x,y+size);
                        glEnd();

        glColor3f(0,0,0);

                        sprintf(str,"1e%0.0f",value[i]-scaling);
                        glRasterPos2f(x + size*1.1, y );
                        for(int j=0;j<strlen(str);j++)
                             glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);

                        y = y + size;

        }
value.free();
  glPopMatrix();
  /*              glMatrixMode(GL_PROJECTION);
               glLoadMatrixd(viewproperty.projmatrix);
                glMatrixMode(GL_MODELVIEW);
                glLoadMatrixd(viewproperty.mvmatrix);
*/
}
svSummaryGlyph::~svSummaryGlyph()
{
 cleanup();
}

void svSummaryGlyph::Render()
{
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glDisable( GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

   glCallList(display_list);
}

void svSummaryGlyph::SetColors()
{
//     summaryColor.free();
//     summaryDenColor.free();
//cerr<<"colors"<<endl;
     svColors *color = new svColors();
     for(int i=0;i<summaryPos.size();i++)
     {
              int c =summaryLabel[i];
             summaryColor.add(color->GetDiscreteColors(c));
     }

     svScalarArray value;

     for(int i=numPower-1;i>=0;i--)
     {
          value.add((svScalar)i);
            svVector4 c;
           c[0]=1;c[1]=1;c[2]=1;c[3]=1;
           summaryDenColor.add(c);
     }
//cerr<<"colors"<<endl;
     summaryDenColor = color->GetContinuousColors(value);
//cerr<<"colors"<<endl;
     //value.free();
     delete color;

}

void svSummaryGlyph::cleanup()
{
  int numDisc = summaryPos.size();
   summaryPos.free();
   summaryDir.free();
   summaryDen.free();
   summaryLabel.free();
   summaryColor.free();
   summaryDenColor.free();  
   summaryMaxDen.free();
   summaryCount.free();


  if(denDistribute!=NULL)
  { 
   for(int i=0;i<numDisc;i++)
   {
      denDistribute[i].free();
   }
   delete [] denDistribute;
  }
  if(visibleDistribute!=NULL)
  {
   for(int i=0;i<numDisc;i++)
   {
      visibleDistribute[i].free();
   }
   delete [] visibleDistribute;
  }
}


}
