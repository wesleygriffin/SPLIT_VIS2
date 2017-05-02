
#include "dual_depth_peeling.h"
#include <GL/glext.h>
#ifndef  GL_RG32F
#define GL_RG32F 0x8230
#endif

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
//#include <GLFW/glfw3.h>
//#include "svVectorField.h"
#include "svQDOT.h"
#include "svDirectArrow.h"
#include "svSplitArrow.h"
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

#define SPLITVECTORS_SCALE 0.5
#define LINEAR_SCALE  50000
#define ARROW_SCALE 0.1
#define SUMMARY_ARROW_SCALE 0.75
#define TUBE_SCALE 0.05
#define SUMMARY_TUBE_SCALE 0.325

#define SYMMETRY_UP 0
#define SYMMETRY_DOWN 1
#define SYMMETRY_UP_SELECT 2
#define SYMMETRY_DOWN_SELECT 3
#define SYMMETRY_TYPE_NUM 4
#define SYMMETRY_STATUS_NUM 4
#define CLUSTER_DIMENSION 7

#define VISUAL_TYPE 3
#define LINEAR 0
#define LC 1
#define LT 2

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
#define UPDATE_ID 14
#define ENCODE_ID 13
#define ALPHA_ID 15
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
GLUI_Scrollbar *sb_alpha;
GLUI_Listbox *list_mesh_type;
GLUI_Listbox *list_mesh_vis;
GLUI_StaticText *text_mag_min;
GLUI_StaticText *text_mag_max;
GLUI_StaticText *text_layer_min;
GLUI_StaticText *text_layer_max;
//=========END================


view3d view_info;

//svPeeling *peeling;
svQDOT *flow_field;
svDirectArrow *directglyph;
svSplitArrow *splitglyph;
svSummaryGlyph *summaryglyph;
svOutline *outline;
svMesh *mesh;

char *configFile;


static GLuint *texture;//[4*3];
svVector3 *texture_pos;
svScalar texture_size;
svScalar texture_visual_size;
svVector3 *texture_visual_pos;
int *texture_status;
int *texture_visual_status;
static GLuint *texture_visual;
int textureinit;
vector<int> symmetrytype;
int symmetry_enable;


//===============
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
int mesh_type = 0;
vector<int> unique_region;
int mesh_vis = 1;

int  mesh_solid_list;
int  outline_display_list;
int  summary_list;
int  render_list;

float alpha = 0.5;

svScalar mag_show;
svScalar mag_max=1e-4;
svScalar mag_min=1e-16;
float glui_mag_show=0.5;

int znum;
int layer_min=0;
int layer_max=108;
int layer_show=50;
float glui_layer_show=0.5;

int encode_visible = 0;
int encode_type = LC;
///////////////////

int frequency =1;
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
GLfloat m[16];

double zmin;
double zmax;
double zshuffle;

//=========================Depth Peeling============
int g_numPasses;
int g_imageWidth;
int g_imageHeight;


nv::Model *g_model;
GLuint g_quadDisplayList;
GLuint g_vboId;
GLuint g_eboId;

bool g_useOQ;
GLuint g_queryId;

static nv::SDKPath sdkPath;

GLSLProgramObject g_shaderDualInit;
GLSLProgramObject g_shaderDualPeel;
GLSLProgramObject g_shaderDualBlend;
GLSLProgramObject g_shaderDualFinal;

vector<float> g_opacity;
bool g_showOsd;
bool g_bShowUI;
unsigned g_numGeoPasses;

int g_oldX, g_oldY;
int g_newX, g_newY;

float g_white[3];
float g_black[3];
float *g_backgroundColor;

GLuint g_dualBackBlenderFboId;
GLuint g_dualPeelingSingleFboId;
GLuint g_dualDepthTexId[2];
GLuint g_dualFrontBlenderTexId[2];
GLuint g_dualBackTempTexId[2];
GLuint g_dualBackBlenderTexId;

GLenum g_drawBuffers[7];

void MakeFullScreenQuad();
void InitDualPeelingRenderTargets();
void DeleteDualPeelingRenderTargets();
void BuildShaders(string SHADER_PATH);
void InitGL(string shader);
void RenderDualPeeling();

//===================================================


void ArrowLight();
void glui_display();
void VisualMapping();

void InitLight()
{
 
  //setting of lighting
  GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8,1};
  GLfloat mat_specular[] = { 0.2,0.2,0.2,0.2 };
  //GLfloat mat_shininess[] = { 65 };
  //GLfloat light_position[] = { 24, 24, 60, 0.0 };
  GLfloat light_position[] = { 100,100,200, 0 };
  GLfloat white_light[] = { 0.8, 0.8, 0.8, 1.0 };
 // glClearColor(0.0, 0.0, 0.0, 0.0);
 // glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
 // glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
//  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

  GLfloat lmodel_ambient[] = {0.2,0.2,0.2,1.0};
  GLfloat local_view[] = {0.0};
//  GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient);
  glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
/* 
  GLfloat mat_spcular[] = {1.0,1.0,1.0,1.0};
  GLfloat mat_shininess[] = {50.};
  GLfloat light_position[] = {0,50,50,0};
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat lmodel_ambient[] = {0.5,0.5,0.5,1.0};
  GLfloat local_view[] = {0.0};
  GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
//  glShadeModel(GL_SMOOTH);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR,white_light);
 // glLightfv(GL_LIGHT0, GL_SHININESS, mat_shininess);
//  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient);
//  glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
  GLfloat light1_position[] = {-50,0,0,1};
  GLfloat mat_specular1[] = {0.2,0.2,0.2,1.0};
  GLfloat mat_shininess1[] = {20.};
  GLfloat white_light1[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat ambient1[] = {0.5, 0.5, 0.5, 1.0};
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light1);
  glLightfv(GL_LIGHT1, GL_SPECULAR,mat_specular1);
 // glLightfv(GL_LIGHT1, GL_SHININESS, mat_shininess1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
  glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
  glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
  glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_position);
  glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,2.0);
//  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient);
//  glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

*/
  GLfloat light1_position[] = {0,0,100,0};
  GLfloat light1_ambient[] = {0.2,0.2,0.2,1};
  GLfloat light1_diffuse[] = {0.5,0.5,0.5,1.0};
  GLfloat light1_specular[] = {0.5,0.5,0.5,1.0};
//  GLfloat spot_direction[] = {0.0,-0.0,-1.0};
  glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR,light1_specular);

/*
  GLfloat light1_position[] = {0,0,100,1};
  GLfloat light1_ambient[] = {0.2,0.2,0.2,1};
  GLfloat light1_diffuse[] = {1.0,1.0,1.0,1.0};
  GLfloat light1_specular[] = {1.0,1.0,1.0,1.0};
  GLfloat spot_direction[] = {0.0,-0.0,-1.0};
  glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR,light1_specular);
  glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
  glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
  glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);

  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
  glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,2.0);
*/  
 ArrowLight();
}


void ArrowLight()
{
  GLfloat mat_ambient[] = {1,1,1,1};
  GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, .8};
  GLfloat mat_specular[] = { 0.2,0.2,0.2,0.2 };
  GLfloat mat_shininess[] = { 50 };
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}
//***************************
// GLUT callback functions
//****************************

/*
void reshape(int w, int h)
{

    //peeling->Reshape(w,h);
    //peeling-> DeleteDualPeelingRenderTargets();
    //peeling-> InitDualPeelingRenderTargets();

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
}*/

void DrawSymmetryButtons(void)
{
       glEnable (GL_BLEND); 
       glEnable(GL_TEXTURE_2D);

//     LoadTexture();
       glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       glPushMatrix();
     //glTranslatef(500,0,0);
     //glScalef(100, 100, 100);
     
      
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
       
       glBindTexture(GL_TEXTURE_2D, 0);

    //   glDisable(GL_TEXTURE_2D);
  //     glDisable(GL_BLEND); 
}
void DrawVisibleButtons(void)
{
       glEnable (GL_BLEND);
       glEnable(GL_TEXTURE_2D);

       glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       glPushMatrix();
      
       for(int i=0;i<VISUAL_TYPE;i++)
       {
          int index1 = texture_visual_status[i]+i*SYMMETRY_STATUS_NUM;
          glBindTexture(GL_TEXTURE_2D, texture_visual[index1]);
          glBegin(GL_QUADS);
          glTexCoord2d(0,0);
          glVertex2f(texture_visual_pos[i][0],texture_visual_pos[i][1]+texture_visual_size);
          glTexCoord2d(1,0);
          glVertex2f(texture_visual_pos[i][0]+1.7*texture_visual_size,
                          texture_visual_pos[i][1]+texture_visual_size);
          glTexCoord2d(1,1);
          glVertex2f(texture_visual_pos[i][0]+1.7*texture_visual_size,texture_visual_pos[i][1]);
          glTexCoord2d(0,1);
          glVertex2f(texture_visual_pos[i][0],texture_visual_pos[i][1]);
          glEnd();
       }//cerr<<endl;

       glPopMatrix();

       glBindTexture(GL_TEXTURE_2D, 0);
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
             VisualMapping();
             summaryglyph->SetScale(length_scale);
             summaryglyph->Generate(1);
        }
        glutPostRedisplay();
}

void Generate()
{
    
        if(glIsList(outline_display_list))
                glDeleteLists(outline_display_list, 1);
    glNewList(outline_display_list, GL_COMPILE);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);

        directglyph->DrawOutline();
        outline->DrawXYZ(directglyph->GetLb(), directglyph->GetRb());
       // outline->DrawAxis_scale(directglyph->GetLb(), directglyph->GetRb(),image_width);
 
  glEnable(GL_BLEND);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHT1);
   glEnable(GL_TEXTURE_2D);
     glEndList();
}


int ACSIZE=2;
typedef struct 
{
	GLfloat x, y;
} jitter_point;

#define MAX_SAMPLES  66

/* 8 jitter points */
jitter_point j8[] =
{
	{-0.334818,  0.435331},
	{ 0.286438, -0.393495},
	{ 0.459462,  0.141540},
	{-0.414498, -0.192829},
	{-0.183790,  0.082102},
	{-0.079263, -0.317383},
	{ 0.102254,  0.299133},
	{ 0.164216, -0.054399}
};


/* accFrustum()
 * The first 6 arguments are identical to the glFrustum() call.
 *  
 * pixdx and pixdy are anti-alias jitter in pixels. 
 * Set both equal to 0.0 for no anti-alias jitter.
 * eyedx and eyedy are depth-of field jitter in pixels. 
 * Set both equal to 0.0 for no depth of field effects.
 *
 * focus is distance from eye to plane in focus. 
 * focus must be greater than, but not equal to 0.0.
 *
 * Note that accFrustum() calls glTranslatef().  You will 
 * probably want to insure that your ModelView matrix has been 
 * initialized to identity before calling accFrustum().
 */
void accFrustum(GLdouble left, GLdouble right, GLdouble bottom, 
   GLdouble top, GLdouble near, GLdouble far, GLdouble pixdx, 
   GLdouble pixdy, GLdouble eyedx, GLdouble eyedy, GLdouble focus)
{
   GLdouble xwsize, ywsize; 
   GLdouble dx, dy;
   GLint viewport[4];

   glGetIntegerv (GL_VIEWPORT, viewport);
	
   xwsize = right - left;
   ywsize = top - bottom;
	
   dx = -(pixdx*xwsize/(GLdouble) viewport[2] + eyedx*near/focus);
   dy = -(pixdy*ywsize/(GLdouble) viewport[3] + eyedy*near/focus);
	
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum (left + dx, right + dx, bottom + dy, top + dy, near, far);
   gluLookAt(view_info.eye[0],view_info.eye[1],view_info.eye[2],
                                view_info.coi[0],view_info.coi[1],view_info.coi[2],
                                0, 1, 0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
 //  glTranslatef (-eyedx, -eyedy, 0.0);
}

/* accPerspective()
 * 
 * The first 4 arguments are identical to the gluPerspective() call.
 * pixdx and pixdy are anti-alias jitter in pixels. 
 * Set both equal to 0.0 for no anti-alias jitter.
 * eyedx and eyedy are depth-of field jitter in pixels. 
 * Set both equal to 0.0 for no depth of field effects.
 *
 * focus is distance from eye to plane in focus. 
 * focus must be greater than, but not equal to 0.0.
 *
 * Note that accPerspective() calls accFrustum().
 */
void accPerspective(GLdouble fovy, GLdouble aspect, 
   GLdouble near, GLdouble far, GLdouble pixdx, GLdouble pixdy, 
   GLdouble eyedx, GLdouble eyedy, GLdouble focus)
{
   GLdouble fov2,left,right,bottom,top;

   fov2 = ((fovy*M_PI) / 180.0) / 2.0;

   top = near / (cos(fov2) / sin(fov2));
   bottom = -top;

   right = top * aspect;
   left = -right;

   accFrustum (left, right, bottom, top, near, far,
               pixdx, pixdy, eyedx, eyedy, focus);
}

/*  Initialize lighting and other values.
 */
void Init(void)
{
   InitLight();

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHT1);

   //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

   glEnable(GL_DEPTH_TEST);
  // glShadeModel (GL_FLAT);

   glEnable(GL_NORMALIZE);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);  
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); 

}

void accDisplay(void)
{
   //InitLight();
//   glEnable(GL_LIGHTING);
//   glEnable(GL_LIGHT0);
//   glEnable(GL_LIGHT1);

   //glEnable(GL_MULTISAMPLE);

   //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   float aspect = view_info.aspect_ratio * float(image_width)/float(image_height);
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

     //  GLfloat m[16];

       trackball.getMatrix().getValue(m);

       int jitter;
       glClear(GL_ACCUM_BUFFER_BIT);
      // glClearAccum(0.5, 0.5, 0.5, 1.0);
       for (jitter = 0; jitter < ACSIZE; jitter++) {
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      // glClearColor(0.5, 0.5, 0.5, 1.0);
       accPerspective (GLfloat_fov,
                       aspect,
                      view_info.hither,
                      view_info.yon, 
                     j8[jitter].x, j8[jitter].y, 0, 0, 50.0);

        glPushMatrix();
        glMultMatrixf(m);
        RenderDualPeeling();
       // splitglyph->RenderVBO();
        //summaryglyph->Render();
        //directglyph->Render();
        //peeling->RenderDualPeeling();
        //glEnable(GL_DEPTH_TEST);
        //summaryglyph->RenderFont();
        glPopMatrix();
 
        glAccum(GL_ACCUM, 1.0/ACSIZE);
      }
      glAccum (GL_RETURN, 1.0);


//================2D Visualization======================================================
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
	glTranslatef(image_width-150,image_height-350,0);
        if(summaryVisible)
            summaryglyph->RenderColor();
        glPopMatrix();

        glColor3f(1,1,1);
        //glEnable(GL_DEPTH_TEST);
      //  glDisable(GL_LIGHTING);
        //glEnable(GL_TEXTURE_2D);
        if(symmetry_enable)
                DrawSymmetryButtons();
        else if(encode_visible)
                DrawVisibleButtons();
        //glDisable(GL_TEXTURE_2D);
       // glEnable(GL_LIGHTING);

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixd(viewproperty.projmatrix);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixd(viewproperty.mvmatrix);

      glFlush();
      glutSwapBuffers();
}

void Reshape(int w, int h)
{
   g_imageWidth=w;g_imageHeight=h;

   glClearColor(0., 0., 0., 1);
   glClearAccum(0, 0., 0., 1);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClear(GL_ACCUM_BUFFER_BIT);

   glViewport(0, 0, (GLsizei) w, (GLsizei) h);

   image_width = w;
   image_height = h;

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

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
//cerr<<aspect<<" "<<GLfloat_fov<<endl;
    gluPerspective(GLfloat_fov,
                                           aspect,
                                           view_info.hither,
                                           view_info.yon);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(view_info.eye[0],view_info.eye[1],view_info.eye[2],
                                view_info.coi[0],view_info.coi[1],view_info.coi[2],
                                0, 1, 0);

   //peeling->Reshape(w,h);
   DeleteDualPeelingRenderTargets();
   InitDualPeelingRenderTargets();

   //InitLight();
   LocateTexture(); 
}
/*
void display(void)
{
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE); 
        
        ViewProperty viewproperty;

        glGetDoublev (GL_MODELVIEW_MATRIX, viewproperty.mvmatrix);
        glGetDoublev (GL_PROJECTION_MATRIX, viewproperty.projmatrix);
        glGetIntegerv( GL_VIEWPORT, viewproperty.viewport );


        glClearColor(0.5, 0.5, 0.5, 1);
        glClearAccum(0.5, 0.5, 0.5, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glEnable(GL_TEXTURE_2D);
        //glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_MULTISAMPLE); 
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

     


        //peeling->InitDualPeelingRenderTargets();

        //glPushMatrix();
       // glMultMatrixf(m);

        //glColor3f(0,0,0);

        //peeling->RenderDualPeeling();


        //glEnable(GL_MULTISAMPLE); 
         //glPopMatrix();

        //glDisable(GL_TEXTURE_RECTANGLE_ARB);
        //glDisable(GL_TEXTURE_2D);
        //glEnable(GL_DEPTH_TEST);


        //glPushMatrix();
        //glMultMatrixf(m);
        //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // back to default

        //glClearColor(0.5, 0.5, 0.5, 1);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //directglyph->Render();

        //glDisable(GL_BLEND);
        //glDisable(GL_LIGHTING);
        //glDisable(GL_LIGHT0);
     
        //directglyph->DrawOutline();
        //outline->DrawXYZ(directglyph->GetLb(), directglyph->GetRb());
        //outline->DrawAxis_scale(directglyph->GetLb(), directglyph->GetRb(),image_width);

        //glPopMatrix();


        //glDisable(GL_DEPTH_TEST);
        //glDisable(GL_TEXTURE_2D);
        //glDisable(GL_BLEND);
        //glDisable(GL_LIGHTING);
        //glDisable(GL_LIGHT0);

//=================================

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.0,(GLfloat) viewproperty.viewport[2], 0.0, (GLfloat) viewproperty.viewport[3]);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_LIGHTING);

	glPushMatrix();
	glTranslatef(325,0,0);
        if(summaryVisible)
            summaryglyph->RenderColor();
        glPopMatrix();

        if(symmetry_enable)
                DrawSymmetryButtons();

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixd(viewproperty.projmatrix);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixd(viewproperty.mvmatrix);


        //glFlush();
        glutSwapBuffers();
}
*/

void VisualMapping()
{
          if(encode_type == LINEAR)
          {
               length_scale = (glui_length_scale * LINEAR_SCALE)*10;
               directglyph->SetScale(length_scale);
               arrow_scale = (glui_arrow_scale * ARROW_SCALE)*2.;
               directglyph->SetRadius(arrow_scale);
               tube_scale = (glui_tube_scale * TUBE_SCALE)*2.;
               directglyph->SetTubeRadius(tube_scale);
          }
          else
          {
               length_scale = (glui_length_scale * SPLITVECTORS_SCALE)*5.;
               splitglyph->SetScale(length_scale);
               arrow_scale = (glui_arrow_scale * ARROW_SCALE)*2.;
               splitglyph->SetRadius(arrow_scale);
               tube_scale = (glui_tube_scale * TUBE_SCALE)*2.;
               splitglyph->SetTubeRadius(tube_scale);
          }
   //summaryglyph->SetScale(length_scale);
   //summaryglyph->Generate(0);
   if(encode_type == LINEAR)
   {
      directglyph->SetColorByClusterMag();
      directglyph->Generate();
   }
   else if(encode_type == LC)
    {
        splitglyph->SetColorByPower();
        splitglyph->Generate();
    }
   else if(encode_type == LT)
   {
       splitglyph->SetColorByClusterMag();
       splitglyph->GenerateTexture();
   }
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
   if(configproperty.isContour)
     directglyph->SetSampling(frequency);
   else
     directglyph->SetSampling(symmetrytype, frequency);
   directglyph->SetSymmetryVisible(symmetrytype);
   directglyph->SetVisible(contourindex);
   directglyph->SetVisible(zmin, zmax);

   splitglyph->ResetVisible();
   if(configproperty.isContour)
    splitglyph->SetSampling(frequency);
   else
    splitglyph->SetSampling(symmetrytype, frequency);
   splitglyph->SetSymmetryVisible(symmetrytype);
   splitglyph->SetVisible(contourindex);
   splitglyph->SetVisible(zmin, zmax);

   glutSetWindow(window);
   VisualMapping();

   summaryglyph->ResetVisible();
   if(configproperty.isContour)
     summaryglyph->SetSampling(frequency);
   else
      summaryglyph->SetSampling(symmetrytype, frequency);
   summaryglyph->SetSymmetryVisible(symmetrytype);
   summaryglyph->SetVisible(contourindex);
   summaryglyph->SetVisible(zmin, zmax);
   summaryglyph->Generate(1);

}
void VisibleProcess()
{
   symmetrytype.clear();
   directglyph->ResetVisible();
   if(configproperty.isContour)
     directglyph->SetSampling(frequency);
   else
     directglyph->SetSampling(symmetrytype, frequency);
   directglyph->SetVisible(contourindex);
   directglyph->SetVisible(zmin, zmax);

   splitglyph->ResetVisible();
   if(configproperty.isContour)
    splitglyph->SetSampling(frequency);
   else
    splitglyph->SetSampling(symmetrytype, frequency);
   splitglyph->SetVisible(contourindex);
   splitglyph->SetVisible(zmin, zmax);

   glutSetWindow(window);
   VisualMapping();

   summaryglyph->ResetVisible();
   if(configproperty.isContour)
     summaryglyph->SetSampling(frequency);
   else
      summaryglyph->SetSampling(symmetrytype, frequency);
   summaryglyph->SetVisible(contourindex);
   summaryglyph->SetVisible(zmin, zmax);
   summaryglyph->Generate(1);

}

void PeelingList()
{
     vector<int> list;
     vector<float> opac;
     list.push_back(render_list);
     opac.push_back(alpha);
     list.push_back(outline_display_list);
     opac.push_back(1);
     if(summaryVisible)
     {
          list.push_back(summary_list);
          opac.push_back(alpha);
     }
     if(mesh_enable)
     {
          list.push_back(mesh_solid_list);
          opac.push_back(alpha);
     }
     //peeling->SetOpacity(opac);
     //peeling->SetDisplayList(list);
}

void control_cb(int control)
{
      glutSetWindow(window);
     if(control == SYMMETRY_ID)
     {
         //  cerr<<symmetry_enable<<endl;
           if(symmetry_enable == true)
           {
                SymmetryProcess();
                encode_visible = 0;
           }       
           else
           {
                 VisibleProcess();
           }
     }
     else if(control == OVERVIEW_ID || control == MESH_ID)
     {
          PeelingList();
     }
     else if(control == ENCODE_ID)
     {
        if(encode_visible)symmetry_enable = 0; 
     }
     else if(control == LENGTH_ID || control == ARROW_ID || control == TUBE_ID)
     {
          if(encode_type == LINEAR)
          {
               length_scale = (glui_length_scale * LINEAR_SCALE)*10;   
               directglyph->SetScale(length_scale);
               arrow_scale = (glui_arrow_scale * ARROW_SCALE)*2.;
               directglyph->SetRadius(arrow_scale);
               tube_scale = (glui_tube_scale * TUBE_SCALE)*2.;
               directglyph->SetTubeRadius(tube_scale);
               directglyph->GenerateSample(frequency);      
          }
          else
          {
               length_scale = (glui_length_scale * SPLITVECTORS_SCALE)*5.;
               splitglyph->SetScale(length_scale);  
               arrow_scale = (glui_arrow_scale * ARROW_SCALE)*2.;
               splitglyph->SetRadius(arrow_scale);
               tube_scale = (glui_tube_scale * TUBE_SCALE)*2.;
               splitglyph->SetTubeRadius(tube_scale);
               splitglyph->GenerateSample(frequency);
          }
          summaryglyph->SetScale(length_scale);
         
          arrow_scale_up = true;
     } 
     else if(control == FREQ_ID)
     {
         if(symmetry_enable)
            SymmetryProcess();
         else
            VisibleProcess();

         //VisualMapping();

     }
     else if(control == MESH_TYPE_ID || control == MESH_VIS_ID)
     {
          mesh_type=list_mesh_type->get_int_val();
          mesh_vis=list_mesh_vis->get_int_val();
          if(mesh_vis == 0)
          {
                mesh->GenerateWireframe(unique_region[mesh_type]);
          }
          else
          {
                mesh->GenerateSurface(unique_region[mesh_type]);
          }
     }
     else if(control == ALPHA_ID)
     {
         PeelingList();
     }
  //   cerr<<sb_arrow->ifmouseup()<<endl;
  //   glutSetWindow(window);
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
	case 'H':
        case 'h':
              trackball.reset();
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
        /*case  'l':
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
        case 'z':
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
          */ 
                
        case '\033':
        case 'q':
        case 'Q':
                exit(0);
                break;
        }
 glutPostRedisplay();

//
}
void mouse(int button, int state, int x, int y)
{
    mouse_button = button;
    mouse_state = state;
//cerr<<mouse_button<<" "<<state<<endl;
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
      else if(encode_visible && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
       {
        bool flag = false;
        for(int i=0;i<VISUAL_TYPE;i++)
        {
             if(x > texture_visual_pos[i][0] && x < texture_visual_pos[i][0]+texture_visual_size*1.5
              && image_height-y>texture_visual_pos[i][1]
              && image_height-y<texture_visual_pos[i][1]+texture_visual_size)
            {
                 if(texture_visual_status[i] == SYMMETRY_DOWN_SELECT)
                       texture_visual_status[i] = SYMMETRY_UP;
                 else if (texture_visual_status[i] == SYMMETRY_UP_SELECT)
                 {
                      texture_visual_status[i] = SYMMETRY_DOWN;
                     for(int j=0;j<VISUAL_TYPE;j++)
                     {
                           if(j!=i) texture_visual_status[j]=SYMMETRY_UP;
                     }
                     flag = true;
                     break;
                 }
                 else if(texture_visual_status[i] == SYMMETRY_UP)
                         texture_visual_status[i] = SYMMETRY_DOWN_SELECT;
                 else if(texture_visual_status[i] == SYMMETRY_DOWN)
                         texture_visual_status[i] = SYMMETRY_UP_SELECT;
                 flag = true;
            }
        }
        for(int i=0;i<VISUAL_TYPE;i++)
        {
             if(texture_visual_status[i] == SYMMETRY_DOWN) encode_type = i;
        }
        if(flag) 
         {
               VisualMapping();
               summaryglyph->SetScale(length_scale);
               summaryglyph->Generate(1);
         }
      }

        if (s & Trackball::BUTTON_DOWN){
        trackball.mouseDown(s, x, y);
        glEnable(GL_CULL_FACE);
        ACSIZE = 1;g_numPasses=2;
        }

        if (s & Trackball::BUTTON_UP){
        trackball.mouseUp(s, x, y);
        glDisable(GL_CULL_FACE);
        ACSIZE=4;g_numPasses=4;
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
       if(encode_visible)
       {
           for(int i=0;i<VISUAL_TYPE;i++)
           {
             if(x > texture_visual_pos[i][0] && x < texture_visual_pos[i][0]+texture_visual_size *1.5
              && image_height-y>texture_visual_pos[i][1]
              && image_height-y<texture_visual_pos[i][1]+texture_visual_size)
            {
                 if(texture_visual_status[i] == SYMMETRY_UP)
                         texture_visual_status[i] = SYMMETRY_UP_SELECT;
                 else if(texture_visual_status[i] == SYMMETRY_DOWN)
                         texture_visual_status[i] = SYMMETRY_DOWN_SELECT;
            }
            else
            {
                 if(texture_visual_status[i] == SYMMETRY_UP_SELECT)
                         texture_visual_status[i] = SYMMETRY_UP;
                 else if(texture_visual_status[i] == SYMMETRY_DOWN_SELECT)
                         texture_visual_status[i] = SYMMETRY_DOWN;
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
	infile>>property.isContour;
	infile>>tmp; //cerr<<tmp<<endl;
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
			       for(int j=0;j<flow_field->GetPlaneNum();j++)
			       {
                                float vv = store*flow_field->GetMaxDensitybyLayer(str, j);
			          property.contourproperty.contourValues[j].add(vv);
                                       property.contourproperty.isUpdate[j] = 1;
		          	}
			}
		}
              property.contourname.push_back(cn);//cerr<<property.contourname[i]<<endl;
	}
	sprintf(str, "%s/%s/contour.txt", property.storeDir, property.rawFile);
	property.contourproperty.outputfile = strdup(str);
	sprintf(str, "%s/%s/", property.storeDir, property.rawFile);
	property.contourproperty.vtkdir = strdup(str);
	
/*--------------clustering------------------------------*/
	infile>>tmp;// cerr<<tmp<<endl;
	infile>>tmp;

	property.magrange = new svScalarArray[2];
	
/*-------------step 1-----------------------------------*/
    property.step1_kmeansproperty.dimension = CLUSTER_DIMENSION;
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

       symmetrytype.clear();
} 
void LoadTexture()
{
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glEnable(GL_DEPTH_TEST);

    texture = new GLuint[SYMMETRY_TYPE_NUM*SYMMETRY_STATUS_NUM];

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

    texture_visual = new GLuint[VISUAL_TYPE*SYMMETRY_STATUS_NUM];
    sprintf(str, "%s/media/figures/linearup.png",SRC_DIR);
    SOILTexture(str, texture_visual[0]);
    sprintf(str, "%s/media/figures/lineardown.png",SRC_DIR);
    SOILTexture(str, texture_visual[1]);
    sprintf(str, "%s/media/figures/lineardown.png",SRC_DIR);
    SOILTexture(str, texture_visual[2]);
    sprintf(str, "%s/media/figures/linearup.png",SRC_DIR);
    SOILTexture(str, texture_visual[3]);

    sprintf(str, "%s/media/figures/colorup.png",SRC_DIR);
    SOILTexture(str, texture_visual[4]);
    sprintf(str, "%s/media/figures/colordown.png",SRC_DIR);
    SOILTexture(str, texture_visual[5]);
    sprintf(str, "%s/media/figures/colordown.png",SRC_DIR);
    SOILTexture(str, texture_visual[6]);
    sprintf(str, "%s/media/figures/colorup.png",SRC_DIR);
    SOILTexture(str, texture_visual[7]);

    sprintf(str, "%s/media/figures/textureup.png",SRC_DIR);
    SOILTexture(str, texture_visual[8]);
    sprintf(str, "%s/media/figures/texturedown.png",SRC_DIR);
    SOILTexture(str, texture_visual[9]);
    sprintf(str, "%s/media/figures/texturedown.png",SRC_DIR);
    SOILTexture(str, texture_visual[10]);
    sprintf(str, "%s/media/figures/textureup.png",SRC_DIR);
    SOILTexture(str, texture_visual[11]);

    delete [] str;

    texture_pos = new svVector3[SYMMETRY_STATUS_NUM];
    texture_status = new int[SYMMETRY_STATUS_NUM];
    texture_visual_pos = new svVector3[VISUAL_TYPE];
    texture_visual_status = new int[VISUAL_TYPE];

    texture_status[0] = SYMMETRY_UP;
    texture_status[1] = SYMMETRY_UP;
    texture_status[2] = SYMMETRY_UP;
    texture_status[3] = SYMMETRY_UP;

    texture_visual_status[0] = SYMMETRY_DOWN;
    texture_visual_status[1] = SYMMETRY_UP;
    texture_visual_status[2] = SYMMETRY_UP;

    LocateTexture();

}

void LocateTexture()
{
    texture_visual_size = 72*1.5;
    texture_size = 72;//(svScalar)(image_height/15.);
    svScalar size = texture_size;//cerr<<size<<endl;
    texture_pos[0][0]=325;          texture_pos[0][1]=image_height-size*1.2; 
    texture_pos[1][0]=325+size*1.7; texture_pos[1][1]=image_height-size*1.2; 
    texture_pos[2][0]=325+size*3.4; texture_pos[2][1]=image_height-size*1.2; 
    texture_pos[3][0]=325+size*5.1; texture_pos[3][1]=image_height-size*1.2; 

    size = texture_visual_size;
    texture_visual_pos[0][0]=325;          texture_visual_pos[0][1]=image_height-size*1.2;
    texture_visual_pos[1][0]=325+size*1.7; texture_visual_pos[1][1]=image_height-size*1.2;
    texture_visual_pos[2][0]=325+size*3.4; texture_visual_pos[2][1]=image_height-size*1.2;

}

void ComputeGLUIFactor()
{
    if(encode_type == LINEAR)
    {
        glui_length_scale = (length_scale / LINEAR_SCALE)/10.;
        glui_arrow_scale = (arrow_scale / ARROW_SCALE)/2.;
        glui_tube_scale = (tube_scale / TUBE_SCALE)/2.;
    }
    else
    {
        glui_length_scale = (length_scale / SPLITVECTORS_SCALE)/5.;
        glui_arrow_scale = (arrow_scale / ARROW_SCALE)/2.;
        glui_tube_scale = (tube_scale / TUBE_SCALE)/2.;
    } 
}

void InitDualPeelingRenderTargets()
{
        glGenTextures(2, g_dualDepthTexId);
        glGenTextures(2, g_dualFrontBlenderTexId);
        glGenTextures(2, g_dualBackTempTexId);

        glewExperimental = GL_TRUE;
        glGenFramebuffersEXT(1, &g_dualPeelingSingleFboId);

        for (int i = 0; i < 2; i++)
        {
                glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualDepthTexId[i]);
                glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                 glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RG32F, g_imageWidth, g_imageHeight,
                                         0, GL_RGB, GL_FLOAT, 0);

                glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualFrontBlenderTexId[i]);
                glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, g_imageWidth, g_imageHeight,
                                         0, GL_RGBA, GL_FLOAT, 0);

                glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualBackTempTexId[i]);
                glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, g_imageWidth, g_imageHeight,
                                         0, GL_RGBA, GL_FLOAT, 0);
        }
        glGenTextures(1, &g_dualBackBlenderTexId);
        glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualBackBlenderTexId);
        glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB, g_imageWidth, g_imageHeight,
                                 0, GL_RGB, GL_FLOAT, 0);

        glGenFramebuffersEXT(1, &g_dualBackBlenderFboId);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualBackBlenderFboId);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                                                          GL_TEXTURE_RECTANGLE_ARB, g_dualBackBlenderTexId, 0);

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualPeelingSingleFboId);
        int j = 0;
                glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                                                                  GL_TEXTURE_RECTANGLE_ARB, g_dualDepthTexId[j], 0);
                glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT,
                                                                  GL_TEXTURE_RECTANGLE_ARB, g_dualFrontBlenderTexId[j], 0);
                glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT,
                                                                  GL_TEXTURE_RECTANGLE_ARB, g_dualBackTempTexId[j], 0);

        j = 1;
                glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT3_EXT,
                                                                  GL_TEXTURE_RECTANGLE_ARB, g_dualDepthTexId[j], 0);
                glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT4_EXT,
                                                                  GL_TEXTURE_RECTANGLE_ARB, g_dualFrontBlenderTexId[j], 0);
                glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT5_EXT,
                                                                  GL_TEXTURE_RECTANGLE_ARB, g_dualBackTempTexId[j], 0);

        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT6_EXT,
                                                          GL_TEXTURE_RECTANGLE_ARB, g_dualBackBlenderTexId, 0);
        CHECK_GL_ERRORS;
}
void DeleteDualPeelingRenderTargets()
{
        glDeleteFramebuffersEXT(1, &g_dualBackBlenderFboId);
        glDeleteFramebuffersEXT(1, &g_dualPeelingSingleFboId);
        glDeleteTextures(2, g_dualDepthTexId);
        glDeleteTextures(2, g_dualFrontBlenderTexId);
        glDeleteTextures(2, g_dualBackTempTexId);
        glDeleteTextures(1, &g_dualBackBlenderTexId);
}

void BuildShaders(string SHADER_PATH)
{
        printf("\nloading shaders...\n");

        g_shaderDualInit.attachVertexShader(SHADER_PATH+"dual_peeling_init_vertex.glsl");
        g_shaderDualInit.attachFragmentShader(SHADER_PATH+"dual_peeling_init_fragment.glsl");
        g_shaderDualInit.link();

        g_shaderDualPeel.attachVertexShader(SHADER_PATH+"shade_vertex.glsl");
        g_shaderDualPeel.attachVertexShader(SHADER_PATH+"dual_peeling_peel_vertex.glsl");
        g_shaderDualPeel.attachFragmentShader(SHADER_PATH+"shade_fragment.glsl");
        g_shaderDualPeel.attachFragmentShader(SHADER_PATH+"dual_peeling_peel_fragment.glsl");
        g_shaderDualPeel.link();

        g_shaderDualBlend.attachVertexShader(SHADER_PATH+"dual_peeling_blend_vertex.glsl");
        g_shaderDualBlend.attachFragmentShader(SHADER_PATH+"dual_peeling_blend_fragment.glsl");
        g_shaderDualBlend.link();

        g_shaderDualFinal.attachVertexShader(SHADER_PATH+"dual_peeling_final_vertex.glsl");
        g_shaderDualFinal.attachFragmentShader(SHADER_PATH+"dual_peeling_final_fragment.glsl");
        g_shaderDualFinal.link();

}

void MakeFullScreenQuad()
{
        g_quadDisplayList = glGenLists(1);
        glNewList(g_quadDisplayList, GL_COMPILE);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0.0, 1.0, 0.0, 1.0);
        glBegin(GL_QUADS);
        {
                glVertex2f(0.0, 0.0);
                glVertex2f(1.0, 0.0);
                glVertex2f(1.0, 1.0);
                glVertex2f(0.0, 1.0);
        }
        glEnd();
        glPopMatrix();
        glEndList();
}
typedef struct
{
 float location[4];
 float color[4];
} Vertex;
Vertex verts[6]; // triangle vertices
GLubyte tindices[6]; // triangle vertex indices
GLuint vboHandle[1]; // a VBO that contains interleaved positions and colors
GLuint indexVBO; 

void RenderDualPeeling()
{
/*   glEnable(GL_NORMALIZE);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glDisable(GL_LIGHTING);
*/ 
       glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
 //===================================
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualPeelingSingleFboId);

        glDrawBuffers(2, &g_drawBuffers[1]);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawBuffer(g_drawBuffers[0]);
        glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glBlendEquationEXT(GL_MAX_EXT);

        g_shaderDualInit.bind();
       // splitglyph->Render();
     //   glPushMatrix();
     //   glMultMatrixf(m);
        splitglyph->RenderVBO();
        splitglyph->RenderLegend();
     //   glPopMatrix();
       // display();
        g_numGeoPasses++;
        g_shaderDualInit.unbind();

        CHECK_GL_ERRORS;
//=======================================

        glDrawBuffer(g_drawBuffers[6]);
        glClearColor(g_backgroundColor[0], g_backgroundColor[1], g_backgroundColor[2], 0);
        glClear(GL_COLOR_BUFFER_BIT);

        int currId = 0;

        for (int pass = 1; g_useOQ || pass < g_numPasses; pass++) {
                currId = pass % 2;
                int prevId = 1 - currId;
                int bufId = currId * 3;

                glDrawBuffers(2, &g_drawBuffers[bufId+1]);
                glClearColor(0, 0, 0, 0);
                glClear(GL_COLOR_BUFFER_BIT);

                glDrawBuffer(g_drawBuffers[bufId+0]);
                glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
                glClear(GL_COLOR_BUFFER_BIT);
                glDrawBuffers(3, &g_drawBuffers[bufId+0]);
                glBlendEquationEXT(GL_MAX_EXT);

                g_shaderDualPeel.bind();
                g_shaderDualPeel.bindTextureRECT("DepthBlenderTex", g_dualDepthTexId[prevId], 0);
                g_shaderDualPeel.bindTextureRECT("FrontBlenderTex", g_dualFrontBlenderTexId[prevId], 1);
              /* for(int j=0;j<display_list.size();j++)
               {
                        g_shaderDualPeel.setUniform("Alpha", (float*)&(g_opacity[j]), 1);
                        glCallList(display_list[j]);
               }
*/
                //splitglyph->Render();
        //        glPushMatrix();
          //      glMultMatrixf(m);
                g_shaderDualPeel.setUniform("Alpha", (float*)&(alpha),1);
                splitglyph->RenderVBO();
                splitglyph->RenderLegend();
            //    glPopMatrix();
        //        display();
                g_numGeoPasses++;
                g_shaderDualPeel.unbind();
                CHECK_GL_ERRORS;
                glDrawBuffer(g_drawBuffers[6]);

                glBlendEquationEXT(GL_FUNC_ADD);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                if (g_useOQ) {
                        glBeginQuery(GL_SAMPLES_PASSED_ARB, g_queryId);
                }

                g_shaderDualBlend.bind();
                g_shaderDualBlend.bindTextureRECT("TempTex", g_dualBackTempTexId[currId], 0);
                glCallList(g_quadDisplayList);
                g_shaderDualBlend.unbind();

                CHECK_GL_ERRORS;

                if (g_useOQ) {
                        glEndQuery(GL_SAMPLES_PASSED_ARB);
                        GLuint sample_count;
                        glGetQueryObjectuiv(g_queryId, GL_QUERY_RESULT_ARB, &sample_count);
                        if (sample_count == 0) {
                                break;
                        }
                }
        }

        glDisable(GL_BLEND);

//==============================================
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        glDrawBuffer(GL_BACK);
        g_shaderDualFinal.bind();
        g_shaderDualFinal.bindTextureRECT("DepthBlenderTex", g_dualDepthTexId[currId], 0);
        g_shaderDualFinal.bindTextureRECT("FrontBlenderTex", g_dualFrontBlenderTexId[currId], 1);
        g_shaderDualFinal.bindTextureRECT("BackBlenderTex", g_dualBackBlenderTexId, 2);
        glCallList(g_quadDisplayList);
        g_shaderDualFinal.unbind();


        CHECK_GL_ERRORS;
}


void InitGL(string shader)
{
     g_numPasses = 4;
     g_useOQ = false;
     g_showOsd = true;
     g_bShowUI = true;
     g_numGeoPasses = 0;

     g_white[0]=1;g_white[1]=1;g_white[2]=1;
     g_black[0]=0;g_black[1]=0;g_black[2]=1;

     g_drawBuffers[0] = GL_COLOR_ATTACHMENT0_EXT;
     g_drawBuffers[1] = GL_COLOR_ATTACHMENT1_EXT;
     g_drawBuffers[2] = GL_COLOR_ATTACHMENT2_EXT;
     g_drawBuffers[3] = GL_COLOR_ATTACHMENT3_EXT;
     g_drawBuffers[4] = GL_COLOR_ATTACHMENT4_EXT;
     g_drawBuffers[5] = GL_COLOR_ATTACHMENT5_EXT;
     g_drawBuffers[6] = GL_COLOR_ATTACHMENT6_EXT;
     g_backgroundColor = g_white;

        InitDualPeelingRenderTargets();
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        BuildShaders(shader);
        MakeFullScreenQuad();

        glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHTING);
        glDisable(GL_NORMALIZE);

        glGenQueries(1, &g_queryId);
}

void init(char *configfname)//rbfname, char *cpname)
{
  length_scale = SPLITVECTORS_SCALE;
  arrow_scale = ARROW_SCALE;
  tube_scale = TUBE_SCALE;
  summary_arrow_scale = SUMMARY_ARROW_SCALE;
  summary_tube_scale = SUMMARY_TUBE_SCALE;

  configproperty.rawDir = new char[200];
  configproperty.rawFile = new char[200];
  configproperty.storeDir = new char[200];

  mesh_solid_list =30;
  render_list = 10;
  summary_list = 20;
  outline_display_list = 40;

  //peeling = new svPeeling();
  //peeling->SetDisplayList(100);

  char *str = new char[200];
  sprintf(str,"%s/libs/dual_depth_peeling/shaders/",SRC_DIR);

  InitGL(str);

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
  splitglyph = new svSplitArrow(flow_field);

  summaryVisible = 1; 

  PeelingList();

  symmetry_enable = 0;
  textureinit = 0;

  ComputeGLUIFactor();

  glui_display();

  //if(configproperty.isContour)
    LoadTexture();
      

  Update();

  //length_scale = directglyph->GetScale();
  //arrow_scale = directglyph->GetRadius();
  //tube_scale = directglyph->GetTubeRadius();

//  ComputeGLUIFactor();

//  LoadTexture();
}

void Update()
{
//  Config(configfname, configproperty);
  char *str = new char[400];

  if(encode_type == LINEAR)
       length_scale = LINEAR_SCALE;
  else
       length_scale = SPLITVECTORS_SCALE;

  for(int i=0;i<unique_region.size();i++)
  {
     list_mesh_type->delete_item(i);
  }

  unique_region.clear();
  unique_region = flow_field->GetUniqueRegion();
  sort(unique_region.begin(), unique_region.end());
  
  for(int i=0;i<unique_region.size();i++)
  {
     sprintf(str, "%d",unique_region[i]);
     list_mesh_type->add_item(i,str);
  }
  list_mesh_type->set_int_val(mesh_type);

  directglyph->New(flow_field, flow_field->GetPlaneNum());
  summaryglyph->New(flow_field, flow_field->GetPlaneNum());
  splitglyph->Clean();
  splitglyph->New(flow_field, flow_field->GetPlaneNum());

  if(configproperty.isContour)
  {
       splitglyph->GenerateContours(configproperty.contourproperty);
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
                   splitglyph->SetData(str, i);
               sprintf(str, "%s/%s/format%d.txt",configproperty.storeDir, configproperty.rawFile, i);
                   directglyph->SetFormat(str,i);
                   summaryglyph->SetFormat(str,i);
                   splitglyph->SetFormat(str,i);
         }
        directglyph->SetContourLabel();
        summaryglyph->SetContourLabel();
        splitglyph->SetContourLabel();
        directglyph->GenerateSymmetry(configproperty.symmetryproperty);//, frequency);
        splitglyph->GenerateSymmetry(configproperty.symmetryproperty);//, frequency);
        summaryglyph->GenerateSymmetry(configproperty.symmetryproperty);//, frequency);

   }

   directglyph->ResetCluster();cerr<<"done1"<<endl;
//   directglyph->GenerateSymmetry(configproperty.symmetryproperty);//, frequency);
   directglyph->SetXdistance(flow_field->GetXdistance());cerr<<"done1"<<endl;
   directglyph->SetYdistance(flow_field->GetYdistance());cerr<<"done1"<<endl;
   directglyph->SetZdistance(flow_field->GetZdistance());cerr<<"done1"<<endl;
   if(configproperty.isContour)
     directglyph->SetSampling(frequency);
   else
      directglyph->SetSampling(symmetrytype, frequency);cerr<<"done1"<<endl;
   directglyph->SetVisible(contourindex);cerr<<"done1"<<endl;
   directglyph->SetVisible(zmin, zmax);cerr<<"done1"<<endl;
   directglyph->SetROI(configproperty.magrange[0][0], configproperty.magrange[0][1]);
   directglyph->GenerateClusters(configproperty.step1_kmeansproperty);
   directglyph->SetROI(configproperty.magrange[1][0], configproperty.magrange[1][1]);
   directglyph->GenerateClusters(configproperty.step2_kmeansproperty);
   directglyph->SetColorByClusterMag();  
   directglyph->SetDisplayList(render_list);
   directglyph->SetScale(length_scale);cerr<<"done1"<<endl;
   directglyph->SetRadius(arrow_scale);
   directglyph->SetTubeRadius(tube_scale);
  // directglyph->Generate();
   directglyph->SetNumPower(flow_field->GetNumPower());
   directglyph->SetScaling(flow_field->GetScaling());


   splitglyph->ResetCluster();
  // splitglyph->GenerateSymmetry(configproperty.symmetryproperty);//, frequency);
   splitglyph->SetXdistance(flow_field->GetXdistance());
   splitglyph->SetYdistance(flow_field->GetYdistance());
   splitglyph->SetZdistance(flow_field->GetZdistance());
   if(configproperty.isContour)
     splitglyph->SetSampling(frequency);
   else
     splitglyph->SetSampling(symmetrytype, frequency);
   splitglyph->SetVisible(contourindex);
   splitglyph->SetVisible(zmin, zmax);
   splitglyph->GenerateClusters(directglyph->GetClusterLabels());
   splitglyph->SetColorByClusterMag();
   splitglyph->SetDisplayList(render_list);
   splitglyph->SetScale(length_scale);
   splitglyph->SetRadius(arrow_scale);
   splitglyph->SetTubeRadius(tube_scale);
   splitglyph->SetES(flow_field->GetScaling());
   splitglyph->SetNumPower(flow_field->GetNumPower());
   splitglyph->SetScaling(flow_field->GetScaling());

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0); 
   VisualMapping();

   splitglyph->Update();
   splitglyph->GenerateTubes();
   splitglyph->GenerateArrows();
   splitglyph->GenerateIndex();
   splitglyph->GenerateLegend();
   splitglyph->GenerateVBO();

   summaryglyph->ResetCluster();
   //summaryglyph->GenerateSymmetry(configproperty.symmetryproperty);//, frequency);
   summaryglyph->SetXdistance(flow_field->GetXdistance());
   summaryglyph->SetYdistance(flow_field->GetYdistance());
   summaryglyph->SetZdistance(flow_field->GetZdistance());
   if(configproperty.isContour)
     summaryglyph->SetSampling(frequency);
   else
      summaryglyph->SetSampling(symmetrytype, frequency);
   summaryglyph->GenerateClusters(directglyph->GetClusterLabels());
   summaryglyph->SetVisible(contourindex);
   summaryglyph->SetVisible(zmin, zmax);
   summaryglyph->SetNumPower(flow_field->GetNumPower());
   summaryglyph->SetScaling(flow_field->GetScaling());
   summaryglyph->SetRadius(summaryradius);
   summaryglyph->SetColorByCluster();
   summaryglyph->SetDisplayList(summary_list);
   summaryglyph->SetScale(length_scale);
   summaryglyph->SetRadius(summary_arrow_scale);
   summaryglyph->SetTubeRadius(summary_tube_scale);
   summaryglyph->Generate(0.5);

   mesh->SetDisplayList(mesh_solid_list);
   mesh->New(unique_region);
   for(int i=0;i<unique_region.size();i++)
   {
        sprintf(str, "%s/%s/mesh%d.txt", 
               configproperty.storeDir, 
               configproperty.rawFile, 
               unique_region[i]);
        mesh->SetData(str, i); 
   }
   mesh->GenerateSurface(unique_region[0]);
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

  Generate();
  
}

//**********************
//// GLUI
////**********************
void glui_display()
{
  char str[50];
  glui = GLUI_Master.create_glui_subwindow( window,
                                            GLUI_SUBWINDOW_LEFT );
  new GLUI_StaticText(glui, "Show me");
  
  GLUI_Checkbox *symmetrybox = new GLUI_Checkbox(glui, "Symmetry", &symmetry_enable, SYMMETRY_ID, control_cb);
  new GLUI_Checkbox(glui, "Summary glyph", &summaryVisible, OVERVIEW_ID, control_cb);
  new GLUI_Checkbox(glui, "Visual mapping", &encode_visible, ENCODE_ID, control_cb);
  /// Mesh
  /// 
  if(configproperty.isContour) symmetrybox->disable();
  GLUI_Panel *panel_mesh = glui->add_panel("");
  panel_mesh->set_alignment(GLUI_ALIGN_LEFT);
  new GLUI_Checkbox(panel_mesh, "Mesh",&mesh_enable, MESH_ID,control_cb);
  list_mesh_type = glui->add_listbox_to_panel(panel_mesh, "Region type",
                                        &mesh_type, MESH_TYPE_ID, control_cb);//!!
  for(int i=0;i<unique_region.size();i++)
  {
     sprintf(str, "%d",unique_region[i]);
     list_mesh_type->add_item(i,str);
  }
  //glui->add_column_to_panel(panel_mesh, true);
  list_mesh_vis = glui->add_listbox_to_panel(panel_mesh, "Show",
                                        &mesh_vis, MESH_VIS_ID, control_cb);
  list_mesh_vis->add_item(0, "Wireframe");
  list_mesh_vis->add_item(1, "Surface");

  list_mesh_type->set_int_val(mesh_type);
  list_mesh_vis->set_int_val(mesh_vis);

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
  sb_arrow->set_float_limits(0.01,1.);

  sprintf(str, "Tube");
  GLUI_Panel *panel_tube = glui->add_panel("");
  panel_tube->set_alignment(GLUI_ALIGN_LEFT);
  GLUI_StaticText *text_tube = glui->add_statictext_to_panel(panel_tube,str);
  text_tube->set_w(50);
  glui->add_column_to_panel(panel_tube,true);
  sb_tube = new GLUI_Scrollbar(panel_tube, "Tube", GLUI_SCROLL_HORIZONTAL, &glui_tube_scale, TUBE_ID, control_cb);
  sb_tube->set_float_limits(0.01,1.);

  sprintf(str, "Transparency");
  GLUI_Panel *panel_alpha = glui->add_panel("");
  panel_alpha->set_alignment(GLUI_ALIGN_LEFT);
  GLUI_StaticText *text_alpha = glui->add_statictext_to_panel(panel_alpha,str);
  text_alpha->set_w(50);
  glui->add_column_to_panel(panel_alpha,true);
  sb_alpha = new GLUI_Scrollbar(panel_alpha, "Transparency", GLUI_SCROLL_HORIZONTAL, &alpha, ALPHA_ID, control_cb);
  sb_alpha->set_float_limits(0.,1.);


  GLUI_EditText *text_freq = new GLUI_EditText(glui, "Spin sampling", &frequency, FREQ_ID, control_cb);



//  new GLUI_Button(glui, "Update", UPDATE_ID, control_cb);
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
//glutSetOption(GLUT_MULTISAMPLE, 8);

 /*       glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ACCUM);

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

        GLUI_Master.set_glutReshapeFunc(reshape);

        if(argc == 2)
        {
            init(argv[1]);
        }
        else
        {
            exit(0);
        }

        Init();

        glutDisplayFunc(accDisplay);
        GLUI_Master.set_glutKeyboardFunc(key);
        GLUI_Master.set_glutSpecialFunc(NULL);
        GLUI_Master.set_glutIdleFunc(idle);
        GLUI_Master.set_glutMouseFunc(mouse);
        glutMotionFunc(motion);
        glutPassiveMotionFunc(movement);   

         glui_display();
 
        glutMainLoop();
        return 0;
*/
   image_width  = IMAGE_WIDTH;
   image_height = IMAGE_HEIGHT;

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA
                          | GLUT_DEPTH | GLUT_ACCUM);

        glutInitWindowSize(image_width, image_height);
        window = glutCreateWindow("QDOT");
        glutInitWindowPosition(0, 0);
    
        if (glewInit() != GLEW_OK)
        {
                printf("glewInit failed. Exiting...\n");
                exit(1);
        }


   GLUI_Master.set_glutReshapeFunc(Reshape);
   init(argv[1]);
   Init();
   glutDisplayFunc(accDisplay);

   GLUI_Master.set_glutKeyboardFunc(key);
   GLUI_Master.set_glutSpecialFunc(NULL);
   GLUI_Master.set_glutIdleFunc(idle);
   GLUI_Master.set_glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(movement);  

 //  glui_display();

   glutMainLoop();
   return 0;


}
