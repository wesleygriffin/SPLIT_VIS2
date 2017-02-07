#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include "svSummaryGlyph.h"
#include "svException.h"
#include "svUtil.h"

using namespace std;

namespace __svl_lib {

svSummaryGlyph::svSummaryGlyph():svGlyph()
{
      denDistribute = NULL;

      numPower = 0;
}

void svSummaryGlyph::RenderAnnulus(svVector3 pos, svVector3 vel,
                                svScalar x, svScalar H, svScalar r)
{
        double degree =1;
        int times1 = x/degree;  
        int times2 = (360-x)/degree;

        svVector3 norm = svGetPerpendicularVector(vel);
        svVector3 head_out, head_in;
        head_out = pos + H * norm;
        head_in  = pos + r * norm;

        glDisable(GL_LIGHTING);
        glBegin(GL_QUAD_STRIP);
        glNormal3f(vel[0], vel[1], vel[2]);
        glNormal3f(-vel[0], -vel[1], -vel[2]);
        for(int ii = 0; ii <= times1; ii++)
        {
                float i = (float)ii * (x/(float)times1);

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
        glNormal3f(-vel[0], -vel[1], -vel[2]);
        for(int ii = 0; ii <= times2; ii++)
        {
                float i = x +  (float)ii * ((360-x)/(float)times2);

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

//cerr<<maxClusterLabel<<endl;
  svScalarArray count;
//cerr<<glyphScale<< " "<<numPower<<endl;
  for(int i=0;i<maxClusterLabel+2;i++)
  {
      svVector3 p;
      p[0]=0;p[1]=0;p[2]=0;
      summaryPos.add(p);
      summaryDir.add(p);
      summaryDen.add(0);
      count.add(0);
      summaryLabel.add(i-1);
     for(int j=0;j<numPower;j++)
      {
        denDistribute[i].add(0);
      }
  }
  for(int i=0;i<seed_num;i++)
  {
     for(int j=0;j<glyph[i].size();j++)
     {
         int in = clusterLabel[i][j]+1;
         
         summaryPos[in][0] = summaryPos[in][0] + glyph[i][j][0];  
         summaryPos[in][1] = summaryPos[in][1] + glyph[i][j][1];
         summaryPos[in][2] = summaryPos[in][2] + glyph[i][j][2];  
         summaryDir[in][0] = summaryDir[in][0] + dir[i][j][0];
         summaryDir[in][1] = summaryDir[in][1] + dir[i][j][1];
         summaryDir[in][2] = summaryDir[in][2] + dir[i][j][2];
         summaryDen[in] = summaryDen[in] + mag[i][j];

         int num = getNumOfIntegerDigits(mag[i][j]);
         denDistribute[in][numPower-num-scaling-1]++;

         count[in] ++;
     } 
  }  

   //cerr<<summaryPos.size()<<endl;

  for(int i=0;i<summaryPos.size();i++)
  {
      if(count[i]>0)
      {
         summaryPos[i][0] = summaryPos[i][0]/count[i];
         summaryPos[i][1] = summaryPos[i][1]/count[i];
         summaryPos[i][2] = summaryPos[i][2]/count[i];
         summaryDir[i][0] = summaryDir[i][0]/count[i];
         summaryDir[i][1] = summaryDir[i][1]/count[i];
         summaryDir[i][2] = summaryDir[i][2]/count[i];
         summaryDen[i] = summaryDen[i]/count[i];
        // cerr<<i<<" "<<summaryDen[i]<<" "<<glyphScale<<" "<<count[i]<<" ";
         for(int j=0;j<numPower;j++)
         {
             denDistribute[i][j] = denDistribute[i][j]/count[i];
	    // cerr<<denDistribute[i][j]<<" ";
         }//cerr<<endl;
         summaryDir[i].normalize();
      }
    //  cerr<<summaryLabel[i]<<" "<<count[i]<<endl;;
  }//cerr<<endl;

 SetColors();
//==================display list===================================

  display_mode = SV_DISPLAYLIST;
  svGlyph::STILL_UPDATE = false;

        if(glIsList(display_list))
                glDeleteLists(display_list, 1);
    glNewList(display_list, GL_COMPILE);


    for(int i=0;i<summaryPos.size();i++)
    {
         if(count[i]>0)
         {
                double radius = glyphRadius;//*summaryDen[i];

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
                         alpha);

               svVector3 end;
               svScalar s = summaryDen[i] * glyphScale;
               end[0] = summaryPos[i][0]+s*summaryDir[i][0];
               end[1] = summaryPos[i][1]+s* summaryDir[i][1];
               end[2] = summaryPos[i][2]+s* summaryDir[i][2];

               glPushMatrix();
               glTranslatef(end[0], end[1], end[2]);
                glRotatef(rotate_z/M_PI*180,0,0,1);
                glRotatef(rotate_x/M_PI*180,1,0,0);
                glutSolidCone(radius, radius*3, 5, 1);
                glPopMatrix();

                glDisable(GL_LIGHTING);
                glDisable(GL_LIGHT0);

                glLineWidth(3.);
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
                glEnable(GL_POLYGON_OFFSET_FILL);
                glPolygonOffset(1.0, 2);

                svVector3 mid;
                mid[0] = end[0] + summaryPos[i][0];
                mid[1] = end[1] + summaryPos[i][1];
                mid[2] = end[2] + summaryPos[i][2];

                mid[0]  = mid[0]/2;
                mid[1] = mid[1]/2;
                mid[2] = mid[2]/2;

               glPushMatrix();
               glTranslatef(mid[0], mid[1], mid[2]);
                glRotatef(rotate_z/M_PI*180,0,0,1);
                glRotatef(rotate_x/M_PI*180,1,0,0);
                glNormal3f(summaryDir[i][0], summaryDir[i][1], summaryDir[i][2]);
                for(int j=0;j<numPower;j++)
                {
                        glColor4f(summaryDenColor[j][0],
                                 summaryDenColor[j][1],
                                 summaryDenColor[j][2],
                                 alpha);
                       svVector3 p;
                      p[0]=0;p[1]=0;p[2]=0;
                       svVector3 v;
                       v[0]=0;v[1]=0;v[2]=1;
                        RenderAnnulus(p,v,
                                        denDistribute[i][j]*360,
                                        H, r);
                        H = H + radius;
                        r = r + radius;
                }

                glDisable(GL_LIGHTING);
                glDisable(GL_LIGHT0);

                glLineWidth(0.5);

                glColor4f(0.5,0.5,0.5,alpha);
                svVector3 norm;// = svGetPerpendicularVector(summaryDir[i]);
                svVector3 head_out;
               // head_out = mid + (svScalar)(radius * numPower) * norm;
                head_out[0]=radius*numPower;head_out[1]=0;head_out[2]=0;
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

                glPopMatrix();
                glLineWidth(1.);
                glDisable(GL_POLYGON_OFFSET_FILL);
         }
    }

           glEndList();

   count.free(); 
}

svSummaryGlyph::~svSummaryGlyph()
{
 cleanup();
}

void svSummaryGlyph::Render()
{
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

  if(denDistribute!=NULL)
  { 
   for(int i=0;i<numDisc;i++)
   {
      denDistribute[i].free();
   }
   delete [] denDistribute;
  }
}


}
