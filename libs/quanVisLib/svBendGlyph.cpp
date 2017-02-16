
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include "svException.h"
#include "svUtil.h"



#include "svBendGlyph.h"

using namespace std;


namespace __svl_lib{

svBendGlyph::svBendGlyph(svVectorField *f):svCylinderGlyph(f)
{
   
  ENABLE_COLOR=false;
  ENABLE_LINEWIDTH = false;

  display_mode = SV_STORE;
  field->GetPhysicalDimension(&lbbox, &rbbox);

}
void svBendGlyph::Render()
{
   glCallList(display_list);
}
//called after storing data into glyph
void svBendGlyph::Generate(BendProperty &property)
{
        glEnable( GL_LINE_SMOOTH );
        glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//        glEnable(GL_DEPTH_TEST);
//        glEnable(GL_NORMALIZE);

  alpha = 0.5;
  display_mode = SV_DISPLAYLIST;

  svGlyph::STILL_UPDATE = false;

        if(glIsList(display_list))
                glDeleteLists(display_list, 1);
    glNewList(display_list, GL_COMPILE);


  for(int i=0;i<seed_num;i++)
  {
      for(int j=0;j<glyph[i].size();j++)
      {
         int index = property.bendRange.size() - 1;
         for(int t=0;t<property.bendRange.size();t++)
         {
              if(mag[i][j] < property.bendRange[t])
              {
                   index = t-1;
                   break;
              }
         }

         if(index >= 0)
         {
             svScalar width = property.bendWidth[index] * glyphRadius;
             svScalar height = property.bendHeight[index]  
                               * mag[i][j]/ property.bendRange[index];
             svScalar unitheight = property.bendHeight[0]; 
             
             svVector3 norm = svGetPerpendicularVector(dir[i][j]);
             svVector3 end = glyph[i][j] + height * dir[i][j];
             svVector3 head1 =  glyph[i][j] + width * norm;
             svVector3 head2 =  end + width * norm;

             cerr<<mag[i][j]<<" "<<property.bendRange[index]<<" "<<property.bendHeight[index]<<" "<<index<<" "<<height<<" "<<width<<" "<<CYLINDERSLICE<<endl;

             glColor4f(glyphColors[i][j][0],glyphColors[i][j][1],glyphColors[i][j][2],alpha);

             glEnable(GL_LIGHTING);
             glEnable(GL_LIGHT0);
             glBegin(GL_QUAD_STRIP);
             for(int t=0;t<=CYLINDERSLICE;t++)
             {
                   svScalar degree = t*360./CYLINDERSLICE;
                   svVector3 pos1 = svGetRotatePoint(head1, glyph[i][j], dir[i][j], degree);              
                   svVector3 pos2  = svGetRotatePoint(head2, end, dir[i][j], degree);
                   svVector3 norm1 = normalize(pos1 - glyph[i][j]);
                   svVector3 norm2 = normalize(pos2 - end);
                   glNormal3f(norm1[0], norm1[1], norm1[2]);
                   glVertex3f(pos1[0], pos1[1], pos1[2]);
                   glNormal3f(norm2[0], norm2[1], norm2[2]);
                   glVertex3f(pos2[0], pos2[1], pos2[2]);
             }
             glEnd();

             glBegin(GL_POLYGON);
             glNormal3f(-dir[i][j][0], -dir[i][j][1], -dir[i][j][2]);
             for(int t=0;t<=CYLINDERSLICE;t++)
             {
                   svScalar degree = t*360./CYLINDERSLICE;
                   svVector3 pos1 = svGetRotatePoint(head1, glyph[i][j], dir[i][j], degree);
                   glVertex3f(pos1[0], pos1[1], pos1[2]);
             }
             glEnd();

             glBegin(GL_POLYGON);
             glNormal3f(dir[i][j][0], dir[i][j][1], dir[i][j][2]);
             for(int t=0;t<=CYLINDERSLICE;t++)
             {
                   svScalar degree = t*360./CYLINDERSLICE;
                   svVector3 pos1 = svGetRotatePoint(head2, end, dir[i][j], degree);
                   glVertex3f(pos1[0], pos1[1], pos1[2]);
             }
             glEnd();

             glDisable(GL_LIGHTING);
             glDisable(GL_LIGHT0);

             glLineWidth(0.5);

             lproperty.vSegment = 10;
             lproperty.hSegment = 10;
             lproperty.color[0]=0.2;
             lproperty.color[1] = 0.2;
             lproperty.color[2] = 0.2;
             lproperty.color[3] = alpha;
             lproperty.vPos = glyph[i][j];
             lproperty.vDir = dir[i][j];
             lproperty.hPos = glyph[i][j];
             lproperty.hDir = dir[i][j];
             lproperty.vUnit = unitheight;
             lproperty.hUnit = property.bendWidth[0];

             glColor4f(lproperty.color[0], lproperty.color[1], lproperty.color[2], alpha);
             svScalar segment = height/unitheight; 
             for(int t = 1;t<=(int)segment;t++)
             {
                   end = glyph[i][j] + (svScalar)(unitheight * t) * dir[i][j];
                   svScalar shift = 0.005;
                   head1 = end + (width+shift) * norm;
                   glBegin(GL_LINE_LOOP);
                   for(int ii=0;ii<CYLINDERSLICE;ii++)
                   {
                       svScalar degree = ii*360./CYLINDERSLICE;
                       svVector3 pos = svGetRotatePoint(head1, end, dir[i][j], degree);
                       glVertex3f(pos[0], pos[1], pos[2]);
                   }
                   glEnd(); 
             }

             Draw_InnerLegend();
             Draw_WidthLegend(); 
         }
      }
  }

  glEndList();

}


}
