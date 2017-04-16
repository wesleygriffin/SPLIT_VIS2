#include "dual_depth_peeling.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <assert.h>
#include <iomanip> 
#include <string>

#include "svQDOT.h"
#include "svDirectArrow.h"
#include "svBendBar.h"
#include "svDirectCylinder.h"
#include "svSplitVectorsCylinder.h"
#include "ivTrackball.h"
#include "ivview.h"
#include "MGL.h"
#include <string.h>
#include "svPickingray.h"

#define FPS 1
#include <sstream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define CLUSTER_DIMENSION 7
#define LEFT_DOWN 1
#define LEFT_UP 2

#define BEND 0
#define DIRECT 1
#define SPLITVECTORS 2

using namespace std;
using namespace __svl_lib;

#ifndef M_PI 
#define M_PI 3.1415926
#endif

#define IMAGE_WIDTH 1900 
#define IMAGE_HEIGHT 1024

view3d view_info;

svBendBar *bendglyph[2];
svDirectCylinder *directglyph[2];
svPeeling *peeling[2];
svSplitVectorsCylinder *splitglyph[2];
svPickingray *picking;

GLfloat invert_tb[16];

char *configFile;

int movecount;

int taskindex;
int dataindex;

typedef struct taskproperty{
   char *dir;
} taskproperty;


struct ConfigProperty{
   
    taskproperty task[8];
	
    svVector3 plane_center;
    svVector3 plane_vector;
    svScalar plane_distance;
	
        RenderProperty  renderproperty;
        

        BendProperty bendproperty;
        BarProperty barproperty;   
        CylinderProperty cylinderproperty;
     
        DirectProperty directproperty;
        SplitVectorsProperty splitproperty;

        svVector4 barcolor;
        svInt frequency;

//        svIntArray pointindex[2];
 
        ViewProperty viewproperty;	
} configproperty;


void Config(char *configfname, ConfigProperty &property);
void Update();

GLboolean bBoundingBox=true;
GLboolean bAxis;

GLuint image_width;
GLuint image_height;

GLfloat m[16];

Trackball trackball;

float sec;
string timestring;
bool startclock;
int pageindex;
float timethreshold;

int *pagestatus;//[7*17]

typedef struct StudyOrder{
   int taskid;
   int dataid;
   int encodeid;
} StudyOrder;

StudyOrder *studyorder;//[7*8];

int pid;
int orderid;

svIntArray task1pointindex[2];
svIntArray task2pointindex[2];
svIntArray task4pointindex[2];
svIntArray task5pointindex[2];
svIntArray task7pointindex;

string inputvalue;
int selectedbutton;
vector<svVector3> task6vector;
//vector<svVector3> task7dot;

typedef struct TaskRecord{
    float panswer;
 //task1, task2, task3, task4, task 5 ,task 6

    int pindex;
	//task3

    vector<svVector3> panswerlist;
    char *imagedir;
 //task7

    vector<int> pindexlist;//task8

    svScalar time;
    int confidence;

    int type; //tasktype
}TaskRecord;

TaskRecord taskrecord;

int mousex;
int mousey;
int mousestate;


ofstream outfile;

void LoadStudyOrder()
{
     char *str = new char[400];
     sprintf(str, "order%d", pid);
     ifstream infile(str);
     int t1, t2, t3;

     vector<StudyOrder> s;
     vector<int> count;
     int n;
     infile>>n;

     for(int i=0;i<n;i++)
     {
          StudyOrder tmp;

          int m;
          infile>>m;
          count.push_back(m);
          for(int j=0;j<m;j++)
          {
              infile>>t1>>t2>>t3;
              tmp.taskid = t1;
              tmp.dataid = t2;
              tmp.encodeid = t3;

              s.push_back(tmp);
          }
     }

     studyorder = new StudyOrder[s.size()];
     pagestatus = new int[s.size()*2 + n];

     for(int i=0;i<s.size();i++)
     {
         studyorder[i] = s[i];
     }
     s.clear();

     int ii=0;
     for(int i=0;i<n;i++)
     {
         pagestatus[ii] =0;
         for(int j=0;j<count[i].size();j++) 
         {
              ii++;
              pagestatus[ii] = 1;
              ii++;
              pagestatus[ii] = 2;
         } 
         ii++;
     }

     count.clear();
/*
     while(infile>>t1>>t2>>t3)
     {
         studyorder[count].taskid = t1;
         studyorder[count].dataid = t2;
         studyorder[count].encodeid = t3;
         count++;
     }
*/
     infile.close();
     delete [] str;
}

void LoadTaskSelectedPoints()
{
     char *str = new char[400];
     sprintf(str, "%s/selectedpoints", configproperty.task[0].dir);
     ifstream infile(str);
     int t1, t2;
     while(infile>>t1>>t2)
     {
         task1pointindex[0].add(t1);
         task1pointindex[1].add(t2);
     }
     infile.close();

     sprintf(str, "%s/selectedpoints", configproperty.task[1].dir);
     infile.open(str);
     int t1, t2;
     while(infile>>t1>>t2)
     {
         task2pointindex[0].add(t1);
         task2pointindex[1].add(t2);
     }
     infile.close();

}

void LoadTaskOrder()
{
     char *str = new char[400];

     sprintf(str, "%s/dataorder", configproperty.task[3].dir);
     ifstream infile(str);
     int t1, t2;
     while(infile>>t1>>t2)
     {
         task4pointindex[0].add(t1);
         task4pointindex[1].add(t2);
     }
     infile.close();

     sprintf(str, "%s/selectedpoints", configproperty.task[4].dir);
     infile.open(str);
     int t1, t2;
     while(infile>>t1>>t2)
     {
         task5pointindex[0].add(t1);
         task5pointindex[1].add(t2);
     }

     delete [] str;
}
/*
void LoadTaskCenter()
{
     char *str = new char[400];

     sprintf(str, "%s/selectedpoints", configproperty.task[6].dir);
     ifstream infile(str);
     int n;
     infile>>n;
     for(int i=0;i<n;i++)
     {
         int ii;
         infile>>ii;
         task7pointindex.add(ii);
     }
     infile.close();

     delete [] str;
}
*/
void LoadTaskVector()
{
     char *str = new char[400];

     sprintf(str, "%s/vector", configproperty.task[5].dir);
     ifstream infile(str);
     int n;
     infile>>n;
     for(int i=0;i<n;i++)
     {
         svVector3 p;
         infile>>p[0]>>p[1]>>p[2];
         task6vector.add(p);
     }
     infile.close();

     delete [] str;

}

void LoadData();

void StringtoDouble()
{
  if(studyorder[dataindex].taskid < 3)
  {
      taskrecord.panswer = (float) (atof(inputvalue.c_str()));
   }   
}

void logInit()
{
     char *str = new char[400];
     sprintf(str, "log%d", pid);

     outfile.open(str);
     delete [] str;
}

void recordInteraction()
{

}

void recordTime()
{
  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
   outfile<<"page "<<pagestatus[pageindex]<<" "<<asctime (timeinfo);
}

void recordTask()
{
   if(pagestatus[pageindex] == 0)
   {
     if(dataindex == -1)
      outfile<<"task 1"<<endl;
    else
      outfile<<"task "<<studyorder[dataindex].taskid+1<<endl;
   }

}
/*
void recordTruth()
{
   outfile<<"data "<<studyorder[dataindex].dataid<<endl;
   if(studyorder[dataindex].taskid == 0||studyorder[dataindex].taskid == 1
     )
   {
        outfile<<"truth "<<task1record.truth
            <<" value1 "<<task1record.ctruth1<<" value2 "
           <<task1record.ctruth2
           <<endl;
   }
   else if(studyorder[dataindex].taskid == 2
     )
   {
       outfile<<"truth "<<task3record.truth
             <<endl;
   }
}
*/


void SaveoImage()
{
     char *file = new char[400];

     sprintf(file, "%d.ppm", studyorder[dataindex].dataid);

        int w = image_width;
        int h = image_height;

  char *image_buf = new char [w*h*3];

  glPixelStorei(GL_PACK_ALIGNMENT, 1);

  glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, image_buf);


  FILE* fp;

  if (NULL != (fp = fopen(file, "wb"))){

    fprintf(fp, "P6 %d %d 255\n", w, h);

    for (int i=h-1; i >= 0; --i)

    {
       fwrite(image_buf+3*i*w, sizeof(unsigned char), 3*w, fp);

    }

    fclose(fp);

  }

  delete [] image_buf;
  delete [] file;

}
void recordAnswer()
{   
   if(studyorder[dataindex].taskid == 0||studyorder[dataindex].taskid == 1)
   {
         StringtoDouble();

        outfile<<"answer "<<
           taskrecord.panswer<<" confidence "
           <<taskrecord.confidence<<endl;
   }
   else if(studyorder[dataindex].taskid == 2)
  {
        StringtoDouble();

        /*if(studyorder[dataindex].encodeid ==BEND)
           if(task3selectedindex>-1)
                task3record.pindex = bendglyph->GetMag(0, task3selectedindex);
        else if(studyorder[dataindex].encodeid ==DIRECT)
           if(task3selectedindex>-1)
                task3record.panswer =directglyph->GetMag(0, task3selectedindex);
        else if(studyorder[dataindex].encodeid ==SPLITVECTORS)
           if(task3selectedindex>-1)
                task3record.panswer = splitglyph->GetMag(0, task3selectedindex);
        */
          outfile<<"answer "<<taskrecord.panswer<<" "<<taskrecord.pindex<<" confidence"
               <<taskrecord.confidence<<endl;
  }
  else if(studyorder[dataindex].taskid <6)
  {
        outfile<<"answer "<<
           taskrecord.pindex<<" confidence "
           <<taskrecord.confidence<<endl;
  }
  else if(studyorder[dataindex].taskid == 6)
  {
       outfile<<"answer "<<taskrecord.panswerlist.size()<<" ";
       for(int i=0;i<taskrecord.panswerlist.size();i++)
       {
         outfile<<taskrecord.panswerlist[i]<<" ";
       }
       outfile<<"confidence"<<taskrecord.confidence<<endl;
//       SavetoImage();
  }
  else if(studyorder[dataindex].taskid == 7)
  {
       outfile<<"answer "<<taskrecord.pindexlist.size()<<" ";
       for(int i=0;i<taskrecord.pindexlist.size();i++)
       {
         outfile<<taskrecord.pindexlist[i]<<" ";
       }
       outfile<<"confidence"<<taskrecord.confidence<<endl;
  }
}

void logEnd()
{
     recordTime();
     recordAnswer();
     recordTime();
     outfile.close();
}


//====================================================================
void InitLight()
{
  //setting of lighting
  GLfloat mat_diffuse[] = { 0.8, .0, 0.0, .4};
  GLfloat mat_specular[] = { 1, 1, 1, 1 };
  GLfloat mat_shininess[] = { 2.0 };//20
  //GLfloat light_position[] = { 24, 24, 60, 0.0 };
  GLfloat light_position[] = { 0,0,50, 0.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
 // glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
}

//***************************
// GLUT callback functions
//****************************

void reshape(int w, int h)
{
   svVector3 UP;
   UP[0]=0;UP[1]=1;UP[2]=0;

    peeling[0]->Reshape(w,h);
    peeling[0]-> DeleteDualPeelingRenderTargets();
    peeling[0]-> InitDualPeelingRenderTargets();

        image_width  = w;
        image_height = h;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport (0, 0, w, h);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
// perspective
                float aspect = view_info.aspect_ratio * float(w)/float(h);
                float GLfloat_fov;
                if ( aspect < 1 ){
                  // fovy is a misnomer.. we GLfloatly mean the fov applies to the
                  // smaller dimension
                  float fovx, fovy;
                  fovx = fovy = view_info.view_angle;
                  GLfloat_fov = svToDegree(2*atan(tan(svToRadian(fovx/2))/aspect));
                }
                else{
                        GLfloat_fov = view_info.view_angle;
                }

                gluPerspective(GLfloat_fov,
                                           aspect,
                                           0.1,//view_info.hither,
                                           view_info.yon);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(view_info.eye[0],view_info.eye[1],view_info.eye[2],
                                view_info.coi[0],view_info.coi[1],view_info.coi[2],
                                0, 1, 0);

        
        InitLight();
    
//    viewproperty.eye = view_info.eye;
  trackball.setWindowSize(w,h);
  trackball.getMatrix().getValue(viewproperty.tm);

    glGetDoublev (GL_MODELVIEW_MATRIX, viewproperty.mvmatrix); 
    glGetDoublev (GL_PROJECTION_MATRIX, viewproperty.projmatrix);
    glGetIntegerv( GL_VIEWPORT, viewproperty.viewport );
//for(int ii=0;ii<4;ii++)
//cerr<<viewproperty.viewport[ii]<<" ";
 // bendglyph->Generate(configproperty.bendproperty,viewproperty,
   //             configproperty.plane_vector);

   picking[0]->initCamera(view_info.eye,view_info.coi,UP,
                      GLfloat_fov, w, h, 0.1);
}

void multiplereshape(int x, int y, int ratio, int index)
{
   svVector3 UP;
   UP[0]=0;UP[1]=1;UP[2]=0;

	int w = image_width/ratio;
	int h = image_height/ratio;
  
       if(index>-1)
       {
    	peeling[index]->Reshape(w,h);
    	peeling[index]-> DeleteDualPeelingRenderTargets();
    	peeling[index]-> InitDualPeelingRenderTargets();
       }
   
       glViewport (x, y, w, h);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float aspect = view_info.aspect_ratio * float(w)/float(h);
        float GLfloat_fov;

         if ( aspect < 1 ){
              float fovx, fovy;
              fovx = fovy = view_info.view_angle;
              GLfloat_fov = svToDegree(2*atan(tan(svToRadian(fovx/2))/aspect));
          }
          else{
                      GLfloat_fov = view_info.view_angle;
            }

         gluPerspective(GLfloat_fov,
                                     aspect,
                                           0.1,//view_info.hither,
                                           view_info.yon);
         glMatrixMode(GL_MODELVIEW);
         glLoadIdentity();
         gluLookAt(view_info.eye[0],view_info.eye[1],view_info.eye[2],
                                view_info.coi[0],view_info.coi[1],view_info.coi[2],                        0, 1, 0);
      if(index>=1)
   	picking[index]->initCamera(view_info.eye,view_info.coi,UP,
                      GLfloat_fov, w, h, 0.1);
}

void renderTask12marker()
{
    if(glIsList(12))
                glDeleteLists(12, 1);
    glNewList(12, GL_COMPILE);

   svVector3 dir[2];
   svVector3 pos1[2], pos2[2];
   int index = studyorder[dataindex].dataid;
   int in = configproperty.pointindex[0][index];
   dir[0] = bendglyph[0]->GetVerticalDir(0, in,configproperty.plane_vector, viewproperty);
   pos1[0] = bendglyph[0]->GetGlyph(0, in);
   if(studyorder[dataindex].encodeidi==BEND)
   {
        pos2[0] = bendglyph[0]->GetEnd(configproperty.bendproperty,viewproperty,
                 configproperty.plane_vector,0,in);
   }
   else if(studyorder[dataindex].encodeidi==LINEAR)
   {
        pos2[0] = directglyph[0]->GetEnd(0,in);
   }
   else if(studyorder[dataindex].encodeid==SPLITVECTORS)
   {
	pos2[0] = splitglyph[0]->GetEnd(0,in);
   }

   in = configproperty.pointindex[1][index];
   dir[1] = bendglyph[0]->GetVerticalDir(0, in,configproperty.plane_vector, viewproperty);
   pos1[1] = bendglyph[0]->GetGlyph(0, in);
   if(studyorder[dataindex].encodeid==BEND)
   {
        pos2[1] = bendglyph[0]->GetEnd(configproperty.bendproperty,viewproperty,
                 configproperty.plane_vector,0,in);
   }
   else if(studyorder[dataindex].encodeid==LINEAR)
   {
        pos2[1] = directglyph[0]->GetEnd(0,in);
   }
   else if(studyorder[dataindex].encodeid==SPLITVECTORS)
   {
        pos2[1] = splitglyph[0]->GetEnd(0,in);
   }

  for(int i=0;i<2;i++)
  {
   double angle_x = acos(dir[i][2]);
   if(dir[i][1] > 0)
   {
        angle_x = - angle_x;
   }
   double xy_project = dir[i][0] * dir[i][0] + dir[i][1] * dir[i][1];
   xy_project = sqrt(xy_project);
   double angle_z = acos(dir[i][1]/xy_project);
   if(angle_x < 0)
   {
        if (dir[i][0] > 0)
        {
           angle_z =  -angle_z;
         }
   }
   else
   {
        angle_z = M_PI - angle_z;
        if(dir[i][0] < 0)
                angle_z =  -angle_z;
   }
    glEnable(GL_LIGHTING);
    glPushMatrix();
    glColor3f(1,1,0);
    float scale=0.05;
    glTranslatef(pos1[i][0],pos1[i][1],pos1[i][2]);
    glRotatef(angle_z/M_PI * 180.,0,0,1);
    glRotatef(angle_x/M_PI * 180.,1,0,0);
    glScalef(scale, scale, scale);
    glTranslatef(0,0,-4);
    glutSolidCone(1, 4, 8, 2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(pos2[i][0],pos2[i][1],pos2[i][2]);
    glRotatef(angle_z/M_PI * 180.,0,0,1);
    glRotatef(angle_x/M_PI * 180.,1,0,0);
    glScalef(scale, scale, scale);
    glTranslatef(0,0,-4);
    glutSolidCone(1, 4, 8, 2);
    glPopMatrix();

  }
    glEndList();
}

void renderTask12letter()
{
   svVector3 pos1[2], pos2[2];
   svScalar mag[2];
   int index = studyorder[dataindex].dataid;
   int in = configproperty.pointindex[0][index];
   pos1[0] = bendglyph[0]->GetGlyph(0, in);
   mag[0] = bendglyph[0]->GetMag(0,in);

   in = configproperty.pointindex[1][index];
   pos1[1] = bendglyph[0]->GetGlyph(0, in);
   mag[1] = bendglyph[0]->GetMag(0,in);

  for(int i=0;i<2;i++)
  {
   glDisable(GL_LIGHT0);
   glDisable(GL_LIGHTING);
        glRasterPos3f(pos1[i][0],pos1[i][1],pos1[i][2]);
       if(mag[i] > mag[1-i])
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'A');
      else
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'B');
  }
}

//A and B are ordered randomly
void renderTask4letter()
{
   svVector3 pos1[2];
   //svScalar mag[2];
   //int index = studyorder[dataindex].dataid;
   //int in = configproperty.pointindex[0][index];
   pos1[0] = bendglyph[0]->GetCenter(task4pointindex[0]);

   //in = configproperty.pointindex[1][index];
   pos1[1] = bendglyph[0]->GetCenter(task4pointindex[1]);

  for(int i=0;i<2;i++)
  {
      glDisable(GL_LIGHT0);
      glDisable(GL_LIGHTING);
      glRasterPos3f(pos1[i][0],pos1[i][1],pos1[i][2]);
      if(i==0)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'A');
      else
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'B');
  }
}

void renderTask5letter(int i)
{
   svVector3 pos;
   pos = bendglyph[i]->GetCenter(0);

      glDisable(GL_LIGHT0);
      glDisable(GL_LIGHTING);
      glRasterPos3f(pos[0],pos[1],pos[2]);
      if(i==0)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'A');
      else
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'B');
}

void renderTask8letter()
{
  char str[5] ={'A', 'B', 'C', 'D', 'E'};
  
  for(int j=0;j<5;j++)
  {
         glRasterPos2f(image_width/10 * i * 10, image_height/10);
         glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
  }
}

int legend = 0;

void renderTask3Marker()
{
  if(taskrecord.pindex>=0)
  {
    if(glIsList(12))
           glDeleteLists(12, 1);
    glNewList(12, GL_COMPILE);

   svVector3 dir;
   svVector3 pos1, pos2;
   int in = task3selectedindex;
   dir = bendglyph[0]->GetVerticalDir(0, in,configproperty.plane_vector, viewproperty);
   pos1 = bendglyph[0]->GetGlyph(0, in);
   if(studyorder[dataindex].encodeid==BEND)
   {
        pos2 = bendglyph[0]->GetEnd(configproperty.bendproperty,viewproperty,
                 configproperty.plane_vector,0,in);
   }
   else if(studyorder[dataindex].encodeid==LINEAR)
   {
        pos2 = directglyph[0]->GetEnd(
                 0,in);
   }
   else if(studyorder[dataindex].encodeid==SPLITVECTORS)
   {
        pos2 = splitglyph[0]->GetEnd(
                 0,in);
   }

   double angle_x = acos(dir[2]);
   if(dir[1] > 0)
                {
                        angle_x = - angle_x;
                }
                double xy_project = dir[0] * dir[0] + dir[1] * dir[1];
                xy_project = sqrt(xy_project);
                double angle_z = acos(dir[1]/xy_project);
                if(angle_x < 0)
                {
                                if (dir[0] > 0)
                                 {
                                        angle_z =  -angle_z;
                                  }
                }
                else
                {
                                angle_z = M_PI - angle_z;
                                if(dir[0] < 0)
                                        angle_z =  -angle_z;
                }
    glEnable(GL_LIGHTING);
    glPushMatrix();
    glColor3f(0,1,1);
    float scale=0.05;
    glTranslatef(pos1[0],pos1[1],pos1[2]);
    glRotatef(angle_z/M_PI * 180.,0,0,1);
    glRotatef(angle_x/M_PI * 180.,1,0,0);
    glScalef(scale, scale, scale);
    glTranslatef(0,0,-4);
    glutSolidCone(1, 4, 8, 2);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0,1,1);
    glTranslatef(pos2[0],pos2[1],pos2[2]);
    glRotatef(angle_z/M_PI * 180.,0,0,1);
    glRotatef(angle_x/M_PI * 180.,1,0,0);
    glScalef(scale, scale, scale);
    glTranslatef(0,0,-4);
    glutSolidCone(1, 4, 8, 2);
    glPopMatrix();

    glEndList();
 }
}

void renderTask6vector()
{
    int index = studyorder[dataindex].dataid;

    if(glIsList(12))
                glDeleteLists(12, 1);
    glNewList(12, GL_COMPILE);

       svVector3 dir = task6vector[index];

   double angle_x = acos(dir[2]);
   if(dir[1] > 0)
                {
                        angle_x = - angle_x;
                }
                double xy_project = dir[0] * dir[0] + dir[1] * dir[1];
                xy_project = sqrt(xy_project);
                double angle_z = acos(dir[1]/xy_project);
                if(angle_x < 0)
                {
                                if (dir[0] > 0)
                                 {
                                        angle_z =  -angle_z;
                                  }
                }
                else
                {
                                angle_z = M_PI - angle_z;
                                if(dir[0] < 0)
                                        angle_z =  -angle_z;
                }

     

    glPushMatrix();
    glColor3f(0,1,1);
    glRotatef(angle_z/M_PI * 180.,0,0,1);
    glRotatef(angle_x/M_PI * 180.,1,0,0);
    glScalef(scale, scale, scale);
    glTranslatef(0,0,8);
    glutSolidCone(1, 4, 8, 2);
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glPushMatrix();
    glColor3f(0,1,1);
    glRotatef(angle_z/M_PI * 180.,0,0,1);
    glRotatef(angle_x/M_PI * 180.,1,0,0);
    glScalef(scale, scale, scale);
    
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,0,8);
    glEnd();
    glPopMatrix();

    glEndList();
}

void renderTask7drawing()
{
    if(glIsList(12))
                glDeleteLists(13, 1);
    glNewList(12, GL_COMPILE);

    glColor3f(0,1,1);
    for(int i=0;i<taskrecord.panswerlist.size();i++)
    {
         glPushMatrix();
         glTranslatef(taskrecord.panswerlist[i][0], taskrecord.panswerlist[i][1],taskrecord.panswerlist[i][2]);
         glScalef(scale, scale, scale);
         glutSolidSphere(1,5,5);
         glPopMatrix();
    }

    glDisable(GL_LIGHTING);

    glBegin(GL_LINE_STRIP);
    for(int i=0;i<taskrecord.panswerlist.size();i++)
    {
	glVertex3f(taskrecord.panswerlist[i][0], taskrecord.panswerlist[i][1],taskrecord.panswerlist[i][2]);
    }
    glEnd();

    glEndList();
}
//5 conditions
void renderTask8legend()
{
    if(glIsList(13))
         glDeleteLists(13, 1);
    glNewList(13, GL_COMPILE);

    glColor3f(0.5,0.5,0.5);

    svVector3 pos[5][4];

    pos[0][0][0]=-4; pos[0][0][1] = 0;
    pos[0][1][0]=-8; pos[0][1][1] = 0;
    pos[0][2][0]=4;  pos[0][2][1] = 0;
    pos[0][3][0]=8;  pos[0][3][1] = 0;

    pos[1][0][0]=-8; pos[1][0][1] = 2;
    pos[1][1][0]=-8; pos[1][1][1] = -2;
    pos[1][2][0]=8;  pos[1][2][1] = 2;
    pos[1][3][0]=8;  pos[1][3][1] = -2;

    pos[2][0][0]=-4; pos[2][0][1] = 2;
    pos[2][1][0]=-4; pos[2][1][1] = -2;
    pos[2][2][0]=4;  pos[2][2][1] = 2;
    pos[2][3][0]=4;  pos[2][3][1] = -2;

    pos[3][0][0]=-6; pos[3][0][1] = 2;
    pos[3][1][0]=-6; pos[3][1][1] = -2;
    pos[3][2][0]=6;  pos[3][2][1] = 2;
    pos[3][3][0]=6;  pos[3][3][1] = -2;

    pos[2][0][0]=-4; pos[2][0][1] = 0;
    pos[2][1][0]=-8; pos[2][1][1] = 0;
    pos[2][2][0]=4;  pos[2][2][1] = 0;
    pos[2][3][0]=8;  pos[2][3][1] = 0;

    for(int i=0;i<5;i++)
    {
      for(int j=0;j<4;j++)
      {
         pos[i][j][2] = 0;
      }
    }

    svVector3 dir[5][2];
    dir[0][0][0]=0;  dir[0][0][1]=1;
    dir[0][1][0]=0;  dir[0][1][1]=1;
    dir[1][0][0]=1;  dir[1][0][1]=0;
    dir[1][1][0]=-1; dir[1][1][1]=0;
    dir[2][0][0]=-1; dir[2][0][1]=0;
    dir[2][1][0]=1;  dir[2][1][1]=0;
    dir[3][0][0]=1;  dir[3][0][1]=0;
    dir[3][1][0]=1;  dir[3][1][1]=0;
    dir[4][0][0]=0;  dir[4][0][1]=1;
    dir[4][1][0]=0;  dir[4][1][1]=-1;

    for(int i=0;i<5;i++)
    {
      for(int j=0;j<2;j++)
      {
         dir[i][j][2] = 0;
      }
    }

    svVector3 r[5][2];
    r[0][0][0]=1;r[0][0][1]=0;r[0][0][2]=-90;
    r[0][1][0]=1;r[0][1][1]=0;r[0][1][2]=-90;
    r[1][0][0]=0;r[1][0][1]=1;r[1][0][2]=90;
    r[1][1][0]=0;r[1][1][1]=1;r[1][1][2]=-90;
    r[2][0][0]=0;r[2][0][1]=1;r[2][0][2]=-90;
    r[2][1][0]=0;r[2][1][1]=1;r[2][1][2]=90;
    r[3][0][0]=0;r[3][0][1]=1;r[3][0][2]=90;
    r[3][1][0]=0;r[3][1][1]=1;r[3][1][2]=90;
    r[4][0][0]=1;r[4][0][1]=0;r[4][0][2]=-90;
    r[4][1][0]=1;r[4][1][1]=0;r[4][1][2]=90;

    svScalar l =4;

    for(int i=0;i<5;i++)
    {
       for(int j=0;j<4;j++)
       {
         svVector3 end;
         end = pos[i][j] + dir[i][j] * l;
	 glPushMatrix();
         glScalef(scale,scale,scale);
         glBegin(GL_LINES);
         glVertex3f(pos[i][j][0], pos[i][j][1],pos[i][j][2]);
         glVertex3f(end[0], end[1], end[2]);
         glEnd();
         glPopMatrix();

         glPushMatrix(); 
         glTranslatef(end[0], end[1], end[2]);
         glRotate(r[i][j/2][2], r[i][j/2][0], r[i][j/2][1]);
         glScalef(scale, scale, scale);
         glutSolidCone(1, 4, 8, 2); 
         glPopMatrix();
       }
    }

    glEndList();
}

void renderTaskTitle()
{
     glColor3f(0,0,0);
     char *str = new char[400];
     if(pagestatus[pageindex] == 0)
     {
          if(studyorder[dataindex+1].taskid == 0)
          {
              sprintf(str, "Task1. What is the ratio between magnitudes at the given points A and B (10 seconds)?");
          }
          else if(studyorder[dataindex+1].taskid == 1)
          {
              sprintf(str, "Task2. What is the ratio between magnitudes at the given points A and B (40 seconds)?");
          }
          else if(studyorder[dataindex+1].taskid == 2)
          {
              sprintf(str, "Task 3. For the highlighted region, mark the location and value of the largest magnitude? (30 seconds).");
          }
          else if(studyorder[dataindex+1].taskid == 3)
          {
              sprintf(str, "Task 4. Given two highlighted regions A and B, which one has higher average magnitudes?");
          }
          else if(studyorder[dataindex+1].taskid == 4)
          {
              sprintf(str, "Task 5. Given two highlighted regions A and B, which one has higher average magnitudes?");
          }
          else if(studyorder[dataindex+1].taskid == 5)
          {
              sprintf(str, "Task 6. What is the cluster where the vectors are parallel to the given vector in red?");
          }
          else if(studyorder[dataindex+1].taskid == 6)
          {
              sprintf(str, "Task 7.  For the hicghlighted region, complete the region by marking the jointed areas with similar magnitude and orientation.");
          }
          else if(studyorder[dataindex+1].taskid == 7)
          {
              sprintf(str, "Task 8. Select all the symmetrical structures.");
          }

             glRasterPos2f(image_width/3, image_height/2); 
             for(int i=0;i<strlen(str);i++)
                     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
     }
     else if(pagestatus[pageindex] == 1)
     {
          if(studyorder[dataindex].taskid == 0)
          {
              sprintf(str, "Task1. What is the ratio between magnitudes at the given points A and B (10 seconds)?");
          }
          else if(studyorder[dataindex].taskid == 1)
          {
              sprintf(str, "Task2. What is the ratio between magnitudes at the given points A and B (40 seconds)?");
          }
          else if(studyorder[dataindex].taskid == 2)
          {
              sprintf(str, "Task 3. For the highlighted region, mark the location and value of the largest magnitude? (30 seconds).");
          }
          else if(studyorder[dataindex].taskid == 3)
          {
              sprintf(str, "Task 4. Given two highlighted regions A and B, which one has higher average magnitudes?");
          }
          else if(studyorder[dataindex+1].taskid == 4)
          {
              sprintf(str, "Task 5. Given two highlighted regions A and B, which one has higher average magnitudes?");
          }
          else if(studyorder[dataindex+1].taskid == 5)
          {
              sprintf(str, "Task 6. What is the cluster where the vectors are parallel to the given vector in red?");
          }
          else if(studyorder[dataindex+1].taskid == 6)
          {
              sprintf(str, "Task 7.  For the hicghlighted region, complete the region by marking the jointed areas with similar magnitude and orientation.");
          }
          else if(studyorder[dataindex+1].taskid == 7)
          {
              sprintf(str, "Task 8. Select all the symmetrical structures.");
          }

             glRasterPos2f(40, 10);
             for(int i=0;i<strlen(str);i++)
                     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
     }

     delete [] str;

}

void renderSpace()
{
     glColor3f(0.5,0.5,0.5);
     char *str = new char[400];

     sprintf(str, "Press SPACE to continue");
             glRasterPos2f(20, image_height-20);
             for(int i=0;i<strlen(str);i++)
                     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);

     delete []str;
}

void renderTime()
{
     glColor3f(0,0,0);
     if(pagestatus[pageindex] == 1
      &&(studyorder[dataindex].taskid <  3))
     {
             const char *str = timestring.c_str();
             glRasterPos2f(image_width-180, image_height-20);
             for(int i=0;i<strlen(str);i++)
                     glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
     }

}

void renderAnswerBox()
{
   int x = image_width/2;
   int y =image_height/2;
   float sizey = 25;
   float sizex = 300;

    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x,y);
    glVertex2f(x+sizex,y);
    glVertex2f(x+sizex,y+sizey);
    glVertex2f(x,y+sizey);
    glEnd(); 

     char *str = new char[400];

     sprintf(str, "Your answer: ");
             glRasterPos2f(x-150, y);
             for(int i=0;i<strlen(str);i++)
                     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);

     delete []str;

    x = x + 1;
    sizex = 10;
    for(int i=0;i<strlen(inputvalue.c_str());i++)
    {
             glRasterPos2f(x, y+1);
             glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, inputvalue[i]);
             x = x + sizex;
    }
}

void renderConfidence()
{
   int x = image_width/2;
   int y = image_height/2-100;
   float sizey = 25;
   float sizex = 25;

     char *str = new char[400];
     glColor3f(0,0,0);

     sprintf(str, "Confidence level:  ");
             glRasterPos2f(x-150, y);
             for(int i=0;i<strlen(str);i++)
                     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);

     delete []str;


  for(int i=0;i<7;i++)
  {
   
    if(mousex > x && mousex < x+ sizex
      && mousey > y && mousey<y+sizex
      && mousestate == LEFT_DOWN)
    {
         //glColor3f(1,0,0);
         confidencelevel = i+1;
    }
    else if(mousex > x && mousex < x+ sizex
      && mousey > y && mousey<y+sizex)
    {
         glColor3f(0,1,1);
    }
    else
        glColor3f(0,0,0);

     if(confidencelevel == i+1)
        glColor3f(1,0,0);

    glBegin(GL_LINE_LOOP);
    glVertex2f(x,y);
    glVertex2f(x+sizex,y);
    glVertex2f(x+sizex,y+sizey);
    glVertex2f(x,y+sizey);
    glEnd();
    //glColor3f(0,0,0);

    glRasterPos2f(x+5, y+5);
    char str[2];
    sprintf(str, "%d", i+1);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[0]);   

    x = x + sizex*1.5;
  }
}

void render2D()
{
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(0.0,(GLfloat) viewproperty.viewport[2], 0.0, (GLfloat) viewproperty.viewport[3]);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glDisable(GL_LIGHTING);

          if(pagestatus[pageindex] == 1)
          {
            glPushMatrix();
            glTranslatef(image_width/1.7,10.,0);
            glScalef(200,200,200);

            if(studyorder[dataindex].encodeid == BEND)
              bendglyph->RenderLegend(configproperty.bendproperty);

            glPopMatrix();

            if(studyorder[dataindex].taskid <3)
                   renderTime();
          }
          else if(pagestatus[pageindex] == 2)
          { 
               renderAnswerBox();
               renderConfidence();
          }
            renderSpace();
            renderTaskTitle();

                glMatrixMode(GL_PROJECTION);
                glLoadMatrixd(viewproperty.projmatrix);
                glMatrixMode(GL_MODELVIEW);
                glLoadMatrixd(viewproperty.mvmatrix);
}

void defaultdisplay()
{
       multiplereshape(0,0,1,0);
        trackball.getMatrix().getValue(m);
        trackball.getMatrix().getValue(viewproperty.tm);
        glPushMatrix();
        glMultMatrixf(m);
        glColor3f(0,0,0);

       if(pagestatus[pageindex] == 1)
         peeling[0]->RenderDualPeeling();

        if(studyorder[dataindex].taskid<2
           && pagestatus[pageindex] == 1)
        {
            renderTask12letter();
        }
        else if(studyorder[dataindex].taskid==2
           && pagestatus[pageindex] == 1)
        {
            renderTask3letter();
         }
        else if(studyorder[dataindex].taskid==3
           && pagestatus[pageindex] == 1)
        {
            renderTask4letter();
         }
        else if(studyorder[dataindex].taskid==7
           && pagestatus[pageindex] == 1)
        {
            renderTask7drawing();
        }

        glPopMatrix();
}

void task5display()
{
       multiplereshape(0,0,2,0);
       if(pagestatus[pageindex] == 1)
       {
        trackball.getMatrix().getValue(m);
        trackball.getMatrix().getValue(viewproperty.tm);
        glPushMatrix();
        glMultMatrixf(m);
        glColor3f(0,0,0);
            peeling[0]->RenderDualPeeling();
            renderTask5letter(0);
        glPopMatrix();
       }

       multiplereshape(image_width/2,0,2,1);
       if(pagestatus[pageindex] == 1)
       {
        trackball.getMatrix().getValue(m);
        trackball.getMatrix().getValue(viewproperty.tm);
        glPushMatrix();
        glMultMatrixf(m);
        glColor3f(0,0,0);
            peeling[1]->RenderDualPeeling();
            renderTask5letter(1);
        glPopMatrix();
       }
}

void task6display()
{
       multiplereshape(0,0,1,0);
        trackball.getMatrix().getValue(m);
        trackball.getMatrix().getValue(viewproperty.tm);
        glPushMatrix();
        glMultMatrixf(m);
        glColor3f(0,0,0);
       if(pagestatus[pageindex] == 1)
         peeling->RenderDualPeeling();
        glPopMatrix();

       multiplereshape(0,0,10,1);
       if(pagestatus[pageindex] == 1)
       {
        trackball.getRotateMatrix().getValue(m);
        glPushMatrix();
        glMultMatrixf(m);
        glColor3f(0,0,0);
           renderTask6vector();
        glPopMatrix();
       }
}

void task8display()
{
       multiplereshape(0,0,1,0);
               trackball.getMatrix().getValue(m);
        trackball.getMatrix().getValue(viewproperty.tm);
        glPushMatrix();
        glMultMatrixf(m);
        glColor3f(0,0,0);
       if(pagestatus[pageindex] == 1)
         peeling->RenderDualPeeling();
        glPopMatrix();

      for(int i=0;i<5;i++)
      {
        multiplereshape(image_width/10 * (i * 10) ,0,10,1);
        trackball.getRotateMatrix().getValue(m);
        glPushMatrix();
        glMultMatrixf(m);
        glColor3f(0,0,0);
        if(pagestatus[pageindex] == 1)
        {
            renderTask8legend();
         }
        glPopMatrix();
      }
}

void display(void)
{
       glEnable(GL_DEPTH_TEST);
       glEnable(GL_MULTISAMPLE);

//        GLfloat m[16];

        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
     //   trackball.getMatrix().getValue(m);
     //   trackball.getMatrix().getValue(viewproperty.tm);
     //   glPushMatrix();
     //   glMultMatrixf(m);
        glColor3f(0,0,0);

        if(!(pagestatus[pageindex]==4 || pagestatus[pageindex]==5 || pagestatus[pageindex]==7))
             defaultdisplay();
        else if(pagestatus[pageindex]==4)
             task5display();
        else if(pagestatus[pageindex]==5)
             task6display();
        else if(pagestatus[pageindex]==7)
             task8display();

     //   glPopMatrix();
        
        if(!(pagestatus[pageindex]==4 || pagestatus[pageindex]==5 || pagestatus[pageindex]==7))
       //         multiplereshape(0,0,1);
       {
             multiplereshape(0,0,1,0);
       }

        render2D();
        glutSwapBuffers();
}


void myTimer(int v)     // timer function to update time
{
   if(startclock == true && taskindex<3)
   {
    ostringstream timestream;      // used to format time string
    if (startclock) sec++;         // increment second
    timestream << setfill('0') << "" << setw(2) << timethreshold - sec;
    timestring=timestream.str();  //convert stream to string
    //cerr<<timestring<<endl;
    if (sec==timethreshold)
    {
      sec = 0;
      startclock = false;
      Update();
    }
  }
//    glutPostRedisplay();
    glutTimerFunc(1000/v, myTimer, v);  //repost timer 

    glutPostRedisplay();

}


void key(unsigned char key, int x, int y)
{
        switch (key) {
        case '\040':
             Update(); 
             break;
        case '.':
             if(pagestatus[pageindex]==2)inputvalue.append(".");break;
        case '0':
//StringtoDouble(); cerr<<task1record.panswer<<endl;
             if(pagestatus[pageindex]==2)inputvalue.append("0");break;
        case '1':
             if(pagestatus[pageindex]==2)inputvalue.append("1");break;
        case '2':
             if(pagestatus[pageindex]==2)inputvalue.append("2");break;
        case '3':
             if(pagestatus[pageindex]==2)inputvalue.append("3");break;
        case '4':
             if(pagestatus[pageindex]==2)inputvalue.append("4");break;
        case '5':
             if(pagestatus[pageindex]==2)inputvalue.append("5");break; 
        case '6':
             if(pagestatus[pageindex]==2)inputvalue.append("6");break;
        case '7':
             if(pagestatus[pageindex]==2)inputvalue.append("7");break;
        case '8':
             if(pagestatus[pageindex]==2)inputvalue.append("8");break;
        case '9':
             if(pagestatus[pageindex]==2)inputvalue.append("9");break;
        case 'e':
             if(pagestatus[pageindex]==2)inputvalue.append("e");break;
        case '\010':
             if(pagestatus[pageindex]==2)if(strlen(inputvalue.c_str())>0)inputvalue.erase(inputvalue.end()-1);
             break; 
       case '\033':
        case 'q':
        case 'Q':
                exit(0);
                break;
        }
        glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
        long s=0x00000000;
    s |= (button == GLUT_LEFT_BUTTON)   ? ((state == GLUT_DOWN) ? Trackball::LBUTTON_DOWN : Trackball::LBUTTON_UP) : 0;
    //s |= (button == GLUT_MIDDLE_BUTTON) ? ((state == GLUT_DOWN) ? Trackball::MBUTTON_DOWN : Trackball::MBUTTON_UP) : 0;
    s |= (button == GLUT_RIGHT_BUTTON)  ? ((state == GLUT_DOWN) ? Trackball::RBUTTON_DOWN : Trackball::RBUTTON_UP) : 0;

    int key_state = glutGetModifiers();
    s |= (key_state & GLUT_ACTIVE_CTRL)  ? Trackball::CTRL_DOWN  : 0;
 //   s |= (key_state & GLUT_ACTIVE_ALT)   ? Trackball::ALT_DOWN   : 0;
    s |= (key_state & GLUT_ACTIVE_SHIFT) ? Trackball::SHIFT_DOWN : 0;

        if (s & Trackball::BUTTON_DOWN){
        trackball.mouseDown(s, x, y);
               if(button == GLUT_LEFT_BUTTON) mousestate = LEFT_DOWN;
        }

        if (s & Trackball::BUTTON_UP){
               if(button == GLUT_LEFT_BUTTON) mousestate = LEFT_UP;

    //    trackball.getMatrix().getValue(viewproperty.tm);

        trackball.mouseUp(s, x, y);
        }

        if(button == GLUT_MIDDLE_BUTTON
           && state == GLUT_DOWN)
        {
               picking[0]->getPickingray((svScalar)x,(svScalar)y);
               if(studyorder[dataindex].taskid == 2)
               {
                    if(invert_matrix(viewproperty.tm, invert_tb))
                    {picking[0]->setPickingray(invert_tb);}
                    if(studyorder[dataindex].encodeid == BEND)
                    {
                         taskrecord.pindex  = picking[0]->getSelectedBar(bendglyph[0]->getBarEnd1(),
                                               bendglyph[0]->getBarEnd2(),
                                               bendglyph[0]->getBarEnd3(),
                                               bendglyph[0]->getBarEnd4(),
                                              1);
                        renderTask3Marker();
                    }
                    else if(studyorder[dataindex].encodeid == LINEAR)
                    {
                         taskrecord.pindex  = picking[0]->getSelectedBar(directglyph[0]->getBarEnd1(),
                                               directglyph[0]->getBarEnd2(),
                                               directglyph[0]->getBarEnd3(),
                                               directglyph[0]->getBarEnd4(),
                                              1);
                        renderTask3Marker();
                    }
                    else if(studyorder[dataindex].encodeid == SPLITVECTORS)
                    {
                         taskrecord.pindex  = picking[0]->getSelectedBar(splitglyph[0]->getBarEnd1(),
                                               splitglyph[0]->getBarEnd2(),
                                               splitglyph[0]->getBarEnd3(),
                                               splitglyph[0]->getBarEnd4(),
                                              1);
                        renderTask3Marker();
                    }
                }
               else if(studyorder[dataindex].taskid == 6)
               {
                     svVecctor3 p;
                     svVector3 lb = bendglyph[0]->GetLb();
                     svVector3 rb = bendglyph[0]->GetRb();
                     if(getPlanePosition(p, lb, rb))
                     {
                            taskrecord.panswerlist.add(p);
                     } 
               }
        }

        glutPostRedisplay();

        mousex = x;
        mousey = image_height-y;

}

void movement(int x, int y)
{
     mousex = x;
     mousey = image_height -y;
     glutPostRedisplay();
}

void motion(int x, int y)
{
        movecount++;
        trackball.mouseMotion(x, y);

 if(movecount == 10)
 {
     trackball.getMatrix().getValue(viewproperty.tm);    

    if(invert_matrix(viewproperty.tm, invert_tb))
    {
     svScalar tx = view_info.eye[0];
     svScalar ty = view_info.eye[1];
     svScalar tz = view_info.eye[2]; 

    viewproperty.eye[0] = tx * invert_tb[0] +
            ty * invert_tb[4] +
            tz * invert_tb[8] +
            invert_tb[12];

     viewproperty.eye[1] = tx * invert_tb[1] +
            ty * invert_tb[5] +
            tz * invert_tb[9] +
            invert_tb[13];

     viewproperty.eye[2] = tx * invert_tb[2] +
            ty * invert_tb[6] +
            tz * invert_tb[10] +
            invert_tb[14];

     movecount = 0;

             if(studyorder[dataindex].encodeid == BEND) bendglyph[0]->Generate(configproperty.bendproperty,viewproperty,
                 configproperty.plane_vector);
             else if(studyorder[dataindex].encodeid ==LINEAR) directglyph[0]->Generate(configproperty.directproperty,viewproperty,
                 configproperty.plane_vector);
             else if(studyorder[dataindex].encodeid ==SPLITVECTORS) splitglyph[0]->Generate(configproperty.splitproperty,viewproperty,
                 configproperty.plane_vector);
        if(studyorder[dataindex].taskid == 4)
        {
             if(studyorder[dataindex].encodeid == BEND) bendglyph[1]->Generate(configproperty.bendproperty,viewproperty,
                 configproperty.plane_vector);
             else if(studyorder[dataindex].encodeid ==LINEAR) directglyph[1]->Generate(configproperty.directproperty,viewproperty,
                 configproperty.plane_vector);
             else if(studyorder[dataindex].encodeid ==SPLITVECTORS) splitglyph[1]->Generate(configproperty.splitproperty,viewproperty,
                 configproperty.plane_vector);

        }
  //   bendglyph->Generate(configproperty.bendproperty,viewproperty,
    //             configproperty.plane_vector);
    }
 } 
       glutPostRedisplay();
}

//configuration files
void Config(char *configfname, ConfigProperty &property)
{
	ifstream infile(configfname);

        float store;
        int num;	
	string tmp;
//dir
        for(int i=0;i<8;i++)
        {
              configproperty.task[i].dir = new char[200];
              infile>>tmp;
              infile>>configproperty.task[i].dir;
        }
//bend
        infile>>tmp;cerr<<tmp<<endl;
        infile>>store;
        num = store;
        for(int i=0;i<num;i++)
        {
           infile>>store;cerr<<store<<endl;
           configproperty.bendproperty.bendRange.add(store);
           configproperty.bendproperty2.bendRange.add(store);
           configproperty.bendproperty3.bendRange.add(store);
           configproperty.bendproperty4.bendRange.add(store);
           infile>>store;
           configproperty.bendproperty.bendHeight.add(store);
           configproperty.bendproperty2.bendHeight.add(store);
           configproperty.bendproperty3.bendHeight.add(store);
           configproperty.bendproperty4.bendHeight.add(store);
           infile>>store;
           configproperty.bendproperty.bendWidth.add(store);
           configproperty.bendproperty2.bendWidth.add(store);
           configproperty.bendproperty3.bendWidth.add(store);
           configproperty.bendproperty4.bendWidth.add(store);
        }
//bar
        infile>>tmp;
        infile>>tmp;
        infile>>configproperty.barproperty.scalex;
        infile>>tmp;
        infile>>configproperty.barproperty.scaley;


//cylinder
	cylinderproperty.SEGMENT_NUM = 10;
	cylinderproperty.scaley = 1;
	cylinderproperty.legend = 10;
	cylinderproperty.legendcolor[0] = 0.5;
        cylinderproperty.legendcolor[1] = 0.5;
        cylinderproperty.legendcolor[2] = 0.5;
        cylinderproperty.legendcolor[3] = 0.5;

//direct
        infile>>tmp;
	infile>>tmp;
	infile>>configproperty.directproperty.UnitHeight;
        infile>>tmp;
        infile>>configproperty.directproperty.width;

//splitvectors
        infile>>tmp;cerr<<tmp<<endl;
        infile>>tmp;
        infile>>configproperty.splitproperty.shiftexp;
        infile>>tmp;
        infile>>configproperty.splitproperty.expHeight;
        configproperty.splitproperty.coeHeight = configproperty.splitproperty.expHeight;
        infile>>tmp;
        infile>>configproperty.splitproperty.expWidth;
        infile>>tmp;cerr<<tmp<<endl;
        infile>>configproperty.splitproperty.coeWidth;
        configproperty.splitproperty.coeColor[0] = 0.5;
        configproperty.splitproperty.coeColor[1] = 0.5;
        configproperty.splitproperty.coeColor[2] = 0.5;
        configproperty.splitproperty.coeColor[3] = 0.5;

//bar
        configproperty.barproperty.halowidth = configproperty.barproperty.scalex*configproperty.bendproperty.bendHeight[0]/2.;
        configproperty.barproperty.legend =1e-6;//1.5e-6 ;
        configproperty.barproperty.legendh = 10 * configproperty.barproperty.scaley;
        configproperty.barproperty.legendw = 2 * configproperty.barproperty.scalex;
        configproperty.barproperty.circleradius = configproperty.barproperty.scalex*configproperty.bendproperty.bendWidth[0]*2.;
        configproperty.barproperty.legendcolor[0]=91/255.;//137./255.;//91/255.;
        configproperty.barproperty.legendcolor[1]=168./255.;//70./255.;//168./255.;
        configproperty.barproperty.legendcolor[2]=126./255.;//64./255.;//126./255.;
        configproperty.barproperty.legendcolor[3]=1;
        configproperty.barproperty.circlecolor[0]=1.;//227./255.;//118./255;//227./255.;
        configproperty.barproperty.circlecolor[1]=0.;//218./255.;//206./255.;//218./255.;
        configproperty.barproperty.circlecolor[2]=0.;//87./255.;//58./255.;//87./255.;
        configproperty.barproperty.circlecolor[3]=1.;
        configproperty.barproperty.dirbarh = 5 * configproperty.barproperty.scaley;

        configproperty.renderproperty.color[0] = 0.5;
        configproperty.renderproperty.color[1] = 0.5;
        configproperty.renderproperty.color[2] = 0.5;
        configproperty.renderproperty.color[3] = 0.5;
        configproperty.barcolor[0] = 76./255.;//248./255.;//76./255.;
        configproperty.barcolor[1] = 28./255.;//170./255.;//28./255.;
        configproperty.barcolor[2] = 95./255.;//161./255.;//95./255.;

        configproperty.frequency = 1;

         infile.close();
	/*-------------------------end----------------------------------------*/
	cerr<<"=>config"<<endl;
	
	/*for(int i=0;i<flow_field->GetPlaneNum();i++)
	{
		for(int j=0;j<contourValues[i].size();j++)
		{
			cerr<<i<<" "<<contourValues[i][j]<<" ";
		}cerr<<endl;
	}*/
} 

void Update()
{
 if(studyorder[pageindex].taskid<7)
 {
   if(pagestatus[pageindex] ==2
     && (confidencelevel == -1 || strlen(inputvalue.c_str())==0))
   {

   } 
   else
   {
   	pageindex++;
   	if(pagestatus[pageindex] == 0)
   	{
     		recordTime();
     		recordTask();
   	}
   	if(pagestatus[pageindex] == 1)
   	{
      		recordTime();
      		recordAnswer();
         	dataindex++;
 //   		cerr<<pageindex<<" "<<pagestatus[pageindex]<<" "<<studyorder[dataindex].taskid<<" "<<studyorder[dataindex].dataid<<" "<<studyorder[dataindex].encodeid<<endl;
     
         	LoadData();
         	trackball.reset();
   		svVector3 center;
  		center[0]=0;center[1]=0;center[2]=0;
  		center.getValue(view_info.coi);

  		view_info.eye[0] = 0;//x/2.0;
  		view_info.eye[1] = 0;//y/2.0;
  		view_info.eye[2] = 5.5;

  		trackball.setEye(view_info.eye);
  		trackball.setFocus(center);

      		if(studyorder[dataindex].taskid ==0)timethreshold = 10;
      		if(studyorder[dataindex].taskid ==1)timethreshold = 60;
      		if(studyorder[dataindex].taskid ==2)timethreshold = 300;

      		if(studyorder[dataindex].taskid <3)
      		{
              		startclock = true;
              		sec = 0;
      		}
     
      		if(studyorder[dataindex].taskid <2)
      		{
         		int index = studyorder[dataindex].dataid;
         		int in = configproperty.pointindex[0][index];
     //    		cerr<<index<<" "<<in<<" ";
         		float mag1 = bendglyph->GetMag(0,in);
         		in =  configproperty.pointindex[1][index];
     //   		cerr<<in<<" ";
         		float mag2 = bendglyph->GetMag(0,in);
          		cerr<<mag1<<" "<<mag2<<" ";
        		if(mag1 > mag2)cerr<<mag1/mag2<<endl;
       	 		else cerr<<mag2/mag1<<endl;

        		if(mag1 > mag2) 
       				{ task1record.truth = mag1/mag2;}
        		else
       				{ task1record.truth = mag2/mag1;}
                	task1record.ctruth1 = mag1;//configproperty.pointindex[0][index];
                	task1record.ctruth2 = mag2;//configproperty.pointindex[1][index];
                	task1record.confidence = confidencelevel;
                	StringtoDouble();

    //   		 startclock = true;
   //    		 sec = 0;
       		}
       		else if(studyorder[dataindex].taskid == 2)
       		{
          		int index = studyorder[dataindex].dataid;
          		task3record.truth = task3truth[index];   
   //       		task3record.confidence = confidencelevel;
          	//	if(studyorder[dataindex].encodeid <4)
          	//	if(task3selectedindex>-1)task3record.panswer =   bendglyph->GetMag(0, task3selectedindex);
       		}
   //  inputvalue.clear();
   //    confidencelevel = -1;
        	recordTime();
        	recordTruth();
       		recordTime();
    	}
     	else
     	{
       		recordTime(); 
       		recordAnswer();
       		recordTime();
       		startclock = false;
     	}
     	inputvalue.clear();
    	confidencelevel = -1;
     	task3selectedindex = -1;
     	timestring.clear();
   }
 }
}

void init(char *configfname)//rbfname, char *cpname)
{
  logInit();
  recordTime();
  recordTask();
  pid = 0;

  pageindex = 0;
  startclock = false;
  timethreshold = 30;
  sec = 0;
  
  for(int i=0;i<7;i++)
  { 
     pagestatus[17*i] = 0;
     for(int j=1;j<17;j++) 
     {
         pagestatus[j + 17*i] = 2-j%2;
     }   
  } 
/*
  pagestatus[0] = 0;
  for(int i=1;i<201;i++)
     pagestatus[i] = 1; 
*/
  movecount = 0;
  configFile = strdup(configfname);

  Config(configfname, configproperty);

  picking = new svPickingray();

  bendglyph = new svBendBar(NULL);
//  bendglyph ->New(NULL,1);

  directglyph = new svDirectCylinder(NULL);
//  directbar ->New(NULL,1);

  splitglyph = new svSplitVectorsCylinder(NULL);
//  splitbar ->New(NULL,1);
  peeling = new svPeeling();
  peeling2 = new svPeeling();
  peeling->SetDisplayList(100);
  peeling2->SetDisplaylist(101);
/*  vector<float> opac;
  opac.push_back(0.5);
  opac.push_back(1);
  opac.push_back(0.5);
  vector<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(12);
  peeling->SetOpacity(opac);
  peeling->SetDisplayList(list);
  */
  
  float c[3];
  c[0]=1;c[1]=1;c[2]=1;
  //peeling->SetBackgroundColor(c);
  //
    peeling ->InitGL("../libs/dual_depth_peeling/shaders/");
  //
    peeling2->InitGL("../libs/dual_depth_peeling/shaders/");

  taskindex = 0;
  dataindex = -1;

  LoadSelectedPoints();
  //LoadMag();
  LoadStudyOrder();
  LoadTask3Answer();

  svVector3 center;
  center[0]=0;center[1]=0;center[2]=0;
  center.getValue(view_info.coi);

  view_info.eye[0] = 0;//x/2.0;
  view_info.eye[1] = 0;//y/2.0;
  view_info.eye[2] = 5.5;

  trackball.setEye(view_info.eye);
  trackball.setFocus(center);
  trackball.setWindowSize(IMAGE_WIDTH, IMAGE_HEIGHT);
  trackball.getMatrix().getValue(viewproperty.tm);
         viewproperty.eye = view_info.eye;

}

void LoadData()
{
  bendglyph ->New(NULL,1);
   directglyph ->New(NULL,1);
   splitglyph ->New(NULL,1);

  char *str = new char[400];
  int index = studyorder[dataindex].dataid;
  taskindex = studyorder[dataindex].taskid; 
  sprintf(str, "%s/D%d", configproperty.task[taskindex].dir, index);
  cerr<<str<<endl;
  bendglyph->SetData(str, 0);
  directglyph->SetData(str, 0);
  splitglyph->SetData(str, 0);

  if(taskindex == 3 || taskindex == 4)
  {
     int index2=studyorder[dataindex].dataid2;
     sprintf(str, "%s/D%d", configproperty.task[taskindex].dir, index2);
     bendglyph->SetData(str, 1);
     directglyph->SetData(str, 1);
     splitglyph->SetData(str, 1);
  }

  bendglyph->ResetCluster();
  directglyph->ResetCluster();
  splitglyph->ResetCluster();

  if(taskindex ==5 || taskindex == 6)
  {
       sprintf(str, "%s/D%d_cluster" , configproperty.task[taskindex].dir, index);
       bendglyph->GenerateClusters(str);
       directglyph->GenerateClusters(str);
       splitglyph->GenerateClusters(str);
  }

  bendglyph->SetBBox();
  directglyph->SetBBox();
  splitglyph->SetBBox();

  delete [] str;  

  vector<float> opac;
  vector<int> list;

  if(studyorder[dataindex].encodeid ==BEND)
  {
  bendglyph->SetBarProperty(configproperty.barproperty);
  bendglyph->SetAlpha(0.5);
  bendglyph->SetDisplayList(10);//cerr<<"done2"<<endl;
  bendglyph->SetLegendList(20);
  if(taskindex ==5 || taskindex == 6)
      bendglyph->SetColorByCluster();//SetColor(configproperty.barcolor);
  else
      bendglyph->SetColor(configproperty.barcolor);
  bendglyph->SetSampling(configproperty.frequency);
  bendglyph->Generate(configproperty.bendproperty,viewproperty,
                 configproperty.plane_vector,0);//cerr<<"done4"<<endl;

  svVector3 startPos = svVector3(10,-9.5,-1.5);
  svVector3 dir1= svVector3(1,0,0);
  svVector3 dir2=svVector3(0,1,0);
  svScalar stepD1 = 0.5;
  svScalar stepD2 = 0.5;
  svInt stepN1 = 17;
  svInt stepN2 = 19;
  bendglyph->GenerateLegend(configproperty.bendproperty,startPos, dir1,  dir2,
                        stepD1,  stepD2,
                        stepN1,  stepN2,configproperty.plane_vector);
  bendglyph->SetRenderProperty(configproperty.renderproperty);
  opac.push_back(0.5);
  opac.push_back(1);
  if(taskindex == 0 || taskindex == 1 || taskindex == 2 || taskindex ==6)
       opac.push_back(0.5);
  list.push_back(10);
  list.push_back(20);
  if(taskindex == 0 || taskindex == 1 || taskindex == 2 || taskindex == 6)
       list.push_back(12);
  peeling->SetOpacity(opac);
  peeling->SetDisplayList(list);

  if(taskindex == 4)
  {
      bendglyph->SetDisplayList(110);
      bendglyph->Generate(configproperty.bendproperty,viewproperty,
                 configproperty.plane_vector,1);

      vector<int> list2;list2.push_back(110);
      vector<float> opac2;opac2.push_back(0.5);
      peeling2->SetOpacity(opac2);
      peeling2->SettDisplayList(list2);      
  }
  }
  else if(studyorder[dataindex].encodeid ==LINEAR)
  {
  directglyph->SetBarProperty(configproperty.barproperty);
  directglphh->SetAlpha(0.5);
  directglyph->SetDisplayList(15);//cerr<<"done1"<<endl;
  if(taskindex ==5 || taskindex == 6)
       directglyph->SetColorByCluster();
  else
       directglyph->SetColor(configproperty.barcolor);//cerr<<"done2"<<endl;
  directglyph->SetROI((svScalar)1e-7, (svScalar)1);
  directglyph->Generate(configproperty.directproperty, viewproperty,
                 0);//cerr<<"done3"<<endl;
  directglyph->SetRenderProperty(configproperty.renderproperty);

  opac.push_back(0.5);
  opac.push_back(1);
  if(taskindex == 0 || taskindex == 1 || taskindex == 2 || taskindex ==6)
  opac.push_back(0.5);
  list.push_back(15);
  list.push_back(20);
  if(taskindex == 0 || taskindex == 1 || taskindex == 2 || taskindex ==6)
  list.push_back(12);
  peeling->SetOpacity(opac);
  peeling->SetDisplayList(list);

  if(taskindex == 4)
  {
      directglyph->SetDisplayList(110);
      directglyph->Generate(configproperty.bendproperty,viewproperty,
                 1);

      vector<int> list2;list2.push_back(110);
      vector<float> opac2;opac2.push_back(0.5);
      peeling2->SetOpacity(opac2);
      peeling2->SettDisplayList(list2);
  }

  }
  else if(studyorder[dataindex].encodeid ==SPLITVECTORS)
  {
  splitglyph->SetBarProperty(configproperty.barproperty);
  splitglyph->SetAlpha(0.5);
  splitglyph->SetDisplayList(5);//cerr<<"done1"<<endl;
  if(taskindex ==5 || taskindex == 6)
    splitglyph->SetColorByCluster();
  else
    splitglyph->SetColor(configproperty.barcolor);//cerr<<"done2"<<endl;
  splitglyph->SetROI((svScalar)1e-7, (svScalar)1);
  splitglyph->Generate(configproperty.splitproperty, viewproperty,
                 1);
  splitglyph->SetRenderProperty(configproperty.renderproperty);

  opac.push_back(0.5);
  opac.push_back(1);
  if(taskindex == 0 || taskindex == 1 || taskindex == 2 || taskindex ==6)
  opac.push_back(0.5);
  list.push_back(5);
  list.push_back(20);
  if(taskindex == 0 || taskindex == 1 || taskindex == 2 || taskindex ==6)
  list.push_back(12);
  peeling->SetOpacity(opac);
  peeling->SetDisplayList(list);

  if(taskindex == 4)
  {
      splitglyph->SetDisplayList(110);
      splitglyph->Generate(configproperty.bendproperty,viewproperty,
                 1);

      vector<int> list2;list2.push_back(110);
      vector<float> opac2;opac2.push_back(0.5);
      peeling2->SetOpacity(opac2);
      peeling2->SettDisplayList(list2);
  }

  }
  if(studyorder[dataindex].taskid <2)
  {
        renderTask12marker();
  }
  else if (studyorder[dataindex].taskid == 2)
  {
         task3selectedindex = -1;
         renderTask3marker();
  }
  else if (studyorder[dataindex].taskid == 5)
  {
         renderTask6vector();
  }
  else if (studyorder[dataindex].taskid == 7)
  {
         renderTask8legend();
  }
}

//**********************
// program entry
//**********************

int main(int argc, char** argv)
{
  srand(12345);
  
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH| GLUT_MULTISAMPLE);

        image_width  = IMAGE_WIDTH;
        image_height = IMAGE_HEIGHT;

        glutInitWindowSize(image_width, image_height);
//glfwWindowHint(GLFW_SAMPLES, 4);
        glutCreateWindow("QDOT");
        glutInitWindowPosition(200, 200);
        if (glewInit() != GLEW_OK)
        {
                printf("glewInit failed. Exiting...\n");
                exit(1);
        }

        if(argc == 2)
        {
//            init(argv[1]);
        }
        else
        {
            exit(0);
        }
	//        glutDisplayFunc(display)
	//
	//   ;

             	glutReshapeFunc(reshape);
		init(argv[1]);
		glutDisplayFunc(display);

		glutKeyboardFunc(key);
		glutMouseFunc(mouse);
		glutMotionFunc(motion);
                glutPassiveMotionFunc(movement);
   glutTimerFunc(1000,myTimer,FPS);

		glutMainLoop();
                logEnd();
		return 0;
	}

