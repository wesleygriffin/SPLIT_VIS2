#include "svDirectBar.h"
#include <algorithm>

using namespace std;

namespace __svl_lib {

svDirectBar::svDirectBar(svVectorField *f) : svBarGlyph(f)
{

}
void svDirectBar::Generate(DirectProperty &property, ViewProperty &viewproperty,
                            svVector3 planeDir)
{
  cleanup();

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
 glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  display_mode = SV_DISPLAYLIST;

  svGlyph::STILL_UPDATE = false;

        if(glIsList(display_list))
                glDeleteLists(display_list, 1);
    glNewList(display_list, GL_COMPILE);

   vector<SortBar> sortbar;

   for(int i=0;i<seed_num;i++)
   {
      for(int j=0;j<glyph[i].size();j++)
      {
         if(roiLabel[i][j])
         {
             SortBar tmpbar;
             tmpbar.index = j;
             tmpbar.mag = mag[i][j];

             sortbar.push_back(tmpbar);
         }
      }
 //     sort(sortbar.begin(), sortbar.end(), compare_sortbar);

      for(int ii=0;ii<sortbar.size();ii++)
      {

                  int j = sortbar[ii].index;
            if(visibleLabel[i][j])
            {
              int index = sortbar[ii].rangeindex;

             svVector3 v1 = dir[i][j];
             svVector3 v2 = planeDir;
             v2.normalize();
             svVector3 v = cross(v1, v2);
             v.normalize();

             svVector3 shift;

             if(dot(dir[i][j], planeDir)>0)
             {
                shift = glyph[i][j] //+ planeDir*barproperty.circleradius
                                    -dir[i][j]*barproperty.circleradius;
             }
             else
             {
                shift = glyph[i][j] //- planeDir*barproperty.circleradius
                                    -dir[i][j]*barproperty.circleradius;
             }

             glColor4f(barproperty.circlecolor[0],barproperty.circlecolor[1],
                       barproperty.circlecolor[2],barproperty.circlecolor[3]);
             glPushMatrix();
             glTranslatef(shift[0], shift[1], shift[2]);
             glutSolidSphere(barproperty.circleradius,8,8);
             glPopMatrix();
           }

      }
      for(int ii=0;ii<sortbar.size();ii++)
      {
           int j = sortbar[ii].index;

           if(visibleLabel[i][j])
          {
             svVector3Array halop;

             svVector3 v1 = dir[i][j];
             svVector3 v2 = planeDir;
             v2.normalize();
             svVector3 v = cross(v1, v2);
             v.normalize();

             svVector3 shiftv;
             shiftv = normalize(cross(dir[i][j], v));
             svScalar shift;
             shift = barproperty.circleradius/4.;

//             glEnable(GL_POLYGON_OFFSET_FILL);
//               glPolygonOffset(0.1, 1.0);
//
             svVector3 eyev  = viewproperty.eye; eyev.normalize();
             if(dot(shiftv, eyev)>0)shift=-shift;

               svVector3 p1, p2, p3, p4;
               glDisable(GL_LIGHTING);
               glDisable(GL_LIGHT0);
/*
          if(barproperty.legend < mag[i][j])
          {
               glColor4f(barproperty.circlecolor[0],barproperty.circlecolor[1],
                       barproperty.circlecolor[2],barproperty.circlecolor[3]);
               glBegin(GL_LINES);
               p2 = glyph[i][j] +  mag[i][j]*barproperty.scaley * property.UnitHeight * dir[i][j]+shiftv*shift;
               p3 = p2 + barproperty.dirbarh * dir[i][j];//+shiftv*shift;
               glVertex3f(p2[0], p2[1], p2[2]);
               glVertex3f(p3[0], p3[1], p3[2]);
               glEnd();
          }
*/
               glEnable(GL_LIGHTING);
               glEnable(GL_LIGHT0);


               glColor4f(barproperty.legendcolor[0], barproperty.legendcolor[1], barproperty.legendcolor[2],barproperty.legendcolor[3]);
               svScalar lh = barproperty.legend*property.UnitHeight*barproperty.scaley;
               glBegin(GL_LINE_LOOP);
              glNormal3f(shiftv[0], shiftv[1], shiftv[2]);
              glNormal3f(-shiftv[0], -shiftv[1], -shiftv[2]);
               p1 = glyph[i][j]-shift*shiftv;
               p2 = glyph[i][j] + barproperty.scalex*property.width*v-shift*shiftv;
               p3 = p2 + lh * dir[i][j];
              p4 = p1 + lh *dir[i][j];
               glVertex3f(p1[0], p1[1], p1[2]);
               glVertex3f(p2[0], p2[1], p2[2]);
               glVertex3f(p3[0], p3[1], p3[2]);
               glVertex3f(p4[0], p4[1], p4[2]);

               glEnd();
               glColor4f(glyphColors[i][j][0],
                         glyphColors[i][j][1],
                         glyphColors[i][j][2],
                         alpha);


               glBegin(GL_QUADS);
              glNormal3f(shiftv[0], shiftv[1], shiftv[2]);
              glNormal3f(-shiftv[0], -shiftv[1], -shiftv[2]);
               p1 = glyph[i][j]+shift*shiftv;
               p2 = glyph[i][j] + barproperty.scalex*property.width*v+shift*shiftv;
               p3 = p2 + mag[i][j]*barproperty.scaley * property.UnitHeight * dir[i][j];
               p4 = glyph[i][j] + mag[i][j]*barproperty.scaley * property.UnitHeight * dir[i][j]+shift*shiftv;
               glVertex3f(p1[0], p1[1], p1[2]);
               glVertex3f(p2[0], p2[1], p2[2]);
               glVertex3f(p3[0], p3[1], p3[2]);
               glVertex3f(p4[0], p4[1], p4[2]);
               glEnd();

               barEnd1[i].add(p1);
               barEnd2[i].add(p2);
               barEnd3[i].add(p3);
               barEnd4[i].add(p4);

               halop.add(p1);halop.add(p1-barproperty.halowidth*v-barproperty.halowidth*dir[i][j]);
               halop.add(p2);halop.add(p2+barproperty.halowidth*v-barproperty.halowidth*dir[i][j]);
               halop.add(p3);halop.add(p3+barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);
               halop.add(p4);halop.add(p4-barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);

//             glDisable(GL_POLYGON_OFFSET_FILL);


        glColor4f(1,1,1,alpha);
        for(int t=0;t<halop.size()/2;t++)
        {
              glBegin(GL_POLYGON);
              glNormal3f(shiftv[0], shiftv[1], shiftv[2]);
              glNormal3f(-shiftv[0], -shiftv[1], -shiftv[2]);

              glVertex3f(halop[t*2][0],halop[t*2][1],halop[t*2][2]);
              glVertex3f(halop[t*2+1][0],halop[t*2+1][1],halop[t*2+1][2]);
              if(t<halop.size()/2-1)
              {
                glVertex3f(halop[t*2+3][0],halop[t*2+3][1],halop[t*2+3][2]);
                glVertex3f(halop[t*2+2][0],halop[t*2+2][1],halop[t*2+2][2]);
              }
              else
              {
                glVertex3f(halop[1][0],halop[1][1],halop[1][2]);
                glVertex3f(halop[0][0],halop[0][1],halop[0][2]);
              }
              glEnd();
        }
        halop.free();
      }
       }
       sortbar.clear();
        
       }
        glEndList();

    }

void svDirectBar::Render()
{   
  glCallList(display_list);
}

svVector3 svDirectBar::GetEnd(DirectProperty &property, ViewProperty &viewproperty,
               svVector3 planeDir,
                int seed, int index)
{
           int i = seed;
           int j = index;
               dir[seed][index].normalize();
             svVector3 v1 = dir[seed][index];
             svVector3 v2 = planeDir;
             v2.normalize();
             svVector3 v = cross(v1, v2);
             v.normalize();

             svVector3 shiftv;
             shiftv = normalize(cross(dir[i][j], v));
             svScalar shift;
             shift = barproperty.circleradius/4.;

             svVector3 eyev  = viewproperty.eye; eyev.normalize();
             if(dot(shiftv, eyev)<0)shift=-shift;

               svVector3 p1, p2, p3, p4;

                p1 = glyph[i][j]+shift*shiftv;
               p2 = glyph[i][j] + barproperty.scalex*property.width*v+shift*shiftv;
               p3 = p2 + mag[i][j]*barproperty.scaley * property.UnitHeight * dir[i][j]; 

             return p3;
}
}
