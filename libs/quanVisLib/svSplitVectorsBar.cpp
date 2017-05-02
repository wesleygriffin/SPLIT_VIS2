#include "svSplitVectorsBar.h"
#include <algorithm>

using namespace std;

namespace __svl_lib {



svSplitVectorsBar::svSplitVectorsBar(svVectorField *f) : svBarGlyph(f)
{

}

void svSplitVectorsBar::Generate(SplitVectorsProperty &property,ViewProperty &viewproperty,
                            svVector3 planeDir)
{
  cleanup();

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
 glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);


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
//      sort(sortbar.begin(), sortbar.end(), compare_sortbar);

      for(int ii=0;ii<sortbar.size();ii++)
      {
            int j = sortbar[ii].index;
            if(visibleLabel[i][j])
            {

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
           svScalar exp = getNumOfIntegerDigits(mag[i][j]);
           svScalar coe = mag[i][j]/pow(10., exp);

           exp = exp + property.shiftexp;
//cerr<<coe<<" "<<exp<<endl;
             svVector3 v1 = dir[i][j];
             svVector3 v2 = planeDir;
             v2.normalize();
             svVector3 v = cross(v1, v2);
             v.normalize();

             svVector3 shiftv;
             shiftv = normalize(cross(dir[i][j], v));
             svScalar shift;
             shift = barproperty.circleradius/4.;

             svVector3 eyev  = viewproperty.eye; eyev.normalize();
             if(dot(shiftv, eyev)>0)shift=-shift;

               svVector3 coep;
               svScalar cw = property.coeWidth * barproperty.scalex;
               svScalar ch = property.coeHeight * coe * barproperty.scaley;
               svVector3 expp;
               svScalar ew = property.expWidth* barproperty.scalex;
               svScalar eh = property.expHeight * exp * barproperty.scaley;
               expp = glyph[i][j]-shiftv*shift;
              coep = glyph[i][j] +
                       (svScalar)(ew/2.-cw/2.) * v + shiftv*shift;

               glDisable(GL_LIGHTING);
               glDisable(GL_LIGHT0);
               glColor4f(barproperty.legendcolor[0], barproperty.legendcolor[1], barproperty.legendcolor[2],barproperty.legendcolor[3]);
               glLineWidth(1.);
               svScalar lh =barproperty.legend * pow(10, property.shiftexp) *property.expHeight*barproperty.scaley;
//(getNumOfIntegerDigits(barproperty.legend)+property.shiftexp)*property.expHeight*barproperty.scaley;
              glLineWidth(1.5);
               glBegin(GL_LINES);
               glVertex3f(coep[0] + v[0] * cw/2.+ shiftv[0]*shift,
                       coep[1] + v[1] * cw/2.+ shiftv[1]*shift,
                       coep[2] + v[2] * cw/2.+ shiftv[2]*shift);
               glVertex3f(coep[0] + v[0] * cw/2.+lh*dir[i][j][0]+ shiftv[0]*shift,
                       coep[1] + v[1] * cw/2.+lh*dir[i][j][1]+ shiftv[1]*shift,
                       coep[2] + v[2] * cw/2.+lh*dir[i][j][2]+ shiftv[2]*shift);
               glEnd();
               glLineWidth(1.);

             glEnable(GL_LIGHTING);
             glEnable(GL_LIGHT0); 



              glColor4f(property.coeColor[0], property.coeColor[1], property.coeColor[2], property.coeColor[3]);
               glBegin(GL_QUADS);
            glNormal3f(shiftv[0], shiftv[1], shiftv[2]);
             glNormal3f(-shiftv[0], -shiftv[1], -shiftv[2]);
               glVertex3f(coep[0],coep[1],coep[2]);
               glVertex3f(coep[0]+v[0]*cw,
                           coep[1]+v[1]*cw,
                           coep[2]+v[2]*cw);
               glVertex3f(coep[0]+v[0]*cw+dir[i][j][0]*ch,
                           coep[1]+v[1]*cw+dir[i][j][1]*ch,
                           coep[2]+v[2]*cw+dir[i][j][2]*ch);
               glVertex3f(coep[0]+dir[i][j][0]*ch,
                           coep[1]+dir[i][j][1]*ch,
                           coep[2]+dir[i][j][2]*ch);
               glEnd();

               glColor4f(glyphColors[i][j][0],
                         glyphColors[i][j][1],
                         glyphColors[i][j][2],
                         alpha);
               glBegin(GL_QUADS);
            glNormal3f(shiftv[0], shiftv[1], shiftv[2]);
             glNormal3f(-shiftv[0], -shiftv[1], -shiftv[2]);
               glVertex3f(expp[0],expp[1],expp[2]);
               glVertex3f(expp[0]+v[0]*ew,
                           expp[1]+v[1]*ew,
                           expp[2]+v[2]*ew);
               glVertex3f(expp[0]+v[0]*ew+dir[i][j][0]*eh,
                           expp[1]+v[1]*ew+dir[i][j][1]*eh,
                           expp[2]+v[2]*ew+dir[i][j][2]*eh);
               glVertex3f(expp[0]+dir[i][j][0]*eh,
                           expp[1]+dir[i][j][1]*eh,
                           expp[2]+dir[i][j][2]*eh);
               glEnd();
             

              if(exp > coe)
              { 
               barEnd1[i].add(expp);
               barEnd2[i].add(expp + v * ew);
               barEnd3[i].add(expp + v*ew + dir[i][j] * eh);
               barEnd4[i].add(expp+dir[i][j] * eh);
              }
              else
              {
               barEnd1[i].add(coep);
               barEnd2[i].add(coep + v * cw);
               barEnd3[i].add(coep + v*cw + dir[i][j] * ch);
               barEnd4[i].add(coep+dir[i][j] * ch);
              }

//                glDisable(GL_POLYGON_OFFSET_FILL);

           }
          }  
          sortbar.clear();      
      }

        glEndList();

  }
void svSplitVectorsBar::Render()
{
  glCallList(display_list);
}
svVector3 svSplitVectorsBar::GetEnd(SplitVectorsProperty &property, ViewProperty &viewproperty,
               svVector3 planeDir,
                int seed, int index)
{
int i = seed;
int j = index;

           svScalar exp = getNumOfIntegerDigits(mag[i][j]);
           svScalar coe = mag[i][j]/pow(10., exp);

           exp = exp + property.shiftexp;

               dir[seed][index].normalize();
             svVector3 v1 = dir[seed][index];
             svVector3 v2 = planeDir;
             v2.normalize();
             svVector3 v = cross(v1, v2);
             v.normalize();
          svVector3 p3;

             svVector3 shiftv;
             shiftv = normalize(cross(dir[i][j], v));
             svScalar shift;
             shift = barproperty.circleradius/4.;

               svVector3 coep;
               svScalar cw = property.coeWidth * barproperty.scalex;
               svScalar ch = property.coeHeight * coe * barproperty.scaley;
               svVector3 expp;
               svScalar ew = property.expWidth* barproperty.scalex;
               svScalar eh = property.expHeight * exp * barproperty.scaley;
               expp = glyph[i][j]-shiftv*shift;
              coep = glyph[i][j] +
                       (svScalar)(ew/2.-cw/2.) * v + shiftv*shift;


             svVector3 eyev  = viewproperty.eye; eyev.normalize();
             if(dot(shiftv, eyev)<0)shift=-shift;

     if(exp > coe)
     {
         p3[0] =expp[0]+v[0]*ew+dir[i][j][0]*eh;
         p3[1] =expp[1]+v[1]*ew+dir[i][j][1]*eh;
         p3[2] =expp[2]+v[2]*ew+dir[i][j][2]*eh;
     }
    else
     {
                 p3[0]=coep[0]+v[0]*cw+dir[i][j][0]*ch;
                       p3[1]=    coep[1]+v[1]*cw+dir[i][j][1]*ch;
                        p3[2]=   coep[2]+v[2]*cw+dir[i][j][2]*ch;
      }
          return p3;
}
}
