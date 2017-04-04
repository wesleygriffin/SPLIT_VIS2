
/* rendering routine of a vector field using glyphs */

#include "svMesh.h"

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


void svMesh::GenerateWireframe(char *file)
{
        if(glIsList(display_list))
                glDeleteLists(display_list, 1);
    glNewList(display_list, GL_COMPILE);

    glDisable(GL_LIGHTING);
  

    ifstream infile(file);

   // svVector3Array pos;
  //  svVector3Array index;
   if(infile.is_open())
   {
    int n1, n2;
    infile>>n1;//>>n2;
    for(int i=0;i<n1;i++)
    {
          svVector3 p[3];
         infile>>p[0][0]>>p[0][1]>>p[0][2];
         infile>>p[1][0]>>p[1][1]>>p[1][2];
         infile>>p[2][0]>>p[2][1]>>p[2][2];

         glBegin(GL_LINE_LOOP);
         for(int j=0;j<3;j++)
            glVertex3f(p[j][0], p[j][1], p[j][2]);
         glEnd();
     //    pos.add(p);
    }
/*
    for(int i=0;i<n2;i++)
    {
          svVector3 p;
         infile>>p[0]>>p[1]>>p[2];
         index.add(p);
    }
*/
    infile.close();
    }
    glEndList();
}

void svMesh::GenerateWiresolid(char *file)
{
        if(glIsList(display_list))
                glDeleteLists(display_list, 1);
    glNewList(display_list, GL_COMPILE);
   
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    glEnable( GL_LINE_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
 

    ifstream infile(file);
    int n1, n2;
    infile>>n1;
    for(int i=0;i<n1;i++)
    {
          svVector3 p[3];
         infile>>p[0][0]>>p[0][1]>>p[0][2];
         infile>>p[1][0]>>p[1][1]>>p[1][2];
         infile>>p[2][0]>>p[2][1]>>p[2][2];

         glBegin(GL_TRIANGLES);
         for(int j=0;j<3;j++)
         {
                svVector3 n = normalize(p[j]);
                glNormal3f(n[0],n[1],n[2]);
                glVertex3f(p[j][0], p[j][1], p[j][2]);
         }
         glEnd();
     }
    infile.close();
    glEndList();

}

void svMesh::Render()
{
    glCallList(display_list);
}

}


