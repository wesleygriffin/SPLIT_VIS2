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
svPeeling *peeling[2];

GLfloat invert_tb[16];

char *configFile;

int movecount;
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

 
        ViewProperty viewproperty;	
} configproperty;


void Config(char *configfname, ConfigProperty &property);

GLboolean bBoundingBox=true;
GLboolean bAxis;

GLuint image_width;
GLuint image_height;

GLfloat m[16];

Trackball trackball;


int mousex;
int mousey;
int mousestate;

void LoadData();

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

    peeling->Reshape(w,h);
    peeling-> DeleteDualPeelingRenderTargets();
    peeling-> InitDualPeelingRenderTargets();

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

   picking->initCamera(view_info.eye,view_info.coi,UP,
                      GLfloat_fov, w, h, 0.1);
}

void defaultdisplay()
{
        trackball.getMatrix().getValue(m);
        trackball.getMatrix().getValue(viewproperty.tm);
        glPushMatrix();
        glMultMatrixf(m);
        glColor3f(0,0,0);

         peeling->RenderDualPeeling();

        glPopMatrix();
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

             defaultdisplay();

     //   glPopMatrix();
        
        glutSwapBuffers();
}


void key(unsigned char key, int x, int y)
{
        switch (key) {
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
        trackball.mouseMotion(x, y);

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

void init(char *configfname)//rbfname, char *cpname)
{
  movecount = 0;
  configFile = strdup(configfname);

  Config(configfname, configproperty);

  bendglyph = new svBendBar(NULL);

  directglyph = new svDirectCylinder(NULL);

  splitglyph = new svSplitVectorsCylinder(NULL);

  peeling = new svPeeling();
  peeling->SetDisplayList(100);
  
    peeling ->InitGL("../libs/dual_depth_peeling/shaders/");



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

  char *str = new char[400];
  sprintf(str, "%s/D%d", configproperty.task[0].dir, 1);
  cerr<<str<<endl;
  bendglyph->SetData(str, 0);

  bendglyph->ResetCluster();

  bendglyph->SetBBox();

  delete [] str;  

  vector<float> opac;
  vector<int> list;

  bendglyph->SetBarProperty(configproperty.barproperty);
  bendglyph->SetAlpha(0.5);
  bendglyph->SetDisplayList(10);//cerr<<"done2"<<endl;
  bendglyph->SetLegendList(20);
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
  opac.push_back(0.5);
  list.push_back(10);
  list.push_back(20);
  list.push_back(12);
  peeling->SetOpacity(opac);
  peeling->SetDisplayList(list);
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

