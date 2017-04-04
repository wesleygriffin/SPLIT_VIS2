#include "svBendBar.h"
#include <algorithm>
#include <fstream>
#include <string.h>

using namespace std;

namespace __svl_lib{

svBendBar::svBendBar(svVectorField *f) : svBarGlyph(f)
{

}

void svBendBar::SaveToOBJ(char *fname,BendProperty &property,svVector3 planeDir)
{

  ofstream outfile(fname);

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
//cerr<<sortbar.size()<<endl;
      sort(sortbar.begin(), sortbar.end(), compare_sortbar);
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
//cerr<<"v "<<p2[0]<<" "<<p2[1]<<" "<<p2[2]<<endl;

             outfile<<"v "<<p2[0]<<" "<<p2[1]<<" "<<p2[2]<<endl;
             outfile<<"v "<<p1[0]<<" "<<p1[1]<<" "<<p1[2]<<endl;
             outfile<<"v "<<p3[0]<<" "<<p3[1]<<" "<<p3[2]<<endl;
             outfile<<"v "<<p4[0]<<" "<<p4[1]<<" "<<p4[2]<<endl;
          }
       }
      if(sortbar.size()>0)
       outfile<<endl;

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

             svVector3 shiftv;
             shiftv = normalize(cross(dir[i][j], v));
             outfile<<"vn "<<shiftv[0]<<" "<<shiftv[1]<<shiftv[2]<<endl;
           }
    } 
   if(sortbar.size()>0)
    outfile<<endl;

      int count= 0;
      for(int ii=0;ii<sortbar.size();ii++)
      {
            int j = sortbar[ii].index;
           if(sampleLabel[i][j])
          {
              outfile<<"f "<<count*4+1<<"//"<<count+1<<" "
                   <<count*4+2<<"//"<<count+1<<" "
                   <<count*4+3<<"//"<<count+1<<" "
                   <<count*4+4<<"//"<<count+1<<endl; 
             count++;
           }
      }
      sortbar.clear();
   }

  outfile.close();
}
svVector3 svBendBar::GetEnd(BendProperty &property, ViewProperty &viewproperty,
               svVector3 planeDir,
                int seed, int index)
{
   int s = property.bendRange.size();
   int legends = property.bendLegendRange.size();//cerr<<s<<" "<<legends<<endl;
   int mindex = s-1;
   int lindex = legends-1;
          for(int t=0;t<s;t++)
          {
             if(mag[seed][index] < property.bendRange[t])
             {
                 mindex = t - 1;
                 break;
             }
          }
          for(int t=0;t<legends;t++)
          {
             if(mag[seed][index] < property.bendLegendRange[t])
             {
                 lindex = t - 1;
                 break;
             }
          }

               dir[seed][index].normalize();
             svVector3 v1 = dir[seed][index];
             svVector3 v2 = planeDir;
             v2.normalize();
             svVector3 v = cross(v1, v2);
             v.normalize();

             svScalar height = mag[seed][index]/property.bendRange[mindex]
                           * property.bendHeight[mindex] * barproperty.scaley;
             svScalar width = property.bendWidth[mindex] * barproperty.scalex;

            svVector3 shiftv = normalize(cross(dir[seed][index], v));
             svScalar shift;
             shift = barproperty.circleradius/4.;
             svVector3 eyev  = viewproperty.eye; eyev.normalize();
             if(dot(shiftv, eyev)<0)shift=-shift;

           svVector3 p3;int i = seed; int j = index;
             p3[0] = glyph[i][j][0] + v[0] * width+dir[i][j][0]*height+shift*shiftv[0];
             p3[1] = glyph[i][j][1] + v[1] * width+dir[i][j][1]*height+shift*shiftv[1];
             p3[2] = glyph[i][j][2] + v[2] * width+dir[i][j][2]*height+shift*shiftv[2];

          return p3;
}
void svBendBar::GenerateLegend(BendProperty &property, svVector3 startPos, svVector3 dir1, svVector3 dir2,
                       svScalar stepD1, svScalar stepD2,
                       svInt stepN1, svInt stepN2, svVector3 planeDir)
{
        if(glIsList(legend_list))
                glDeleteLists(legend_list, 1);
    glNewList(legend_list, GL_COMPILE);
   glDisable(GL_LIGHTING);
   glColor3f(0.5,0.5,0.5);
 //  DrawGrid(startPos, dir1,  dir2,
   //                     stepD1,  stepD2,
     //                   stepN1,  stepN2);
  DrawOutline(false);
        glEndList();

}

void svBendBar::Generate(BendProperty &property, ViewProperty &viewproperty,
               svVector3 planeDir)
{
  cleanup();

  barEnd1 = new svVector3Array[seed_num];
  barEnd2 = new svVector3Array[seed_num];
  barEnd3 = new svVector3Array[seed_num];
  barEnd4 = new svVector3Array[seed_num];


  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  display_mode = SV_DISPLAYLIST;

  svGlyph::STILL_UPDATE = false;

        if(glIsList(display_list))
                glDeleteLists(display_list, 1);
    glNewList(display_list, GL_COMPILE);

   vector<SortBar> sortbar;
   

   int count = 0;
   int s = property.bendRange.size();
   int legends = property.bendLegendRange.size();//cerr<<s<<" "<<legends<<endl;
   svIntArray legendindex;
   for(int i=0;i<seed_num;i++)
   {
      for(int j=0;j<glyph[i].size();j++)
      {
          int index = s-1;
          int lindex = legends-1;
          for(int t=0;t<s;t++)
          {
             if(mag[i][j] < property.bendRange[t])
             {
                 index = t - 1;
                 break;
             }
          }
          for(int t=0;t<legends;t++)
          {
             if(mag[i][j] < property.bendLegendRange[t])
             {
                 lindex = t - 1;
                 break;
             }
          }
          legendindex.add(lindex);
          if(index >= 0)
          {
             count++;
             SortBar tmpbar;
             tmpbar.rangeindex = index;
             tmpbar.index = j;
             tmpbar.mag = mag[i][j];
              sortbar.push_back(tmpbar);
          }
      }
 
    //  sort(sortbar.begin(), sortbar.end(), compare_sortbar);
      for(int ii=0;ii<sortbar.size();ii++)
      {
           // cerr<<sortbar.size()<<endl;
                  int j = sortbar[ii].index;
            if(sampleLabel[i][j])//&&mag[i][j]>=1e-4)
            {
              int index = sortbar[ii].rangeindex;

             dir[i][j].normalize();
             svVector3 v1 = dir[i][j];
             svVector3 v2 = planeDir;
             v2.normalize();
             svVector3 v = cross(v1, v2);
             v.normalize();

             svVector3 shiftv;

             shiftv = glyph[i][j] -dir[i][j]*barproperty.circleradius;

             glColor3f(barproperty.circlecolor[0],barproperty.circlecolor[1],
                       barproperty.circlecolor[2]);//,barproperty.circlecolor[3]);
             glPushMatrix();
             glTranslatef(shiftv[0], shiftv[1], shiftv[2]);
             glutSolidSphere(barproperty.circleradius,8,8);
             glPopMatrix();
            
             svVector4 bendcolor = glyphColors[i][j];

             svScalar height = mag[i][j]/property.bendRange[index]
                           * property.bendHeight[index] * barproperty.scaley;
             svScalar width = property.bendWidth[index] * barproperty.scalex;

             shiftv = normalize(cross(dir[i][j], v));
             svScalar shift;
             shift = barproperty.circleradius/4.;
             svVector3 eyev  = viewproperty.eye; eyev.normalize();
             if(dot(shiftv, eyev)>0)shift=-shift;
             svVector3 p1, p2, p3, p4;
            svScalar legendh = property.bendLegendHeight[legendindex[j]]*barproperty.scaley;
            svScalar legendw = property.bendLegendWidth[legendindex[j]]*barproperty.scalex;
            // p1[0] = glyph[i][j][0] + v[0]*barproperty.legendw-shift*shiftv[0];
            // p1[1] = glyph[i][j][1] + v[1]*barproperty.legendw-shift*shiftv[1];
            // p1[2] = glyph[i][j][2] + v[2]*barproperty.legendw-shift*shiftv[2];
             p1[0] = glyph[i][j][0] + v[0]*legendw-shift*shiftv[0];
             p1[1] = glyph[i][j][1] + v[1]*legendw-shift*shiftv[1];
             p1[2] = glyph[i][j][2] + v[2]*legendw-shift*shiftv[2];
             p2[0] = glyph[i][j][0]-shift*shiftv[0];
             p2[1] = glyph[i][j][1]-shift*shiftv[1];
             p2[2] = glyph[i][j][2]-shift*shiftv[2];
            // p3[0] = glyph[i][j][0] + v[0]*barproperty.legendw+ dir[i][j][0]*barproperty.legendh-shift*shiftv[0];
            // p3[1] = glyph[i][j][1] + v[1]*barproperty.legendw+ dir[i][j][1]*barproperty.legendh-shift*shiftv[1];
            // p3[2] = glyph[i][j][2] + v[2]*barproperty.legendw+ dir[i][j][2]*barproperty.legendh-shift*shiftv[2];
            p3[0] = glyph[i][j][0] + v[0]*legendw+ dir[i][j][0]*legendh-shift*shiftv[0];
            p3[1] = glyph[i][j][1] + v[1]*legendw+ dir[i][j][1]*legendh-shift*shiftv[1];
            p3[2] = glyph[i][j][2] + v[2]*legendw+ dir[i][j][2]*legendh-shift*shiftv[2];
             p4[0] = glyph[i][j][0] + dir[i][j][0]*legendh-shift*shiftv[0];
             p4[1] = glyph[i][j][1] + dir[i][j][1]*legendh-shift*shiftv[1];
             p4[2] = glyph[i][j][2] + dir[i][j][2]*legendh-shift*shiftv[2];
             
           //  if(barproperty.legendh < width) cerr<<mag[i][j]<<endl;
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

                  glColor3f(barproperty.legendcolor[0], barproperty.legendcolor[1],
                        barproperty.legendcolor[2]);//, barproperty.legendcolor[3]); 
            barEnd1[i].add(p2);
            barEnd2[i].add(p1);
            barEnd3[i].add(p3);
            barEnd4[i].add(p4);           

            glDisable(GL_LIGHTING);
            glDisable(GL_LIGHT0);  
           
      //   if(barproperty.legendh  == width)
        //  {
        //
          //   glEnable(GL_POLYGON_OFFSET_FILL);
            // glPolygonOffset(1.0, 1.0);
          /*   glBegin(GL_LINE_LOOP);
             glNormal3f(shiftv[0], shiftv[1], shiftv[2]);
             glNormal3f(-shiftv[0], -shiftv[1], -shiftv[2]);
             glVertex3f(prep2[0],prep2[1],prep2[2]);
             glVertex3f(prep1[0],prep1[1],prep1[2]);
             glVertex3f(prep3[0],prep3[1],prep3[2]);
             glVertex3f(prep4[0],prep4[1],prep4[2]);
             glEnd();
*/
             svVector3 legendPos1, legendPos2;
             svScalar numLine = legendindex[j]+1;
              
             svScalar yd = legendw/numLine;//(legendh + legendw)/numLine;

            // glDisable(GL_POLYGON_OFFSET_FILL);

             svScalar shifty1 = yd;
             svScalar shiftx2 = yd;
             svScalar shifty2 = 0;
             svScalar shiftx1 = 0;
             glLineWidth(2.5);
          //   glColor3f(0.25,0.25,0.25);
             for(int t =0;t<numLine-1;t++)
             {
             /*    if(shifty1 > legendh)
                 {
                      shiftx1 = yd * (t+1) - legendh;
                      shifty1 = legendh;
                 }
                 if(shiftx2 > legendw)
                 {
                      shifty2 = -yd * (t+1) + legendw;
                      shiftx2 = legendw;
                 }

                 legendPos1 = prep4 + v * shiftx1 - dir[i][j] * shifty1;
                 legendPos2 = prep4 + v * shiftx2 + dir[i][j] * shifty2;
                 glBegin(GL_LINES);
                 glVertex3f(legendPos1[0], legendPos1[1], legendPos1[2]);
                 glVertex3f(legendPos2[0], legendPos2[1], legendPos2[2]);
                 glEnd();
                 shifty1 = shifty1 + yd;
                 shiftx2 = shiftx2 + yd;
*/
                 legendPos1 = prep2 + v * yd *(t+1) ;
                 legendPos2 = prep4 + v * yd *(t+1);
/*
                 glBegin(GL_LINES);
                 glVertex3f(legendPos1[0], legendPos1[1], legendPos1[2]);
                 glVertex3f(legendPos2[0], legendPos2[1], legendPos2[2]);
                 glEnd();
*/
             }
             glLineWidth(1.);

            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);


             glColor3f(bendcolor[0],bendcolor[1],bendcolor[2]);//,alpha);
             glBegin(GL_QUADS);
             glNormal3f(shiftv[0], shiftv[1], shiftv[2]);
             glNormal3f(-shiftv[0], -shiftv[1], -shiftv[2]);
             glVertex3f(p2[0],p2[1],p2[2]);
             glVertex3f(p1[0],p1[1],p1[2]);
             glVertex3f(p3[0],p3[1],p3[2]);
             glVertex3f(p4[0],p4[1],p4[2]);
             glEnd();
//}
         // svVector3 crossp;
          svVector3Array halop;
        glColor3f(1,1,1);//,alpha);
     svScalar halowidth = barproperty.halowidth;
                halop.add(p2); halop.add(p2-halowidth*v-halowidth*dir[i][j]);
                  halop.add(p1); halop.add(p1+halowidth*v-halowidth*dir[i][j]);
                  halop.add(p3); halop.add(p3+halowidth*v+halowidth*dir[i][j]);
                  halop.add(p4); halop.add(p4-halowidth*v+halowidth*dir[i][j]);
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
    /*    glColor3f(property.groupcolor[index][0], property.groupcolor[index][1], property.groupcolor[index][2]);
  */          /*    halop.add(prep2); halop.add(prep2-barproperty.halowidth*v-barproperty.halowidth*dir[i][j]);
                  halop.add(prep1); halop.add(prep1+barproperty.halowidth*v-barproperty.halowidth*dir[i][j]);
                  halop.add(prep3); halop.add(prep3+barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);
                  halop.add(prep4); halop.add(prep4-barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);
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
*/
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
/*
        glColor4f(barproperty.circlecolor[0],barproperty.circlecolor[1],
                 barproperty.circlecolor[2],barproperty.circlecolor[3]);  
        glLineWidth(1.);      
        glBegin(GL_LINES);
        glVertex3f(p4[0],p4[1],p4[2]);
        glVertex3f(p4[0]+dir[i][j][0]*barproperty.dirbarh,
                   p4[1]+dir[i][j][1]*barproperty.dirbarh,
                   p4[2]+dir[i][j][2]*barproperty.dirbarh);
        glEnd();
*/

      }
      }
      legendindex.free();
      sortbar.clear(); 
   }
//cerr<<count<<endl;
        glEndList();

}


void svBendBar::Generate(BendProperty &property, ViewProperty &viewproperty,
               svVector3 planeDir, int indexx)
{
  cleanup();

  barEnd1 = new svVector3Array[seed_num];
  barEnd2 = new svVector3Array[seed_num];
  barEnd3 = new svVector3Array[seed_num];
  barEnd4 = new svVector3Array[seed_num];


  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  display_mode = SV_DISPLAYLIST;

  svGlyph::STILL_UPDATE = false;

        if(glIsList(display_list))
                glDeleteLists(display_list, 1);
    glNewList(display_list, GL_COMPILE);

   vector<SortBar> sortbar;
   

   int count = 0;
   int s = property.bendRange.size();
   int legends = property.bendLegendRange.size();//cerr<<s<<" "<<legends<<endl;
   svIntArray legendindex;
   for(int i=indexx;i<indexx+1;i++)
   {
      for(int j=0;j<glyph[i].size();j++)
      {
          int index = s-1;
          int lindex = legends-1;
          for(int t=0;t<s;t++)
          {
             if(mag[i][j] < property.bendRange[t])
             {
                 index = t - 1;
                 break;
             }
          }
          for(int t=0;t<legends;t++)
          {
             if(mag[i][j] < property.bendLegendRange[t])
             {
                 lindex = t - 1;
                 break;
             }
          }
          legendindex.add(lindex);
          if(index >= 0)
          {
             count++;
             SortBar tmpbar;
             tmpbar.rangeindex = index;
             tmpbar.index = j;
             tmpbar.mag = mag[i][j];
              sortbar.push_back(tmpbar);
          }
      }
 
    //  sort(sortbar.begin(), sortbar.end(), compare_sortbar);
      for(int ii=0;ii<sortbar.size();ii++)
      {
           // cerr<<sortbar.size()<<endl;
                  int j = sortbar[ii].index;
            if(sampleLabel[i][j])//&&mag[i][j]>=1e-4)
            {
              int index = sortbar[ii].rangeindex;

             dir[i][j].normalize();
             svVector3 v1 = dir[i][j];
             svVector3 v2 = planeDir;
             v2.normalize();
             svVector3 v = cross(v1, v2);
             v.normalize();

             svVector3 shiftv;

             shiftv = glyph[i][j] -dir[i][j]*barproperty.circleradius;

             glColor3f(barproperty.circlecolor[0],barproperty.circlecolor[1],
                       barproperty.circlecolor[2]);//,barproperty.circlecolor[3]);
             glPushMatrix();
             glTranslatef(shiftv[0], shiftv[1], shiftv[2]);
             glutSolidSphere(barproperty.circleradius,8,8);
             glPopMatrix();
            
             svVector4 bendcolor = glyphColors[i][j];

             svScalar height = mag[i][j]/property.bendRange[index]
                           * property.bendHeight[index] * barproperty.scaley;
             svScalar width = property.bendWidth[index] * barproperty.scalex;

             shiftv = normalize(cross(dir[i][j], v));
             svScalar shift;
             shift = barproperty.circleradius/4.;
             svVector3 eyev  = viewproperty.eye; eyev.normalize();
             if(dot(shiftv, eyev)>0)shift=-shift;
             svVector3 p1, p2, p3, p4;
            svScalar legendh = property.bendLegendHeight[legendindex[j]]*barproperty.scaley;
            svScalar legendw = property.bendLegendWidth[legendindex[j]]*barproperty.scalex;
            // p1[0] = glyph[i][j][0] + v[0]*barproperty.legendw-shift*shiftv[0];
            // p1[1] = glyph[i][j][1] + v[1]*barproperty.legendw-shift*shiftv[1];
            // p1[2] = glyph[i][j][2] + v[2]*barproperty.legendw-shift*shiftv[2];
             p1[0] = glyph[i][j][0] + v[0]*legendw-shift*shiftv[0];
             p1[1] = glyph[i][j][1] + v[1]*legendw-shift*shiftv[1];
             p1[2] = glyph[i][j][2] + v[2]*legendw-shift*shiftv[2];
             p2[0] = glyph[i][j][0]-shift*shiftv[0];
             p2[1] = glyph[i][j][1]-shift*shiftv[1];
             p2[2] = glyph[i][j][2]-shift*shiftv[2];
            // p3[0] = glyph[i][j][0] + v[0]*barproperty.legendw+ dir[i][j][0]*barproperty.legendh-shift*shiftv[0];
            // p3[1] = glyph[i][j][1] + v[1]*barproperty.legendw+ dir[i][j][1]*barproperty.legendh-shift*shiftv[1];
            // p3[2] = glyph[i][j][2] + v[2]*barproperty.legendw+ dir[i][j][2]*barproperty.legendh-shift*shiftv[2];
            p3[0] = glyph[i][j][0] + v[0]*legendw+ dir[i][j][0]*legendh-shift*shiftv[0];
            p3[1] = glyph[i][j][1] + v[1]*legendw+ dir[i][j][1]*legendh-shift*shiftv[1];
            p3[2] = glyph[i][j][2] + v[2]*legendw+ dir[i][j][2]*legendh-shift*shiftv[2];
             p4[0] = glyph[i][j][0] + dir[i][j][0]*legendh-shift*shiftv[0];
             p4[1] = glyph[i][j][1] + dir[i][j][1]*legendh-shift*shiftv[1];
             p4[2] = glyph[i][j][2] + dir[i][j][2]*legendh-shift*shiftv[2];
             
           //  if(barproperty.legendh < width) cerr<<mag[i][j]<<endl;
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

                  glColor3f(barproperty.legendcolor[0], barproperty.legendcolor[1],
                        barproperty.legendcolor[2]);//, barproperty.legendcolor[3]); 
            barEnd1[i].add(p2);
            barEnd2[i].add(p1);
            barEnd3[i].add(p3);
            barEnd4[i].add(p4);           

            glDisable(GL_LIGHTING);
            glDisable(GL_LIGHT0);  
           
      //   if(barproperty.legendh  == width)
        //  {
        //
          //   glEnable(GL_POLYGON_OFFSET_FILL);
            // glPolygonOffset(1.0, 1.0);
          /*   glBegin(GL_LINE_LOOP);
             glNormal3f(shiftv[0], shiftv[1], shiftv[2]);
             glNormal3f(-shiftv[0], -shiftv[1], -shiftv[2]);
             glVertex3f(prep2[0],prep2[1],prep2[2]);
             glVertex3f(prep1[0],prep1[1],prep1[2]);
             glVertex3f(prep3[0],prep3[1],prep3[2]);
             glVertex3f(prep4[0],prep4[1],prep4[2]);
             glEnd();
*/
             svVector3 legendPos1, legendPos2;
             svScalar numLine = legendindex[j]+1;
              
             svScalar yd = legendw/numLine;//(legendh + legendw)/numLine;

            // glDisable(GL_POLYGON_OFFSET_FILL);

             svScalar shifty1 = yd;
             svScalar shiftx2 = yd;
             svScalar shifty2 = 0;
             svScalar shiftx1 = 0;
             glLineWidth(2.5);
          //   glColor3f(0.25,0.25,0.25);
             for(int t =0;t<numLine-1;t++)
             {
             /*    if(shifty1 > legendh)
                 {
                      shiftx1 = yd * (t+1) - legendh;
                      shifty1 = legendh;
                 }
                 if(shiftx2 > legendw)
                 {
                      shifty2 = -yd * (t+1) + legendw;
                      shiftx2 = legendw;
                 }

                 legendPos1 = prep4 + v * shiftx1 - dir[i][j] * shifty1;
                 legendPos2 = prep4 + v * shiftx2 + dir[i][j] * shifty2;
                 glBegin(GL_LINES);
                 glVertex3f(legendPos1[0], legendPos1[1], legendPos1[2]);
                 glVertex3f(legendPos2[0], legendPos2[1], legendPos2[2]);
                 glEnd();
                 shifty1 = shifty1 + yd;
                 shiftx2 = shiftx2 + yd;
*/
                 legendPos1 = prep2 + v * yd *(t+1) ;
                 legendPos2 = prep4 + v * yd *(t+1);
/*
                 glBegin(GL_LINES);
                 glVertex3f(legendPos1[0], legendPos1[1], legendPos1[2]);
                 glVertex3f(legendPos2[0], legendPos2[1], legendPos2[2]);
                 glEnd();
*/
             }
             glLineWidth(1.);

            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);


             glColor3f(bendcolor[0],bendcolor[1],bendcolor[2]);//,alpha);
             glBegin(GL_QUADS);
             glNormal3f(shiftv[0], shiftv[1], shiftv[2]);
             glNormal3f(-shiftv[0], -shiftv[1], -shiftv[2]);
             glVertex3f(p2[0],p2[1],p2[2]);
             glVertex3f(p1[0],p1[1],p1[2]);
             glVertex3f(p3[0],p3[1],p3[2]);
             glVertex3f(p4[0],p4[1],p4[2]);
             glEnd();
//}
         // svVector3 crossp;
          svVector3Array halop;
        glColor3f(1,1,1);//,alpha);
     svScalar halowidth = barproperty.halowidth;
                halop.add(p2); halop.add(p2-halowidth*v-halowidth*dir[i][j]);
                  halop.add(p1); halop.add(p1+halowidth*v-halowidth*dir[i][j]);
                  halop.add(p3); halop.add(p3+halowidth*v+halowidth*dir[i][j]);
                  halop.add(p4); halop.add(p4-halowidth*v+halowidth*dir[i][j]);
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
    /*    glColor3f(property.groupcolor[index][0], property.groupcolor[index][1], property.groupcolor[index][2]);
  */          /*    halop.add(prep2); halop.add(prep2-barproperty.halowidth*v-barproperty.halowidth*dir[i][j]);
                  halop.add(prep1); halop.add(prep1+barproperty.halowidth*v-barproperty.halowidth*dir[i][j]);
                  halop.add(prep3); halop.add(prep3+barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);
                  halop.add(prep4); halop.add(prep4-barproperty.halowidth*v+barproperty.halowidth*dir[i][j]);
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
*/
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
/*
        glColor4f(barproperty.circlecolor[0],barproperty.circlecolor[1],
                 barproperty.circlecolor[2],barproperty.circlecolor[3]);  
        glLineWidth(1.);      
        glBegin(GL_LINES);
        glVertex3f(p4[0],p4[1],p4[2]);
        glVertex3f(p4[0]+dir[i][j][0]*barproperty.dirbarh,
                   p4[1]+dir[i][j][1]*barproperty.dirbarh,
                   p4[2]+dir[i][j][2]*barproperty.dirbarh);
        glEnd();
*/

      }
      }
      legendindex.free();
      sortbar.clear(); 
   }
//cerr<<count<<endl;
        glEndList();

}
void svBendBar::Render()
{
  glCallList(display_list);
}

void svBendBar::RenderLegend(BendProperty &property)
{
    float x, y;
    x = 0.1; y =0;
    for(int i=0;i<property.bendLegendWidth.size();i++)
    {
          glColor3f(barproperty.legendcolor[0], barproperty.legendcolor[1],
                        barproperty.legendcolor[2]);
          glBegin(GL_LINE_LOOP);
          glVertex2f(x,y);
          glVertex2f(x+property.bendLegendWidth[i]*barproperty.scalex,y);
          glVertex2f(x+property.bendLegendWidth[i]*barproperty.scalex,
                      y+property.bendLegendHeight[i]*barproperty.scaley);
          glVertex2f(x,y+property.bendLegendHeight[i]*barproperty.scaley);
          glEnd();

            svScalar legendh = property.bendLegendHeight[i]*barproperty.scaley;
            svScalar legendw = property.bendLegendWidth[i]*barproperty.scalex;
             svVector3 legendPos1, legendPos2;
             svScalar numLine = i+1;

             svScalar yd = (legendw)/numLine;

           svVector3 prep4;
          prep4[0] = x;
          prep4[1] = y+property.bendLegendHeight[i]*barproperty.scaley;
          prep4[2] = 0;

           svVector3 prep2;
           prep2[0]=x;
           prep2[1] = y;
           prep2[2] = 0;

             svScalar shifty1 = yd;
             svScalar shiftx2 = yd;
             svScalar shifty2 = 0;
             svScalar shiftx1 = 0;
      //       glLineWidth(2.5);
      //       glColor3f(0.25,0.25,0.25);
             for(int t =0;t<numLine-1;t++)
             {
        /*         if(shifty1 > legendh)
                 {
                      shiftx1 = yd * (t+1) - legendh;
                      shifty1 = legendh;
                 }
                 if(shiftx2 > legendw)
                 {
                      shifty2 = -yd * (t+1) + legendw;
                      shiftx2 = legendw;
                 }

                 legendPos1[0] = prep4[0] + shiftx1;
                 legendPos1[1] = prep4[1] - shifty1;
                 legendPos2[0] = prep4[0] + shiftx2;
                 legendPos2[1] = prep4[1] + shifty2;
                 glBegin(GL_LINES);
                 glVertex2f(legendPos1[0], legendPos1[1]);
                 glVertex2f(legendPos2[0], legendPos2[1]);
                 glEnd();
*/
                 legendPos1[0] = prep2[0] + yd * (t+1);
                 legendPos1[1] = prep2[1];
                 legendPos2[0] = prep4[0] + yd * (t+1);
                 legendPos2[1] = prep4[1];
                 glBegin(GL_LINES);
                 glVertex2f(legendPos1[0], legendPos1[1]);
                 glVertex2f(legendPos2[0], legendPos2[1]);
                 glEnd();


         //        shifty1 = shifty1 + yd;
        //         shiftx2 = shiftx2 + yd;

             }
             glLineWidth(1.);
         glColor3f(0,0,0);       
            char str[10];
                sprintf(str,"%0.1f", property.bendLegendValue[i]);
        glRasterPos2f(x,y+legendh+0.1);
         for(int i=0;i<strlen(str);i++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);

          x = x + 100*property.bendLegendWidth[0]*barproperty.scalex;
    }
}

/*void svBendBar::RenderLegend(BendProperty &property)
{
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(0.0,(GLfloat) viewport[2], 0.0, (GLfloat) viewport[3]);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glDisable(GL_LIGHTING);

            for(int i=0;i<property.groupcolor.size();i++)
            {
                 glBegin(GL_QUADS);

                 glEnd(); 
            }

                glMatrixMode(GL_PROJECTION);
                glLoadMatrixd(pmat);
                glMatrixMode(GL_MODELVIEW);
                glLoadMatrixd(vmat);

}
*/
}
