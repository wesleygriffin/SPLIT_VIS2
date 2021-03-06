
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
#include "svColors.h"

using namespace std;

namespace __svl_lib {


void svMesh::New(vector<int> region)
{
    cleanup();

    int size = region.size();
    mesh = new svVector3Array[size];
    dir = new svVector3Array[size]; 
    colors = new svVector4Array[size];
    meshregion.resize(size);
    for(int i=0;i<region.size();i++)
           meshregion[i] = region[i];
}
   
void svMesh::SetData(char *file, int index)
{ 
//   cerr<<meshregion.size()<<" "<<index<<" ";

   ifstream infile(file);
   if(infile.is_open())
   {
    int n1, n2;
    infile>>n1;//>>n2;
//    cerr<<n1<<endl;
    for(int i=0;i<n1;i++)
    {
         svVector3 p[3];
         infile>>p[0][0]>>p[0][1]>>p[0][2];
         infile>>p[1][0]>>p[1][1]>>p[1][2];
         infile>>p[2][0]>>p[2][1]>>p[2][2];

         svVector3 n[3];
         n[0] = normalize(cross(p[1]-p[0], p[2]-p[0]));
         n[1] = normalize(cross(p[2]-p[1], p[0]-p[1]));
         n[2] = normalize(cross(p[0]-p[2], p[1]-p[2]));
      
         mesh[index].add(p[0]);mesh[index].add(p[1]);mesh[index].add(p[2]);
         dir[index].add(n[0]);dir[index].add(n[1]);dir[index].add(n[2]);
    }

  }
  infile.close();
}

void svMesh::SetColors()
{
     svColors *color = new svColors();
     for(int i=0;i<meshregion.size();i++)
     {
         for(int j=0;j<mesh[i].size();j++)
         {
             colors[i].add(color->Get8Colors(meshregion[i]));
         }
     }
     delete color;
}

void svMesh::GenerateWireframe(int region)
{
    if(glIsList(display_list))
                glDeleteLists(display_list, 1);
    glNewList(display_list, GL_COMPILE);

   glDisable(GL_LIGHTING);
   int index;
   for(int i=0;i<meshregion.size();i++)
   {
         if(region == meshregion[i])
         {
              index = i;break;
         }
   }
   glColor3f(166./255., 206./255., 227./255.); 
   for(int i=0;i<mesh[index].size()-2;)
   {
      glBegin(GL_LINE_LOOP);
      glVertex3f( mesh[index][i][0],mesh[index][i][1],mesh[index][i][2]);
      glVertex3f( mesh[index][i+1][0],mesh[index][i+1][1],mesh[index][i+1][2]);
      glVertex3f( mesh[index][i+2][0],mesh[index][i+2][1],mesh[index][i+2][2]);
      glEnd();
      i+=3;
   }
   glEnable(GL_LIGHTING);

   glEndList();
}

void svMesh::GenerateWireframe()
{
    if(glIsList(display_list))
                glDeleteLists(display_list, 1);
    glNewList(display_list, GL_COMPILE);

   glDisable(GL_LIGHTING);
   glColor3f(166./255., 206./255., 227./255.);

   for(int j=0;j< meshregion.size()-1;j++)
   {
   for(int i=0;i<mesh[j].size()-2;)
   {
      int index = j;
      glBegin(GL_LINE_LOOP);
      glVertex3f( mesh[index][i][0],mesh[index][i][1],mesh[index][i][2]);
      glVertex3f( mesh[index][i+1][0],mesh[index][i+1][1],mesh[index][i+1][2]);
      glVertex3f( mesh[index][i+2][0],mesh[index][i+2][1],mesh[index][i+2][2]);
      glEnd();
      i+=3;
   }
   }
   glEnable(GL_LIGHTING);

   glEndList();
}


void svMesh::GenerateSurface(int region)
{
    if(glIsList(display_list))
                glDeleteLists(display_list, 1);
    glNewList(display_list, GL_COMPILE);
 
  int index;
   for(int i=0;i<meshregion.size();i++)
   {
         if(region == meshregion[i])
         {
              index = i;break;
         }
   }

   glColor3f(166./255., 206./255., 227./255.);

      glBegin(GL_TRIANGLES);

   for(int i=0;i<mesh[index].size()-2;)
   {
      glNormal3f( dir[index][i][0],dir[index][i][1],dir[index][i][2]);
      glVertex3f( mesh[index][i][0],mesh[index][i][1],mesh[index][i][2]);
      glNormal3f( dir[index][i+1][0],dir[index][i+1][1],dir[index][i+1][2]);
      glVertex3f( mesh[index][i+1][0],mesh[index][i+1][1],mesh[index][i+1][2]);
      glNormal3f( dir[index][i+2][0],dir[index][i+2][1],dir[index][i+2][2]);
      glVertex3f( mesh[index][i+2][0],mesh[index][i+2][1],mesh[index][i+2][2]);
      i+=3;
   }
      glEnd();
  
   glEndList();
}

void svMesh::GenerateSurfaces(int index)
{
    if(glIsList(display_list))
                glDeleteLists(display_list, 1);
    glNewList(display_list, GL_COMPILE);

   glColor3f(166./255., 206./255., 227./255.);
      glBegin(GL_TRIANGLES);
  for(int j=0;j<meshregion.size()-index;j++)
  {
   for(int i=0;i<mesh[j].size()-2;)
   {
     int index = j;
      glNormal3f( dir[index][i][0],dir[index][i][1],dir[index][i][2]);
      glVertex3f( mesh[index][i][0],mesh[index][i][1],mesh[index][i][2]);
      glNormal3f( dir[index][i+1][0],dir[index][i+1][1],dir[index][i+1][2]);
      glVertex3f( mesh[index][i+1][0],mesh[index][i+1][1],mesh[index][i+1][2]);
      glNormal3f( dir[index][i+2][0],dir[index][i+2][1],dir[index][i+2][2]);
      glVertex3f( mesh[index][i+2][0],mesh[index][i+2][1],mesh[index][i+2][2]);
      i+=3;
   }
 }   
   glEnd();

   glEndList();
}



void svMesh::GenerateWireframe(char *file)
{
    if(glIsList(display_list))
                glDeleteLists(display_list, 1);
    glNewList(display_list, GL_COMPILE);

    glDisable(GL_LIGHTING);

    ifstream infile(file);

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
    }
    infile.close();
    }
    glEndList();
}


void svMesh::GenerateWiresolid(char *file)
{
        if(glIsList(display_list))
                glDeleteLists(display_list, 1);
    glNewList(display_list, GL_COMPILE);
    ifstream infile(file);
    int n1, n2;
    infile>>n1;
    for(int i=0;i<n1;i++)
    {
          svVector3 p[3];
         infile>>p[0][0]>>p[0][1]>>p[0][2];
         infile>>p[1][0]>>p[1][1]>>p[1][2];
         infile>>p[2][0]>>p[2][1]>>p[2][2];

         svVector3 n[3];
         n[0] = normalize(cross(p[1]-p[0], p[2]-p[0]));
         n[1] = normalize(cross(p[2]-p[1], p[0]-p[1]));
         n[2] = normalize(cross(p[0]-p[2], p[1]-p[2]));

         glColor3f(190/255., 186./255., 218./255.);//76./255.,28./266.,95./255.);
         glBegin(GL_TRIANGLES);
         for(int j=0;j<3;j++)
         {
                glNormal3f(n[j][0],n[j][1],n[j][2]);
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

void svMesh::GenerateSurfacesByColor()
{
    if(glIsList(display_list))
                glDeleteLists(display_list, 1);
    glNewList(display_list, GL_COMPILE);

      glBegin(GL_TRIANGLES);
  for(int j=0;j<meshregion.size();j++)
  {
   
   for(int i=0;i<mesh[j].size()-2;)
   {
     int index = j;
      glColor3f( colors[index][i][0],colors[index][i][1],colors[index][i][2]);
      glNormal3f( dir[index][i][0],dir[index][i][1],dir[index][i][2]);
      glVertex3f( mesh[index][i][0],mesh[index][i][1],mesh[index][i][2]);
      glNormal3f( dir[index][i+1][0],dir[index][i+1][1],dir[index][i+1][2]);
      glVertex3f( mesh[index][i+1][0],mesh[index][i+1][1],mesh[index][i+1][2]);
      glNormal3f( dir[index][i+2][0],dir[index][i+2][1],dir[index][i+2][2]);
      glVertex3f( mesh[index][i+2][0],mesh[index][i+2][1],mesh[index][i+2][2]);
      i+=3;
   }
 }   
   glEnd();

   glEndList();
}
 
void svMesh::cleanup()
{
    for(int i=0;i<meshregion.size();i++)  
        mesh[i].free();
    delete [] mesh;

     for(int i=0;i<meshregion.size();i++)
          dir[i].free();
    delete []dir;
    meshregion.clear();
     
     for(int i=0;i<meshregion.size();i++)
         colors[i].free();
    delete []colors;
}

}


