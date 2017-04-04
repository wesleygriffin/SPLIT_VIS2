#include "svCylinderGlyph.h"


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


namespace __svl_lib{

svCylinderGlyph::svCylinderGlyph(svVectorField *f):svGlyph()
{
  field = f;

       cylinderEnd1 = new svVector3Array[1];
       cylinderEnd2 = new svVector3Array[1];
       cylinderEnd3 = new svVector3Array[1];
       cylinderEnd4 = new svVector3Array[1];
}
void svCylinderGlyph::Draw_InnerLegend()
{
 svVector3 pos = lproperty.vPos;
 svVector3 vel = lproperty.vDir;
 svScalar l = lproperty.vSegment;
 svScalar length = lproperty.vUnit;

 svVector3 end;

 end = pos + (l*length) * vel;

// glLineWidth(1);

 glColor4f(lproperty.color[0], lproperty.color[1], lproperty.color[2], lproperty.color[3]);

 glBegin(GL_LINES);

 glVertex3f(pos[0], pos[1], pos[2]);
 glVertex3f(end[0], end[1], end[2]);

 glEnd();

 svScalar fishbone_l = length;//2/3.;

 svVector3 v;

 for(int i=0;i<(int)l;i++)
 {
        svVector3 head = pos + vel * (i+1) * length;
        v = svGetPerpendicularVector(vel);
        svVector3 p = head - fishbone_l * vel;
        svVector3 p1 = p + v * fishbone_l*0.3;
        svVector3 p2 = p - v * fishbone_l*0.3;

        glBegin(GL_LINES);
        glVertex3f(head[0], head[1], head[2]);
        glVertex3f(p1[0], p1[1], p1[2]);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(head[0], head[1], head[2]);
        glVertex3f(p2[0], p2[1], p2[2]);
        glEnd();

 }

}

void svCylinderGlyph::Draw_WidthLegend()
{
 svVector3 pos = lproperty.hPos;
 svVector3 vel = lproperty.hDir;
 svScalar l = lproperty.hSegment;
 svScalar length = lproperty.hUnit;

 svVector3 v = svGetPerpendicularVector(vel);
 svVector3 end1;
 svVector3 end2;
 svScalar shift = 0.005;
 end1 = pos;// - vel * shift;
 end2 = pos+ (svScalar)(l*length) * v;//- vel * shift;


// glLineWidth(1);

 glColor4f(lproperty.color[0], lproperty.color[1], lproperty.color[2], lproperty.color[3]);
 glBegin(GL_LINES);

 glVertex3f(end1[0], end1[1], end1[2]);
 glVertex3f(end2[0], end2[1], end2[2]);

 glEnd();

 svVector3 top = pos + vel;

 svScalar fishbone_l = length;//2/3.;
 svVector3 v2 = normalize(svGetRotatePoint(top, pos, v, 90));

 for(int i=0;i<(int)l;i++)
 {
        svVector3 head = pos + v * (i+1) * length;
        v2 = svGetPerpendicularVector(v);
        svVector3 p = head - fishbone_l * v;
        svVector3 p1 = p + v2 * fishbone_l*0.3;
        svVector3 p2 = p - v2 * fishbone_l*0.3;

        glBegin(GL_LINES);
        glVertex3f(head[0], head[1], head[2]);
        glVertex3f(p1[0], p1[1], p1[2]);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(head[0], head[1], head[2]);
        glVertex3f(p2[0], p2[1], p2[2]);
        glEnd();
 }

}

void svCylinderGlyph::cleanup()
{
       if(cylinderEnd1 == NULL)
       {
             for(int i=0;i<seed_num;i++)
             {
                  cylinderEnd1[i].free();
             }
             delete [] cylinderEnd1;
             cylinderEnd1 = NULL;
       }
       if(cylinderEnd2 == NULL)
       {
             for(int i=0;i<seed_num;i++)
             {
                  cylinderEnd2[i].free();
             }
             delete [] cylinderEnd2;
             cylinderEnd2 = NULL;
       }
       if(cylinderEnd3 == NULL)
       {
             for(int i=0;i<seed_num;i++)
             {
                  cylinderEnd3[i].free();
             }
             delete [] cylinderEnd3;
             cylinderEnd3 = NULL;
       }
       if(cylinderEnd4 == NULL)
       {
             for(int i=0;i<seed_num;i++)
             {
                  cylinderEnd4[i].free();
             }
             delete [] cylinderEnd4;
             cylinderEnd4 = NULL;
       }

}

}
