#include "dual_depth_peeling.h"
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <assert.h>
#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <GLFW/glfw3.h>
//#include "svVectorField.h"
#include "svQDOT.h"
#include "svDirectArrow.h"
#include "svSummaryGlyph.h"
//#include "svGlyph.h"
#include "svUtil.h"
#include "ivTrackball.h"
#include "ivview.h"
#include "MGL.h"
#include "svOutline.h"
#include "svMesh.h"
#include "svConfig.h"
#include <string.h>

#include <GL/glui.h>

#define LINEAR 0
#define LLo 1
#define LC 2
#define LT 3

#define SYMMETRY_UP 0
#define SYMMETRY_DOWN 1
#define SYMMETRY_UP_SELECT 2
#define SYMMETRY_DOWN_SELECT 3
#define SYMMETRY_TYPE_NUM 4
#define SYMMETRY_STATUS_NUM 4
#define CLUSTER_DIMENSION 7

#define SYMMETRY_ID 1
#define OVERVIEW_ID 2
#define LENGTH_ID 3
#define ARROW_ID 4
#define TUBE_ID 5
#define FREQ_ID 6
#define MESH_ID 7
#define MESH_TYPE_ID 8
#define MESH_VIS_ID 9
#define MAG_ID 10
#define LAYER_NUM_ID 11
#define LAYER_ID 12

using namespace __svl_lib;

void reshape(int w, int h);
void display(void);
void key(unsigned char key, int x, int y);

#define IMAGE_WIDTH 1920
#define IMAGE_HEIGHT 1080

int window;
int mouse_button;
int mouse_state;
//=======GLUI==============
//
//===========================
GLUI *glui;
GLUI_Scrollbar *sb_line;
GLUI_Scrollbar *sb_arrow;
GLUI_Scrollbar *sb_tube;
GLUI_Scrollbar *sb_mag;
GLUI_Scrollbar *sb_layer;
GLUI_Listbox *list_mesh_type;
GLUI_StaticText *text_mag_min;
GLUI_StaticText *text_mag_max;
GLUI_StaticText *text_layer_min;
GLUI_StaticText *text_layer_max;
//=========END================


view3d view_info;

svPeeling *peeling;
svQDOT *flow_field;
svDirectArrow *directglyph;
svSummaryGlyph *summaryglyph;
svOutline *outline;
svMesh *mesh;

char *configFile;


static GLuint *texture;//[4*3];
svVector3 *texture_pos;
svScalar texture_size;
int *texture_status;
int textureinit;
vector<int> symmetrytype;
int symmetry_enable;


//===============
float summary_length_scale;
float summary_arrow_scale;
float summary_tube_scale;
float length_scale;
float arrow_scale;
float tube_scale;
float glui_length_scale;
float glui_arrow_scale;
float glui_tube_scale;
bool length_scale_up=false;
bool arrow_scale_up=false;
bool tube_scale_up=false;

int mesh_enable;
int mesh_type;
vector<int> unique_region;
int mesh_vis;
svInt mesh_solid_list = 50;

svScalar mag_show;
svScalar mag_max=1e-4;
svScalar mag_min=1e-16;
float glui_mag_show=0.5;

int znum;
int layer_min=0;
int layer_max=108;
int layer_show=50;
float glui_layer_show=0.5;

int encode_type;
///////////////////

int frequency = 2;
int summaryVisible;
svScalar scale;
svScalar directradius;
svScalar summaryradius;
int regioncount = 0;

struct ConfigProperty{
	
	char *rawDir;
	char *rawFile;
	char *storeDir;
	
	svVector3 plane_center;
    svVector3 plane_vector;
    svScalar plane_distance;
	
	svBool isContour;
	
	KmeansProperty step1_kmeansproperty;
	KmeansProperty step2_kmeansproperty;
        SymmetryProperty symmetryproperty; 

        vector<string> contourname;	
	ContourProperty contourproperty;
	
	svScalarArray *magrange;
	
} configproperty;

int contourindex = 0;
void Config(char *configfname, ConfigProperty &property);
void Update();
void LocateTexture();
void LoadTexture();


GLboolean bBoundingBox=true;
GLboolean bAxis;

GLuint image_width;
GLuint image_height;

GLint nx, ny, nz;
Trackball trackball;

double zmin;
double zmax;
double zshuffle;

void InitLight()
{
  //setting of lighting
  GLfloat mat_diffuse[] = { 0.8, .0, 0, .4};
  GLfloat mat_specular[] = { 0.2,0.2,0.2,0.2 };
  GLfloat mat_shininess[] = { 20 };
  //GLfloat light_position[] = { 24, 24, 60, 0.0 };
  GLfloat light_position[] = { 0,30,100, 0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
 // glClearColor(0.0, 0.0, 0.0, 0.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
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

    peeling->Reshape(w,h);
    peeling-> DeleteDualPeelingRenderTargets();
    peeling-> InitDualPeelingRenderTargets();

        image_width  = w;
        image_height = h;

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
                                           view_info.hither,
                                           view_info.yon);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                gluLookAt(view_info.eye[0],view_info.eye[1],view_info.eye[2],
                                view_info.coi[0],view_info.coi[1],view_info.coi[2],
                                0, 1, 0);

        

        InitLight();
        LocateTexture(); 
}

void DrawSymmetryButtons(void)
{
       glEnable (GL_BLEND); 
       glEnable(GL_TEXTURE_2D);

//       LoadTexture();
       glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       glPushMatrix();
     //  glTranslatef(500,0,0);
     //  glScalef(100, 100, 100);
     
       for(int i=0;i<SYMMETRY_TYPE_NUM;i++)
       {
          int index1 = texture_status[i]+i*SYMMETRY_STATUS_NUM; 
          glBindTexture(GL_TEXTURE_2D, texture[index1]);
          //cerr<<index1<<" ";
          glBegin(GL_QUADS);
          glTexCoord2d(0,0); 
          glVertex2f(texture_pos[i][0],texture_pos[i][1]+texture_size);
          glTexCoord2d(1,0); 
          glVertex2f(texture_pos[i][0]+1.7*texture_size,
                          texture_pos[i][1]+texture_size);
          glTexCoord2d(1,1); 
          glVertex2f(texture_pos[i][0]+1.7*texture_size,texture_pos[i][1]);
          glTexCoord2d(0,1); 
          glVertex2f(texture_pos[i][0],texture_pos[i][1]);
          glEnd();
          //cerr<<texture_pos[i][0]<<" "<<texture_pos[i][1]<<endl;
       }//cerr<<endl;

       glPopMatrix();
       
       glDisable(GL_TEXTURE_2D);
       glDisable(GL_BLEND); 

}


void idle()
{
	if (glutGetWindow() != window)
		glutSetWindow(window);
	
        if(arrow_scale_up && sb_arrow->ifmouseup())
        {
            // cerr<<"arrow up"<<endl;
             arrow_scale_up = false;
        }
        glutPostRedisplay();

}

void display(void)
{
        // set new model view and projection matrix
        glClearColor(0.5, 0.5, 0.5, 1);
//	glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

     //   glEnable(GL_COLOR_MATERIAL);
     //   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
     //   InitLight();
       //glEnable(GL_MULTISAMPLE);

        //peeling->Reshape(image_width, image_height);
        //peeling-> DeleteDualPeelingRenderTargets();
        //peeling-> InitDualPeelingRenderTargets();

        GLfloat m[16];
        trackball.getMatrix().getValue(m);

        glPushMatrix();
        glMultMatrixf(m);


 //       if (bBoundingBox)
                directglyph->DrawOutline();
        //if (bAxis)
        //	draw_axis();

//        glEnable(GL_LIGHTING);
//        glEnable(GL_LIGHT0);
//        glEnable(GL_TEXTURE_2D);
         outline->DrawXYZ(directglyph->GetLb(), directglyph->GetRb());
         outline->DrawAxis_scale(directglyph->GetLb(), directglyph->GetRb(),image_width);

        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

         peeling->RenderDualPeeling();

        directglyph->Render();
        if(summaryVisible)
              summaryglyph->Render();
       // mesh->Render();

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

        glPopMatrix();
//=================================
        ViewProperty viewproperty;

       glGetDoublev (GL_MODELVIEW_MATRIX, viewproperty.mvmatrix);
    glGetDoublev (GL_PROJECTION_MATRIX, viewproperty.projmatrix);
    glGetIntegerv( GL_VIEWPORT, viewproperty.viewport );

        glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(0.0,(GLfloat) viewproperty.viewport[2], 0.0, (GLfloat) viewproperty.viewport[3]);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glDisable(GL_LIGHTING);

	glPushMatrix();
	glTranslatef(200,0,0);
        if(summaryVisible)
		summaryglyph->RenderColor();
        glPopMatrix();

        if(symmetry_enable)
                DrawSymmetryButtons();


                glMatrixMode(GL_PROJECTION);
                glLoadMatrixd(viewproperty.projmatrix);
                glMatrixMode(GL_MODELVIEW);
                glLoadMatrixd(viewproperty.mvmatrix);

        glFlush();
        glutSwapBuffers();
}
void SymmetryProcess()
{
      symmetrytype.clear();

      for(int i=0;i<SYMMETRY_TYPE_NUM;i++)
      {
              if(texture_status[i] == SYMMETRY_DOWN)
              {
                  symmetrytype.push_back(i);
              }
      }
   directglyph->ResetVisible();
   directglyph->SetSampling(symmetrytype, frequency);
   directglyph->SetSymmetryVisible(symmetrytype);
   directglyph->SetVisible(contourindex);
   directglyph->SetVisible(zmin, zmax);
   glutSetWindow(window);
   directglyph->Generate();

   summaryglyph->ResetVisible();
cerr<<"======Symmetry======"<<endl;   
   summaryglyph->SetSampling(symmetrytype, frequency);
cerr<<"Symmetry"<<endl;   
   summaryglyph->SetSymmetryVisible(symmetrytype);
cerr<<"Symmetry"<<endl;   
   summaryglyph->SetVisible(contourindex);
cerr<<"Symmetry"<<endl;   
   summaryglyph->SetVisible(zmin, zmax);
cerr<<"Symmetry"<<endl;
   summaryglyph->Generate(1);

}
void VisibleProcess()
{
   symmetrytype.clear();
   directglyph->ResetVisible();
   directglyph->SetSampling(symmetrytype, frequency);
   directglyph->SetVisible(contourindex);
   directglyph->SetVisible(zmin, zmax);
   glutSetWindow(window);
   directglyph->Generate();

   summaryglyph->ResetVisible();
   summaryglyph->SetSampling(symmetrytype, frequency);
   summaryglyph->SetVisible(contourindex);
   summaryglyph->SetVisible(zmin, zmax);
   summaryglyph->Generate(1);

}
void control_cb(int control)
{
     if(control == SYMMETRY_ID)
     {
           cerr<<symmetry_enable<<endl;
           if(symmetry_enable == true)
           {
                SymmetryProcess();
           }       
           else
           {
                 VisibleProcess();
           }
     }
     else if(control == OVERVIEW_ID)
     {
          // summaryVisible = 1-summaryVisible;
     }
     else if(control == ARROW_ID)
     {
          arrow_scale_up = true;
         
     }
  //   cerr<<sb_arrow->ifmouseup()<<endl;
     glutSetWindow(window);
/*
         if(sb_arrow->ifmouseup())
         {
             cerr<<"scrolldown"<<endl;
             scrolldown = false;
         }
 */
     glutPostRedisplay();
}

void key(unsigned char key, int x, int y)
{
        switch (key) {
        case 'f':
             if(frequency>1)frequency--;
               directglyph->SetSampling(symmetrytype, frequency);
              directglyph->Generate();
cerr<<frequency<<endl;
              break;
        case 'F':
              frequency++;
cerr<<frequency<<endl;
                directglyph->SetSampling(symmetrytype, frequency);
              directglyph->Generate();
              break;
	case 'H':
        case 'h':
              trackball.reset();
              break;
        case 'm':
              {if(regioncount > unique_region.size()-1) regioncount = 0;
              else regioncount++;  
              if(regioncount == unique_region.size())
              {  
                 char *str = new char[200];
                 sprintf(str, "%s/%s/mesh%d.txt", configproperty.storeDir, configproperty.rawFile, 100);
                 mesh->GenerateWireframe(str);
                 delete [] str;

                  cout<<"no region is displayed"<<endl;}
              else
              {
                 char *str = new char[200];
                 sprintf(str, "%s/%s/mesh%d.txt", configproperty.storeDir, configproperty.rawFile, unique_region[regioncount]);
                 mesh->GenerateWireframe(str);
                 cout<<"region "<<unique_region[regioncount]<<endl;
                 delete [] str;
              }
              break;}
        //case 'S':
        case 's':
              summaryVisible = 1-summaryVisible;
	      break;
        case 'x':
                scale = scale*1.5;	
		directradius = directradius * 1.5;
	//	summaryradius = summaryradius * 1.5;
                directglyph->SetScale(scale);
		directglyph->SetRadius(directradius);
                directglyph->Generate();
		// summaryglyph->SetRadius(summaryradius);
                summaryglyph->SetScale(scale);
                summaryglyph->Generate(1);
		break;
	case 'X':
                scale = scale/1.5;
                directradius = directradius / 1.5;
          //      summaryradius = summaryradius / 1.5;
		directglyph->SetScale(scale);
                directglyph->SetRadius(directradius);
               directglyph->Generate();
              //  summaryglyph->SetRadius(summaryradius);
                summaryglyph->SetScale(scale);
                summaryglyph->Generate(1);
		break;
        case 'u':
        case 'U':
               Update();
               break;
        case 'c':
        case 'C':
              if(contourindex<directglyph->GetContourListSize()-1)contourindex++;
              else contourindex = 0;
              cerr<<"The current contour: "<<configproperty.contourname[contourindex]<<endl;
               directglyph->ResetVisible();
              directglyph->SetVisible(contourindex);
                directglyph->SetVisible(zmin, zmax);
               directglyph->Generate();
               summaryglyph->ResetVisible();
               summaryglyph->SetVisible(contourindex);
               summaryglyph->SetVisible(zmin, zmax);
               summaryglyph->Generate(1);
               break;
        case  'l':
               {cout<<"Please input the index of the min layer"<<endl;
               cin>>zmin;
               cout<<"Please input the index of the  max layer"<<endl;
               cin>>zmax;
               directglyph->ResetVisible();
               directglyph->SetVisible(contourindex);
               directglyph->SetVisible(zmin, zmax);
               directglyph->Generate();
               summaryglyph->ResetVisible();
               summaryglyph->SetVisible(contourindex);
               summaryglyph->SetVisible(zmin, zmax);
               summaryglyph->Generate(1);
               svVector3 p = flow_field->GetPlanePosition(zmin);
               cout<<"min plane: "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
               p = flow_field->GetPlanePosition(zmax);
               cout<<"max plane: "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
              // cout<<"zmin "<<zmin<<", zmax "<<zmax<<endl;
               }break;   
        /*case 'z':
	      if(zmin>0)			zmin--;
               directglyph->SetVisible(zmin, zmax);
               directglyph->Generate();
               summaryglyph->SetVisible(zmin, zmax);
               summaryglyph->Generate(1);
               cout<<"zmin "<<zmin<<", zmax "<<zmax<<endl;
               break;
        case 'Z':
               if(zmin<zmax) zmin++;
               directglyph->SetVisible(zmin, zmax);
               directglyph->Generate();
               summaryglyph->SetVisible(zmin, zmax);
               summaryglyph->Generate(1);
               cout<<"zmin "<<zmin<<", zmax "<<zmax<<endl;
               break;
               
        case 'x': if(zmax>zmin)zmax--;
                directglyph->SetVisible(zmin, zmax);
               directglyph->Generate();
               summaryglyph->SetVisible(zmin, zmax);
               summaryglyph->Generate(1);
                cout<<"zmin "<<zmin<<", zmax "<<zmax<<endl;
                break;
        case 'X':
                if(zmax<flow_field->GetPlaneNum()-1)zmax++;
                directglyph->SetVisible(zmin, zmax);
               directglyph->Generate();
               summaryglyph->SetVisible(zmin, zmax);
               summaryglyph->Generate(1);
                cout<<"zmin "<<zmin<<", zmax "<<zmax<<endl;
                break;
               */ 
        case 't':if(zmin>0)
                {zmin = zmin - 1;
                zmax = zmax - 1;
               directglyph->ResetVisible();
               directglyph->SetVisible(contourindex);
               directglyph->SetVisible(zmin, zmax);
               directglyph->Generate();
               summaryglyph->ResetVisible();
               summaryglyph->SetVisible(contourindex);
               summaryglyph->SetVisible(zmin, zmax);
               summaryglyph->Generate(1);
               svVector3 p = flow_field->GetPlanePosition(zmin);
               cout<<"min plane: "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
               p = flow_field->GetPlanePosition(zmax);
               cout<<"max plane: "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
               // cout<<"zmin "<<zmin<<", zmax "<<zmax<<endl;
                }break; 
        case 'T':
              if(zmax <flow_field->GetPlaneNum()-1)
              {
                zmin = zmin + 1;
                zmax = zmax + 1;
               directglyph->ResetVisible();
               directglyph->SetVisible(contourindex);
                directglyph->SetVisible(zmin, zmax);
               directglyph->Generate();
               summaryglyph->ResetVisible();
               summaryglyph->SetVisible(contourindex);
               summaryglyph->SetVisible(zmin, zmax);
               summaryglyph->Generate(1);
               svVector3 p = flow_field->GetPlanePosition(zmin);
               cout<<"min plane: "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
               p = flow_field->GetPlanePosition(zmax);
               cout<<"max plane: "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
               // cout<<"zmin "<<zmin<<", zmax "<<zmax<<endl;
               } break;
           
                
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
    mouse_button = button;
    mouse_state = state;
cerr<<mouse_button<<" "<<state<<endl;
        long s=0x00000000;
    s |= (button == GLUT_LEFT_BUTTON)   ? ((state == GLUT_DOWN) ? Trackball::LBUTTON_DOWN : Trackball::LBUTTON_UP) : 0;
    s |= (button == GLUT_MIDDLE_BUTTON) ? ((state == GLUT_DOWN) ? Trackball::MBUTTON_DOWN : Trackball::MBUTTON_UP) : 0;
    s |= (button == GLUT_RIGHT_BUTTON)  ? ((state == GLUT_DOWN) ? Trackball::RBUTTON_DOWN : Trackball::RBUTTON_UP) : 0;

    int key_state = glutGetModifiers();
    s |= (key_state & GLUT_ACTIVE_CTRL)  ? Trackball::CTRL_DOWN  : 0;
//    s |= (key_state & GLUT_ACTIVE_ALT)   ? Trackball::ALT_DOWN   : 0;
    s |= (key_state & GLUT_ACTIVE_SHIFT) ? Trackball::SHIFT_DOWN : 0;

       if(symmetry_enable && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
       {
        bool flag = false;
        for(int i=0;i<SYMMETRY_TYPE_NUM;i++)
        {
       //     cerr<<x<<" "<<image_height-y<<" "<<texture_pos[i][0]<<" "<<texture_pos[i][0]+texture_size<<" "<<texture_pos[i][1]<<" "<<texture_pos[i][1]+texture_size<<endl;
             if(x > texture_pos[i][0] && x < texture_pos[i][0]+texture_size*1.5
              && image_height-y>texture_pos[i][1] 
              && image_height-y<texture_pos[i][1]+texture_size)
            {//cerr<<i<<" "<<texture_status[i]<<" ";
                 if(texture_status[i] == SYMMETRY_DOWN_SELECT)
                       texture_status[i] = SYMMETRY_UP;
                 else if (texture_status[i] == SYMMETRY_UP_SELECT)
                      texture_status[i] = SYMMETRY_DOWN;
                 else if(texture_status[i] == SYMMETRY_UP)
                         texture_status[i] = SYMMETRY_DOWN_SELECT;
                 else if(texture_status[i] == SYMMETRY_DOWN)
                         texture_status[i] = SYMMETRY_UP_SELECT;
                 flag = true;
               // cerr<<texture_status[i]<<endl;
            }     
        }
        if(flag)
        SymmetryProcess();
      }

        if (s & Trackball::BUTTON_DOWN){
        trackball.mouseDown(s, x, y);
        }

        if (s & Trackball::BUTTON_UP){
        trackball.mouseUp(s, x, y);
        }
        glutPostRedisplay();

}

void motion(int x, int y)
{
        trackball.mouseMotion(x, y);
        glutPostRedisplay();
}

void movement(int x, int y)
{
       if(symmetry_enable)
       {
           for(int i=0;i<SYMMETRY_TYPE_NUM;i++)
           {
             if(x > texture_pos[i][0] && x < texture_pos[i][0]+texture_size *1.5
              && image_height-y>texture_pos[i][1]
              && image_height-y<texture_pos[i][1]+texture_size)
            {
                 if(texture_status[i] == SYMMETRY_UP)
                         texture_status[i] = SYMMETRY_UP_SELECT;
                 else if(texture_status[i] == SYMMETRY_DOWN)
                         texture_status[i] = SYMMETRY_DOWN_SELECT;
            } 
            else
            {
                 if(texture_status[i] == SYMMETRY_UP_SELECT)
                         texture_status[i] = SYMMETRY_UP;
                 else if(texture_status[i] == SYMMETRY_DOWN_SELECT)
                         texture_status[i] = SYMMETRY_DOWN;
            }
           }
       }
       glutPostRedisplay();
}
//******************************************
// initialization code for GL and NV_EXT
//******************************************
/*
struct ConfigProperty{
	
	char *rawDir;
	char *rawFile;
	char *storeDir;
	
	svVector3 plane_center;
    svVector3 plane_vector;
    svScalar plane_distance;
	
	svBool isContour;
	
	KmeansProperty kmeansproperty;
	ContourProperty contourproperty;
	
	svScalarArray *magrange;
	
} configproperty;
*/

void Config(char *configfname, ConfigProperty &property)
{
	ifstream infile(configfname);
	
	string tmp;
/*-------------file names-----------------*/
	infile>>tmp;
	infile>>property.rawDir;
	infile>>tmp;
	infile>>property.rawFile;
	infile>>tmp;
	infile>>property.storeDir;
//cerr<<tmp<<endl;
	mkdir(property.storeDir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  

        infile>>tmp;
	infile>>property.plane_center[0]>>property.plane_center[1]>>property.plane_center[2];
	infile>>tmp;
	infile>>property.plane_vector[0]>>property.plane_vector[1]>>property.plane_vector[2];
	infile>>tmp;
	infile>>property.plane_distance;
	
/*--------------initialization of QDOT field------------*/
	char *qdot_format = new char[400];
	sprintf(qdot_format,"%s/%s/format.txt", property.storeDir, 
	            property.rawFile);
        flow_field->SetVTK(property.rawDir, property.rawFile,
	                   property.storeDir,
					   "sort.txt", "format.txt", "density.txt",
					   property.plane_center,
					   property.plane_vector,
					   property.plane_distance);
        flow_field->New(qdot_format);	
        char *str = new char[200];

        sprintf(str, "%s/%s/", property.storeDir,  property.rawFile);				
        flow_field->NewMesh(str);
	delete [] qdot_format;
	
/*------------------contour----------------------*/
	infile>>tmp; //cerr<<tmp<<endl;
	double store;
//	infile>>property.isContour;
//	infile>>tmp; //cerr<<tmp<<endl;
	int num;
	infile>>num;
        property.contourproperty.seed_num = flow_field->GetPlaneNum();
        property.contourproperty.contourValues = new svScalarArray[flow_field->GetPlaneNum()];
        property.contourproperty.isUpdate.free();
        for(int i=0;i<flow_field->GetPlaneNum();i++)
                property.contourproperty.isUpdate.add(0);
	char linestr[5];

       property.contourname.clear();

//       char *str = new char[200];
       sprintf(str, "%s/%s/density.txt", property.storeDir, property.rawFile);
  //   cerr<<num<<" "<<str<<endl; 
	for(int i=0;i<num;i++)
	{
               std::string cn("");

		infile>>linestr;//cerr<<linestr<<endl;
		if(!strcmp(linestr,"G"))
		{
			//cerr<<linestr<<endl;
			char linestr2[10];
			infile>>linestr2;

			if(!strcmp(linestr2,"A"))
			{
                        cn += "Absolute value ";

				infile>>store;
                                       std::ostringstream ss;
                                        ss<<store;
                                        cn += ss.str();

				for(int j=0;j<flow_field->GetPlaneNum();j++)
				{
					property.contourproperty.contourValues[j].add(store);
				       property.contourproperty.isUpdate[j] = 1;
                                 }
			}
			else
			{
                        cn += "Ratio ";

				infile>>store;
                                       std::ostringstream ss;
                                        ss<<store;
                                        cn += ss.str();

				//cerr<<store<<endl;
				for(int j=0;j<flow_field->GetPlaneNum();j++)
				{
					property.contourproperty.contourValues[j].add(store*flow_field->GetMaxDensitybyLayer(str, j));
                                       property.contourproperty.isUpdate[j] = 1;
				}
			}
		}
		else
		{
			char linestr2[10];
			infile>>linestr2;
			if(!strcmp(linestr2,"A"))
			{
				infile>>tmp; //[
				vector<double> store3(2);
				infile>>store3[0]>>store3[1]; //index of layer
				infile>>tmp; //]
				infile>>store; //value
				
				int index1 = (int)store3[0];
				int index2 = (int)store3[1];
				for(int j=index1;j<index2;j++)
				{
					property.contourproperty.contourValues[j].add(store);
                                                                               property.contourproperty.isUpdate[j] = 1;

				}
			}
			else
			{
				infile>>tmp;
				vector<double> store3(2);
				infile>>store3[0]>>store3[1];
				infile>>tmp;
				infile>>store;
				
				int index1 = (int)store3[0];
				int index2 = (int)store3[1];			
				for(int j=index1;j<index2;j++)
				{
					property.contourproperty.contourValues[j].add( store*flow_field->GetMaxDensitybyLayer(str, j));
                                       property.contourproperty.isUpdate[j] = 1;

				}			
			}			
		}
              property.contourname.push_back(cn);//cerr<<property.contourname[i]<<endl;
//		property.contourproperty.isUpdate.add(1);
	}
	sprintf(str, "%s/%s/contour.txt", property.storeDir, property.rawFile);
	property.contourproperty.outputfile = strdup(str);
	sprintf(str, "%s/%s/", property.storeDir, property.rawFile);
	property.contourproperty.vtkdir = strdup(str);
//    cerr<<"done"<<endl;
	
	/*--------------clustering------------------------------*/
	infile>>tmp;// cerr<<tmp<<endl;
	infile>>tmp;

	property.magrange = new svScalarArray[2];
	
	/*-------------step 1-----------------------------------*/
    property.step1_kmeansproperty.dimension = 7;
    property.step1_kmeansproperty.isNormalize = true;
     property.step1_kmeansproperty.clusterLayer.free();
    property.step1_kmeansproperty.clusterWeight.free();
    for(int i=0;i<flow_field->GetPlaneNum();i++)
    {
        property.step1_kmeansproperty.clusterLayer.add(-1);
    }   
  	infile>>tmp;// cerr<<tmp<<endl;
	for(int i=0;i<CLUSTER_DIMENSION;i++)
	{
		infile>>store;
		property.step1_kmeansproperty.clusterWeight.add(store);
	}	
	int index1, index2;
	infile>>tmp;//cerr<<tmp<<endl;
	infile>>index1;
	infile>>index2;
        if (index2>=flow_field->GetPlaneNum()) index2=flow_field->GetPlaneNum()-1;
	infile>>tmp; //cerr<<tmp<<endl;
	infile>>store;  
    bool isWhole = store;
    if(isWhole)
    {
                 for(int i=index1; i<=index2;i++)
                {
                   property.step1_kmeansproperty.clusterLayer[i]++;
                }
    }
    else
    {
            int count = 1;
             for(int i=index1; i<=index2;i++)
                {
                   property.step1_kmeansproperty.clusterLayer[i]= property.step1_kmeansproperty.clusterLayer[i]+count;
                   count++; 
                }
    }
	infile>>tmp; //cerr<<tmp<<endl;
	infile>>store;  property.magrange[0].add(store);
	infile>>store;  property.magrange[0].add(store);
	infile>>tmp; //cerr<<tmp<<endl;
	infile>>store;   property.step1_kmeansproperty.numCluster = store;	
	sprintf(str, "%s/%s/input.txt", property.storeDir, property.rawFile);
	property.step1_kmeansproperty.file1 = strdup(str);
	sprintf(str, "%s/%s/output.txt", property.storeDir, property.rawFile);
	property.step1_kmeansproperty.file2 = strdup(str);

    /*--------------------step 2-------------------------------------------*/
    property.step2_kmeansproperty.dimension = 7;
    property.step2_kmeansproperty.isNormalize = true;
property.step2_kmeansproperty.clusterLayer.free();
property.step2_kmeansproperty.clusterWeight.free();
    for(int i=0;i<flow_field->GetPlaneNum();i++)
    {
        property.step2_kmeansproperty.clusterLayer.add(-1);
    }  
        infile>>tmp;//cerr<<tmp<<endl; 
  	infile>>tmp; //cerr<<tmp<<endl;
	for(int i=0;i<CLUSTER_DIMENSION;i++)
	{
		infile>>store;
		property.step2_kmeansproperty.clusterWeight.add(store);
	}	
	//int index1, index2;
	infile>>tmp;//cerr<<tmp<<endl;
	infile>>index1;
	infile>>index2;
        if (index2>=flow_field->GetPlaneNum()) index2=flow_field->GetPlaneNum()-1;
	for(int i=index1; i<=index2;i++)
	{
		property.step2_kmeansproperty.clusterLayer[i]++;
	}
	infile>>tmp; //cerr<<tmp<<endl;
	infile>>store;  
    isWhole = store;
    if(isWhole)
    {
                 for(int i=index1; i<=index2;i++)
                {
                   property.step2_kmeansproperty.clusterLayer[i]++;
                }
    }
    else
    {
            int count = 1;
             for(int i=index1; i<=index2;i++)
                {
                   property.step2_kmeansproperty.clusterLayer[i]= property.step2_kmeansproperty.clusterLayer[i]+count;
                   count++; 
                }
    }
	infile>>tmp;// cerr<<tmp<<endl;
	infile>>store;  property.magrange[1].add(store);
	infile>>store;  property.magrange[1].add(store);
	infile>>tmp;// cerr<<tmp<<endl;
	infile>>store;   property.step2_kmeansproperty.numCluster = store;	
	sprintf(str, "%s/%s/input.txt", property.storeDir, property.rawFile);
	property.step2_kmeansproperty.file1 = strdup(str);
	sprintf(str, "%s/%s/output.txt", property.storeDir, property.rawFile);
	property.step2_kmeansproperty.file2 = strdup(str);

   // zmin=0;
   // zmax = flow_field->GetPlaneNum()-1;
//----------------------symmetry--------------------
        infile>>tmp;
        infile>>store;
        svVector3 pp;
        svVector3 dd;
        sprintf(str, "%s/%s/", property.storeDir, property.rawFile);
        property.symmetryproperty.datafile= strdup(str);
        //sprintf(str, "%s/symmetry.txt", property.storeDir);
        //property.symmetryproperty.outputfile = new char[200];
        for(int i=0;i<(int)store;i++)
        {
            infile>>pp[0]>>pp[1]>>pp[2];
            infile>>dd[0]>>dd[1]>>dd[2];
            dd.normalize();
            property.symmetryproperty.pos.add(pp);
            property.symmetryproperty.dir.add(dd);
        }
        property.symmetryproperty.planepos = flow_field->GetMinPlane();
        property.symmetryproperty.planedir = flow_field->GetPlaneDir();
        property.symmetryproperty.planedistance = property.plane_distance;
        delete [] str;	
	infile.close();

	/*-------------------------end----------------------------------------*/
	//cerr<<"=>config"<<endl;
	
	/*for(int i=0;i<flow_field->GetPlaneNum();i++)
	{
		for(int j=0;j<contourValues[i].size();j++)
		{
			cerr<<i<<" "<<contourValues[i][j]<<" ";
		}cerr<<endl;
	}*/

       symmetrytype.clear();
       //symmetrytype.push_back(0);
       //symmetrytype.push_back(1);
       //symmetrytype.push_back(2);
       //symmetrytype.push_back(3);
} 
void LoadTexture()
{
    //texture[0] = 1;
    //texture[1] = 2;

  /*  if(textureinit >0) 
    {
       for(int i=0;i<12;i++)
            glDeleteTextures(1, &texture[i]);
       delete [] texture;
       delete [] texture_pos;
       delete [] texture_status;
    }
*/
    texture = new GLuint[SYMMETRY_TYPE_NUM*SYMMETRY_STATUS_NUM];

//    glGenTextures(2, texture);


    char *str = new char[200];
    sprintf(str, "%s/media/figures/antisymmetryup.png",SRC_DIR);
    SOILTexture(str, texture[0]);
    sprintf(str, "%s/media/figures/antisymmetrydown.png",SRC_DIR);
    SOILTexture(str, texture[1]);
    sprintf(str, "%s/media/figures/antisymmetrydown.png",SRC_DIR);
    SOILTexture(str, texture[2]);
    sprintf(str, "%s/media/figures/antisymmetryup.png",SRC_DIR);
    SOILTexture(str, texture[3]);

    sprintf(str, "%s/media/figures/positiveup.png",SRC_DIR);
    SOILTexture(str, texture[4]);
    sprintf(str, "%s/media/figures/positivedown.png",SRC_DIR);
    SOILTexture(str, texture[5]);
    sprintf(str, "%s/media/figures/positivedown.png",SRC_DIR);
    SOILTexture(str, texture[6]);
    sprintf(str, "%s/media/figures/positiveup.png",SRC_DIR);
    SOILTexture(str, texture[7]);

    sprintf(str, "%s/media/figures/negative1up.png",SRC_DIR);
    SOILTexture(str, texture[8]);
    sprintf(str, "%s/media/figures/negative1down.png",SRC_DIR);
    SOILTexture(str, texture[9]);
    sprintf(str, "%s/media/figures/negative1down.png",SRC_DIR);
    SOILTexture(str, texture[10]);
    sprintf(str, "%s/media/figures/negative1up.png",SRC_DIR);
    SOILTexture(str, texture[11]);

    sprintf(str, "%s/media/figures/negative2up.png",SRC_DIR);
    SOILTexture(str, texture[12]);
    sprintf(str, "%s/media/figures/negative2down.png",SRC_DIR);
    SOILTexture(str, texture[13]);
    sprintf(str, "%s/media/figures/negative2down.png",SRC_DIR);
    SOILTexture(str, texture[14]);
    sprintf(str, "%s/media/figures/negative2up.png",SRC_DIR);
    SOILTexture(str, texture[15]);


    delete [] str;

    texture_pos = new svVector3[SYMMETRY_STATUS_NUM];
    texture_status = new int[SYMMETRY_STATUS_NUM];

    texture_status[0] = SYMMETRY_UP;
    texture_status[1] = SYMMETRY_UP;
    texture_status[2] = SYMMETRY_UP;
    texture_status[3] = SYMMETRY_UP;

    LocateTexture();

}

void LocateTexture()
{
    texture_size = 72;//(svScalar)(image_height/15.);
    svScalar size = texture_size;//cerr<<size<<endl;
    texture_pos[0][0]=325;          texture_pos[0][1]=image_height-size*1.2; 
    texture_pos[1][0]=325+size*1.7; texture_pos[1][1]=image_height-size*1.2; 
    texture_pos[2][0]=325+size*3.4; texture_pos[2][1]=image_height-size*1.2; 
    texture_pos[3][0]=325+size*5.1; texture_pos[3][1]=image_height-size*1.2; 

}

void ComputeGLUIFactor()
{
    if(encode_type == LINEAR)
    {
        glui_length_scale = (length_scale / DEFAULT_LINEAR_LENGTH)/5.;
        glui_arrow_scale = (arrow_scale / DEFAULT_ARROW)/5.;
        glui_tube_scale = (tube_scale / DEFAULT_TUBE)/5.;
    }
    else
    {
        glui_length_scale = (length_scale / DEFAULT_SPLITVECTORS_LENGTH)/5.;
        glui_arrow_scale = (arrow_scale / DEFAULT_ARROW)/5.;
        glui_tube_scale = (tube_scale / DEFAULT_TUBE)/5.;
    } 
}

void init(char *configfname)//rbfname, char *cpname)
{

  property.rawDir = new char[200];
  property.rawFile = new char[200];
  property.storeDir = new char[200];


  peeling = new svPeeling;
  peeling->SetDisplayList(100);

  char *str = new char[200];
  sprintf(str,"%s/libs/dual_depth_peeling/shaders/",SRC_DIR);
  peeling->InitGL(str);

  delete [] str;


  mesh = new svMesh();
  outline = new svOutline();
  flow_field = new svQDOT();

  configFile = strdup(configfname);

  configproperty.symmetryproperty.outputfile = new char[200];
  configproperty.symmetryproperty.inputfile = new char[200];

  Config(configfname, configproperty);

  zmin=0;
  zmax = flow_field->GetPlaneNum()-1;

  directglyph = new svDirectArrow(flow_field);//cerr<<"done"<<endl;
  summaryglyph = new svSummaryGlyph(flow_field);

  summaryradius = 0.27;
  summaryVisible = 1; 

  symmetry_enable = 0;
  textureinit = 0;

  mesh_solid_list =3;
  render_list = 1;
  summary_list = 2; 

  Update();

  length_scale = directglyph->GetScale();
  arrow_scale = directglyph->GetRadius();
  tube_scale = directglyph->GetTubeRadius();

  ComputeGLUIFactor();

  LoadTexture();

}

void Update()
{
//  Config(configfname, configproperty);

  vector<int> lists;
  lists.push_back(mesh_solid_list);
  lists.push_back(render_list);
  lists.push_back(summary_list);
  vector<float> opac;
  opac.push_back(0.5);
  opac.push_back(0.5);
  opac.push_back(0.5);

  peeling->SetOpacity(opac);
  peeling->SetDisplayList(lists);

  unique_region.clear();
  unique_region = flow_field->GetUniqueRegion();

  directglyph->New(flow_field, flow_field->GetPlaneNum());
  summaryglyph->New(flow_field, flow_field->GetPlaneNum());

  if(configproperty.isContour)
  {
       directglyph->GenerateContours(configproperty.contourproperty);//cerr<<"done"<<endl;
       summaryglyph->GenerateContours(configproperty.contourproperty); 
  }
  else
   {
         for(int i=0;i<flow_field->GetPlaneNum();i++)
         {
               sprintf(str, "%s/%s/%d.txt", configproperty.storeDir, configproperty.rawFile, i);
	           directglyph->SetData(str, i);
                   summaryglyph->SetData(str, i);
               sprintf(str, "%s/%s/format%d.txt",configproperty.storeDir, configproperty.rawFile, i);
                   directglyph->SetFormat(str,i);
                   summaryglyph->SetFormat(str,i);
         }
        directglyph->SetContourLabel();
        summaryglyph->SetContourLabel();
   }

   directglyph->ResetCluster();
   directglyph->GenerateSymmetry(configproperty.symmetryproperty);//, frequency);
   directglyph->SetXdistance(flow_field->GetXdistance());
   directglyph->SetYdistance(flow_field->GetYdistance());
   directglyph->SetZdistance(flow_field->GetZdistance());
   directglyph->SetSampling(symmetrytype, frequency);
   directglyph->SetVisible(contourindex);
   directglyph->SetVisible(zmin, zmax);
   directglyph->SetROI(configproperty.magrange[0][0], configproperty.magrange[0][1]);
   directglyph->GenerateClusters(configproperty.step1_kmeansproperty);
   directglyph->SetROI(configproperty.magrange[1][0], configproperty.magrange[1][1]);
   directglyph->GenerateClusters(configproperty.step2_kmeansproperty);
   directglyph->SetColorByClusterMag();  
   directglyph->SetDisplayList(render_list);
   directglyph->SetScale(length_scale);
   directglyph->SetRadius(arrow_scale);
   directglyph->SetTubeRadius(tube_scale);
   directglyph->Generate();

   summaryglyph->ResetCluster();
   summaryglyph->GenerateSymmetry(configproperty.symmetryproperty);//, frequency);
   summaryglyph->SetXdistance(flow_field->GetXdistance());
   summaryglyph->SetYdistance(flow_field->GetYdistance());
   summaryglyph->SetZdistance(flow_field->GetZdistance());
   summaryglyph->SetSampling(symmetrytype, frequency);
   summaryglyph->GenerateClusters(directglyph->GetClusterLabels());
   summaryglyph->SetVisible(contourindex);
   summaryglyph->SetVisible(zmin, zmax);
   summaryglyph->SetNumPower(flow_field->GetNumPower());
   summaryglyph->SetScaling(flow_field->GetScaling());
   summaryglyph->SetRadius(summaryradius);
   summaryglyph->SetColorByCluster();
   summaryglyph->SetDisplayList(summary_list);
   summaryglyph->SetScale(summary_length_scale);
   summaryglyph->SetRadius(summary_arrow_scale);
   summaryglyph->SetTubeRadius(summary_tube_scale);
   summaryglyph->Generate(0.5);

   mesh->SetDisplayList(mesh_solid_list);
   sprintf(str, "%s/%s/mesh%d.txt", configproperty.storeDir, configproperty.rawFile, unique_region[0]);
   mesh->GenerateWiresolid(str);
   delete [] str;

  svVector3 center = flow_field->GetCenter();//cerr<<"done"<<endl;
  center.getValue(view_info.coi);

  GLfloat x, y, z;
  flow_field->GetPhysicalDimension(&x,&y, &z);

  view_info.eye[0] = 0;//x/2.0;
  view_info.eye[1] = 0;// y/2.0;
  view_info.eye[2] = z*2.0;

  trackball.setEye(view_info.eye);
  trackball.setFocus(center);
  trackball.setWindowSize(image_width, image_height);
}

//**********************
//// GLUI
////**********************
void glui_display()
{
  glui = GLUI_Master.create_glui_subwindow( window,
                                            GLUI_SUBWINDOW_LEFT );

  new GLUI_StaticText(glui, "Show me");
  
  new GLUI_Checkbox(glui, "Symmetry", &symmetry_enable, SYMMETRY_ID, control_cb);
  new GLUI_Checkbox(glui, "Overview", &summaryVisible, OVERVIEW_ID, control_cb);

  char str[50];

  sprintf(str, "Length");
  GLUI_Panel *panel_length = glui->add_panel("");
  panel_length->set_alignment(GLUI_ALIGN_LEFT);
  GLUI_StaticText *text_line = glui->add_statictext_to_panel(panel_length,str);
  text_line->set_w(50);
  glui->add_column_to_panel(panel_length,true);
  sb_line = new GLUI_Scrollbar(panel_length, "Length", GLUI_SCROLL_HORIZONTAL, &glui_length_scale, LENGTH_ID, control_cb);
  sb_line->set_float_limits(0.,1.);

  sprintf(str, "Arrow");
  GLUI_Panel *panel_arrow = glui->add_panel("");
  panel_arrow->set_alignment(GLUI_ALIGN_LEFT);
  GLUI_StaticText *text_arrow = glui->add_statictext_to_panel(panel_arrow,str);
  text_arrow->set_w(50);
  glui->add_column_to_panel(panel_arrow,true);
  sb_arrow = new GLUI_Scrollbar(panel_arrow, "Arrow", GLUI_SCROLL_HORIZONTAL, &glui_arrow_scale, ARROW_ID, control_cb);
  sb_arrow->set_float_limits(0.,1.);

  sprintf(str, "Tube");
  GLUI_Panel *panel_tube = glui->add_panel("");
  panel_tube->set_alignment(GLUI_ALIGN_LEFT);
  GLUI_StaticText *text_tube = glui->add_statictext_to_panel(panel_tube,str);
  text_tube->set_w(50);
  glui->add_column_to_panel(panel_tube,true);
  sb_tube = new GLUI_Scrollbar(panel_tube, "Tube", GLUI_SCROLL_HORIZONTAL, &glui_tube_scale, TUBE_ID, control_cb);
  sb_tube->set_float_limits(0.,1.);

  GLUI_EditText *text_freq = new GLUI_EditText(glui, "Sampling", &frequency, FREQ_ID, control_cb);

  GLUI_Panel *panel_mesh = glui->add_panel("");
  panel_mesh->set_alignment(GLUI_ALIGN_LEFT);
  new GLUI_Checkbox(panel_mesh, "Mesh",&mesh_enable, MESH_ID,control_cb);
  list_mesh_type = glui->add_listbox_to_panel(panel_mesh, "Type", 
                                        &mesh_type, MESH_TYPE_ID, control_cb);
  for(int i=0;i<unique_region.size();i++)
  {
     sprintf(str, "%d",unique_region[i]);
     list_mesh_type->add_item(i,str);
  }
  glui->add_column_to_panel(panel_mesh, true); 
  GLUI_Listbox *list_mesh_vis = glui->add_listbox_to_panel(panel_mesh, "Show",
                                        &mesh_vis, MESH_VIS_ID, control_cb);
  list_mesh_vis->add_item(0, "Wireframe");
  list_mesh_vis->add_item(1, "Surface");

/*  GLUI_Panel *panel_show = glui->add_panel("Visibility");
  panel_show->set_alignment(GLUI_ALIGN_LEFT);
  sprintf(str, "Magnitude: %6.1e - %6.1e", mag_show, mag_max);
  GLUI_StaticText *text_mag = glui->add_statictext_to_panel(panel_show,str);
  GLUI_Panel *panel_mag = glui->add_panel_to_panel(panel_show, "");
  panel_mag->set_alignment(GLUI_ALIGN_LEFT);
  sprintf(str, "%6.1e", mag_min);
  text_mag_min = glui->add_statictext_to_panel(panel_mag,str);
  text_mag_min->set_w(50);
  glui->add_column_to_panel(panel_mag,true);
  sb_mag = new GLUI_Scrollbar(panel_mag, "Magnitude", GLUI_SCROLL_HORIZONTAL, &glui_mag_show, MAG_ID, control_cb);
  sb_mag->set_float_limits(0.,1.);
  glui->add_column_to_panel(panel_mag,true);
  sprintf(str, "%6.1e", mag_max);
  text_mag_max = glui->add_statictext_to_panel(panel_mag,str);
  text_mag_max->set_w(50);
  GLUI_EditText *text_layer = new GLUI_EditText(panel_show, "Number of layers", &znum, LAYER_NUM_ID, control_cb); 
  GLUI_Panel *panel_layer = glui->add_panel_to_panel(panel_show, "Layers");
  panel_layer->set_alignment(GLUI_ALIGN_LEFT);
  sprintf(str, "%d", layer_min);
  text_layer_min = glui->add_statictext_to_panel(panel_layer,str);
  text_layer_min->set_w(50);
  glui->add_column_to_panel(panel_layer,true);
  sb_layer = new GLUI_Scrollbar(panel_layer, "", GLUI_SCROLL_HORIZONTAL, &glui_layer_show, LAYER_ID, control_cb);
  sb_layer->set_float_limits(0.,1.);
  glui->add_column_to_panel(panel_layer,true);
  sprintf(str, "%d", layer_max);
  text_layer_max = glui->add_statictext_to_panel(panel_layer,str); 
  text_layer_max->set_w(50);
*/
}


//**********************
// program entry
//**********************

int main(int argc, char** argv)
{
//  srand(12345);
  
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);


        image_width  = IMAGE_WIDTH;
        image_height = IMAGE_HEIGHT;

        glutInitWindowSize(image_width, image_height);
        window = glutCreateWindow("QDOT");
        glutInitWindowPosition(0, 0);
        if (glewInit() != GLEW_OK)
        {
                printf("glewInit failed. Exiting...\n");
                exit(1);
        }

/*
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
*/
//        glfwWindowHint(GLFW_SAMPLES, 4);

//        glutDisplayFunc(display);
/*        glutReshapeFunc(reshape);
        glutKeyboardFunc(key);
        glutMouseFunc(mouse);
        glutMotionFunc(motion);
*/
  
        GLUI_Master.set_glutReshapeFunc(reshape);

        if(argc == 2)
        {
            init(argv[1]);
        }
        else
        {
            exit(0);
        }

        glutDisplayFunc(display);
        GLUI_Master.set_glutKeyboardFunc(key);
        GLUI_Master.set_glutSpecialFunc(NULL);
        GLUI_Master.set_glutIdleFunc(idle);
        GLUI_Master.set_glutMouseFunc(mouse);
        glutMotionFunc(motion);
        glutPassiveMotionFunc(movement);   

         glui_display();
 
        glutMainLoop();
        return 0;
}
