#include "svBendBar.h"
#include <algorithm>
#include <string.h>

using namespace std;

namespace __svl_lib{

svBendBar::svBendBar(svVectorField *f) : svBarGlyph(f)
{

}

void svBendBar::Generate(BendProperty &property, ViewProperty &viewproperty,
               svVector3 planeDir)
{
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
 glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  display_mode = SV_DISPLAYLIST;

  svGlyph::STILL_UPDATE = false;

        if(glIsList(display_list))
                glDeleteLists(display_list, 1);
    glNewList(display_list, GL_COMPILE);

   vector<SortBar> sortbar;

   int s = property.bendRange.size();
   for(int i=0;i<seed_num;i++)
   {
      for(int j=0;j<glyph[i].size();j++)
      {
          int index = s-1;
          for(int t=0;t<s;t++)
          {
             if(mag[i][j] < property.bendRange[t])
             {
                 index = t - 1;
                 break;
             }
          }
          if(index >= 0)
          {
             SortBar tmpbar;
             tmpbar.rangeindex = index;
             tmpbar.index = j;
             tmpbar.mag = mag[i][j];
          
              sortbar.push_back(tmpbar);
          }
      }
      sort(sortbar.begin(), sortbar.end(), compare_sortbar);
      for(int ii=0;ii<sortbar.size();ii++)
      {
                  int j = sortbar[ii].index;
            if(sampleLabel[i][j])
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
                shift = glyph[i][j] + planeDir*barproperty.circleradius
                                    -v*barproperty.circleradius;
             }
             else
             {
                shift = glyph[i][j] - planeDir*barproperty.circleradius
                                    -v*barproperty.circleradius;
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
           if(sampleLabel[i][j])
          {
            int index = sortbar[ii].rangeindex;
            svVector4 bendcolor = glyphColors[i][j];

             svScalar height = mag[i][j]/property.bendRange[index]
                           * property.bendHeight[index] * barproperty.scaley;
             svScalar width = property.bendWidth[index] * barproperty.scalex;

             svVector3 v1 = dir[i][j];
             svVector3 v2 = planeDir;
             v2.normalize();
             svVector3 v = cross(v1, v2);
             v.normalize();

             svVector3 shiftv;
             shiftv = normalize(cross(dir[i][j], v));
             svScalar shift;
             shift = barproperty.circleradius/4.;
             svVector3 p1, p2, p3, p4;
             p1[0] = glyph[i][j][0] + v[0]*barproperty.legendw-shift*shiftv[0];
             p1[1] = glyph[i][j][1] + v[1]*barproperty.legendw-shift*shiftv[1];
             p1[2] = glyph[i][j][2] + v[2]*barproperty.legendw-shift*shiftv[2];
             p2[0] = glyph[i][j][0]-shift*shiftv[0];
             p2[1] = glyph[i][j][1]-shift*shiftv[1];
             p2[2] = glyph[i][j][2]-shift*shiftv[2];
             p3[0] = glyph[i][j][0] + v[0]*barproperty.legendw+ dir[i][j][0]*barproperty.legendh-shift*shiftv[0];
             p3[1] = glyph[i][j][1] + v[1]*barproperty.legendw+ dir[i][j][1]*barproperty.legendh-shift*shiftv[1];
             p3[2] = glyph[i][j][2] + v[2]*barproperty.legendw+ dir[i][j][2]*barproperty.legendh-shift*shiftv[2];
             p4[0] = glyph[i][j][0] + dir[i][j][0]*barproperty.legendh-shift*shiftv[0];
             p4[1] = glyph[i][j][1] + dir[i][j][1]*barproperty.legendh-shift*shiftv[1];
             p4[2] = glyph[i][j][2] + dir[i][j][2]*barproperty.legendh-shift*shiftv[2];
             
             svVector3 prep1, prep2, prep3, prep4;
             prep1 = p1;
             prep2 = p2;
             prep3 = p3;
             prep4 = p4;

             p1[0] = glyph[i][j][0] + v[0] * width+shift*shiftv[0];
             p1[1] = glyph[i][j][1] + v[1] * width+shift*shiftv[1];
             p1[2] = glyph[i][j][2] + v[2] * width+shift*shiftv[2];
             p2[0] = glyph[i][j][0] +shift*shiftv[0];
             p2[1] = glyph[i][j][1] +shift*shiftv[1];
             p2[2] = glyph[i][j][2] +shift*shiftv[2];
             p3[0] = glyph[i][j][0] + v[0] * width+dir[i][j][0]*height+shift*shiftv[0];
             p3[1] = glyph[i][j][1] + v[1] * width+dir[i][j][1]*height+shift*shiftv[1];
             p3[2] = glyph[i][j][2] + v[2] * width+dir[i][j][2]*height+shift*shiftv[2];
             p4[0] = glyph[i][j][0] +dir[i][j][0]*height+shift*shiftv[0];
             p4[1] = glyph[i][j][1] +dir[i][j][1]*height+shift*shiftv[1];
             p4[2] = glyph[i][j][2] +dir[i][j][2]*height+shift*shiftv[2];

                  glColor4f(barproperty.legendcolor[0], barproperty.legendcolor[1],
                        barproperty.legendcolor[2], barproperty.legendcolor[3]); 
             
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

             glBegin(GL_QUADS);
             glNormal3f(shiftv[0], shiftv[1], shiftv[2]);
             glNormal3f(-shiftv[0], -shiftv[1], -shiftv[2]);
             glVertex3f(prep2[0],prep2[1],prep2[2]);
             glVertex3f(prep1[0],prep1[1],prep1[2]);
             glVertex3f(prep3[0],prep3[1],prep3[2]);
             glVertex3f(prep4[0],prep4[1],prep4[2]);
             glEnd();

            glColor4f(bendcolor[0],bendcolor[1],bendcolor[2],alpha);
             glBegin(GL_QUADS);
             glNormal3f(shiftv[0], shiftv[1], shiftv[2]);
             glNormal3f(-shiftv[0], -shiftv[1], -shiftv[2]);
             glVertex3f(p2[0],p2[1],p2[2]);
             glVertex3f(p1[0],p1[1],p1[2]);
             glVertex3f(p3[0],p3[1],p3[2]);
             glVertex3f(p4[0],p4[1],p4[2]);
             glEnd();

         // svVector3 crossp;
          svVector3Array halop;
         /* if(barproperty.legendw < width)
          {
              if(barproperty.legendh > height)
              {
                  glColor4f(barproperty.legendcolor[0], barproperty.legendcolor[1],
                        barproperty.legendcolor[2], barproperty.legendcolor[3]); 
                  glBegin(GL_QUADS);
                  glVertex3f(prep2[0],prep2[1],prep2[2]);
                  glVertex3f(prep1[0],prep1[1],prep1[2]);
                  glVertex3f(prep3[0],prep3[1],prep3[2]);
                  glVertex3f(prep4[0],prep4[1],prep4[2]);
                  glEnd();

                  crossp[0] = glyph[i][j][0] + barproperty.legendw * v[0] 
                           + height *dir[i][j][0];
                  crossp[1] = glyph[i][j][1] + barproperty.legendw * v[1] 
                           + height *dir[i][j][1];
                  crossp[2] = glyph[i][j][2] + barproperty.legendw * v[2] 
                           + height *dir[i][j][2];

                  glColor4f(bendcolor[0],bendcolor[1],bendcolor[2],alpha);
                  glBegin(GL_POLYGON);
                  glVertex3f(prep1[0],prep1[1],prep1[2]);
                  glVertex3f(p1[0],p1[1],p1[2]);
                  glVertex3f(p3[0],p3[1],p3[2]);
                  glVertex3f(crossp[0], crossp[1], crossp[2]);
                  glEnd();

                  halop.add(prep2); halop.add(prep2-barproperty.halowidth*v-barproperty.halowidth*dir[i][j]);
                  halop.add(p1); halop.add(p1+barproperty.halowidth*v-barproperty.halowidth*dir[i][j]);
                  halop.add(p3); halop.add(p3+barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);
                  halop.add(crossp); halop.add(crossp+barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);
                  halop.add(prep3); halop.add(prep3+barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);
                  halop.add(prep4); halop.add(prep4-barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);
              }
             else
            {
             glColor4f(barproperty.legendcolor[0], barproperty.legendcolor[1],
                        barproperty.legendcolor[2], barproperty.legendcolor[3]); 
             glBegin(GL_QUADS);
             glVertex3f(prep2[0],prep2[1],prep2[2]);
             glVertex3f(prep1[0],prep1[1],prep1[2]);
             glVertex3f(prep3[0],prep3[1],prep3[2]);
             glVertex3f(prep4[0],prep4[1],prep4[2]);
             glEnd();

             glColor4f(bendcolor[0],bendcolor[1],bendcolor[2],alpha);
             glBegin(GL_POLYGON);
             glVertex3f(prep3[0],prep3[1],prep3[2]);
             glVertex3f(prep4[0],prep4[1],prep4[2]);
             glVertex3f(p4[0],p4[1],p4[2]);
             glVertex3f(p3[0],p3[1],p3[2]);
             glEnd();
             glBegin(GL_POLYGON);
             glVertex3f(prep1[0],prep1[1],prep1[2]);
             glVertex3f(p1[0],p1[1],p1[2]);
             glVertex3f(p3[0],p3[1],p3[2]);
             glVertex3f(prep3[0],prep3[1],prep3[2]);
             glEnd();

                  halop.add(prep2); halop.add(prep2-barproperty.halowidth*v-barproperty.halowidth*dir[i][j]);
                  halop.add(p1); halop.add(p1+barproperty.halowidth*v-barproperty.halowidth*dir[i][j]);
                  halop.add(p3); halop.add(p3+barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);
                  halop.add(p4); halop.add(p4-barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);

           }
        }
        else
          {
              if(barproperty.legendh > height)
              {
            glColor4f(bendcolor[0],bendcolor[1],bendcolor[2],alpha);
             glBegin(GL_QUADS);
             glVertex3f(p2[0],p2[1],p2[2]);
             glVertex3f(p1[0],p1[1],p1[2]);
             glVertex3f(p3[0],p3[1],p3[2]);
             glVertex3f(p4[0],p4[1],p4[2]);
             glEnd();

             glColor4f(barproperty.legendcolor[0], barproperty.legendcolor[1],
                        barproperty.legendcolor[2], barproperty.legendcolor[3]);
             glBegin(GL_POLYGON);
             glVertex3f(p4[0],p4[1],p4[2]);
             glVertex3f(p3[0],p3[1],p3[2]);
             glVertex3f(prep3[0],prep3[1],prep3[2]);
             glVertex3f(prep4[0],prep4[1],prep4[2]);
             glEnd();
             glBegin(GL_POLYGON);
             glVertex3f(p1[0],p1[1],p1[2]);
             glVertex3f(prep1[0],prep1[1],prep1[2]);
             glVertex3f(prep3[0],prep3[1],prep3[2]);
             glVertex3f(p3[0],p3[1],p3[2]);
             glEnd();

                  halop.add(prep2); halop.add(prep2-barproperty.halowidth*v-barproperty.halowidth*dir[i][j]);
                  halop.add(prep1); halop.add(prep1+barproperty.halowidth*v-barproperty.halowidth*dir[i][j]);
                  halop.add(prep3); halop.add(prep3+barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);
                  halop.add(prep4); halop.add(prep4-barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);

           }
           else
           {
            glColor4f(bendcolor[0],bendcolor[1],bendcolor[2],alpha);
             glBegin(GL_QUADS);
             glVertex3f(p2[0],p2[1],p2[2]);
             glVertex3f(p1[0],p1[1],p1[2]);
             glVertex3f(p3[0],p3[1],p3[2]);
             glVertex3f(p4[0],p4[1],p4[2]);
             glEnd();

             crossp[0] = glyph[i][j][0] + width * v[0] 
                           + barproperty.legendh *dir[i][j][0];
             crossp[1] = glyph[i][j][1] + width * v[1]
                           + barproperty.legendh *dir[i][j][1];
             crossp[2] = glyph[i][j][2] + width * v[2]
                           + barproperty.legendh *dir[i][j][2];

             glColor4f(barproperty.legendcolor[0], barproperty.legendcolor[1],
                        barproperty.legendcolor[2], barproperty.legendcolor[3]);
             glBegin(GL_POLYGON);
             glVertex3f(p1[0],p1[1],p1[2]);
             glVertex3f(prep1[0],prep1[1],prep1[2]);
             glVertex3f(prep3[0],prep3[1],prep3[2]);
             glVertex3f(crossp[0],crossp[1],crossp[2]);
             glEnd();
                  halop.add(prep2); halop.add(prep2-barproperty.halowidth*v-barproperty.halowidth*dir[i][j]);
                  halop.add(prep1); halop.add(prep1+barproperty.halowidth*v-barproperty.halowidth*dir[i][j]);
                  halop.add(prep3); halop.add(prep3+barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);
                  halop.add(crossp); halop.add(crossp+barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);
                  halop.add(p3); halop.add(p3+barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);
                  halop.add(p4); halop.add(p4-barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);

           }
        }
*/
        glColor4f(1,1,1,alpha);
/*
                  halop.add(prep2); halop.add(prep2-barproperty.halowidth*v-barproperty.halowidth*dir[i][j]);
                  halop.add(prep1); halop.add(prep1+barproperty.halowidth*v-barproperty.halowidth*dir[i][j]);
                  halop.add(prep3); halop.add(prep3+barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);
                  halop.add(prep4); halop.add(prep4-barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);

        for(int t=0;t<halop.size()/2;t++)
        {
              glBegin(GL_POLYGON);
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
  */
                halop.add(p2); halop.add(p2-barproperty.halowidth*v-barproperty.halowidth*dir[i][j]);
                  halop.add(p1); halop.add(p1+barproperty.halowidth*v-barproperty.halowidth*dir[i][j]);
                  halop.add(p3); halop.add(p3+barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);
                  halop.add(p4); halop.add(p4-barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);
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

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

        glColor4f(barproperty.circlecolor[0],barproperty.circlecolor[1],
                 barproperty.circlecolor[2],barproperty.circlecolor[3]);  
        glLineWidth(1.);      
        glBegin(GL_LINES);
        glVertex3f(p4[0],p4[1],p4[2]);
        glVertex3f(p4[0]+dir[i][j][0]*barproperty.dirbarh,
                   p4[1]+dir[i][j][1]*barproperty.dirbarh,
                   p4[2]+dir[i][j][2]*barproperty.dirbarh);
        glEnd();


      }
      }

      sortbar.clear(); 
   }

        glEndList();

}


void svBendBar::Render()
{
  glCallList(display_list);
}
}
