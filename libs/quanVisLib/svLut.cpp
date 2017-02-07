/*
 * svLut
 */

#include <iostream>
#include <fstream>
#include "svLut.h"
#include "svUtil.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "../jcUtil/MGL.h"
#include <string.h>

#define  DEFAULT_COLOR_SCHEMA  SV_LOCS
#define  DEFAULT_ENTRY_NUM     256

#define  DEFAULT_MIN_SCALAR    0.01
#define  DEFAULT_MAX_SCALAR    7

#define  DEFAULT_FILE_NAME   "../data/COLOR_LOCS.table1"

namespace __svl_lib {

svLut::svLut()
{
  lutModel = svInt(DEFAULT_COLOR_SCHEMA);
  entry_num = svInt(DEFAULT_ENTRY_NUM);
  min_cv = 0.;
  max_cv = 255;

  min_lineWidth = svScalar(DEFAULT_MIN_SCALAR);
  max_lineWidth = svScalar(DEFAULT_MAX_SCALAR);

  min_lv = 0.1; // default
  max_lv = 1.5;

  alpha = 1.f;
  genTexture = false;
  render_lut = false;

  for(int i=0;i<256;i++) {
    scalarLut[i]=min_lineWidth+ 
            (svFloat)i/255.*(max_lineWidth-min_lineWidth);
  };
}

void svLut::SetColorModel(svInt &newModel)
{
  lutModel = newModel;
  if(newModel==SV_BLUE_YELLOW) {   
    //min_cv = 0; 
    //max_cv = 19; 
    entry_num=20;
  }
 
  SetColorModel(newModel, min_cv, max_cv);
}

void svLut::SetColorModel(svInt &newModel, svScalar lr, svScalar rr)
{
  lutModel = newModel;
  min_cv = lr; max_cv = rr;

  if(newModel == SV_GRAY)
  {
    AddColorSegment(lr,svVector4(0,0,0,1),rr,svVector4(256,256,256,1));
    MakeLOCTexture();
  }
  else if(newModel == SV_LOCS)
  {
    char *fn=strdup("../data/COLOR_LOCS.table");
    Load(fn);
    MakeLOCTexture();
  }
  else if(newModel == SV_BLUE_YELLOW)
  {
    char *fn=strdup("../data/COLOR_BLUE_YELLOW.table");
    Load(fn);
    MakeBlueYellowTexture();
  }
}

void svLut::RenderBlueYellow()
{
  // render it 
  /*
   * This is a simple way to draw it
   * however, we dont' have much control over what else to draw
   glRasterPos2i(300, 300);
   glDrawPixels(LUT_WIDTH, LUT_HEIGHT, GL_RGBA, 
                GL_UNSIGNED_BYTE, lutImage);
		*/
    GLdouble vmat[16], pmat[16];
    GLint    viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
    glGetIntegerv(GL_VIEWPORT, viewport);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,(GLfloat) viewport[2], (GLfloat) viewport[3], 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    //glColor3f(1., 1., 0.);
    //void *font1 = GLUT_BITMAP_HELVETICA_18;
    //text("Color lookup table", font1, (float)100, (float)100);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // set material properties
    glShadeModel(GL_SMOOTH);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D, texName);

   //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex3f(120, 356, 0);
        glTexCoord2f(1,0);
        glVertex3f(100, 356, 0);
        glTexCoord2f(1,1);
        glVertex3f(100, 100, 0);
        glTexCoord2f(0,1);
        glVertex3f(120, 100, 0);
    glEnd();

/*
    glBegin(GL_LINE_LOOP);
        glVertex3f(122, 358, 0);
        glVertex3f(98, 358, 0);
        glVertex3f(98, 98, 0);
        glVertex3f(122, 98, 0);
    glEnd();
    */

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
    glDisable(GL_TEXTURE_2D);
}

void svLut::Render()
{
 //static bool is_front = (strstr(getenv("HOST"),"cs-front")?true:false);
 //if(!is_front) return;
 //if(render_lut == false) return;
  if(lutModel == SV_LOCS)
      RenderLOC();
  else if(lutModel == SV_BLUE_YELLOW)
      RenderBlueYellow();
}

void svLut::MakeBlueYellowTexture()
{
#define LUT_HEIGHT 20
#define LUT_WIDTH  16 
 static GLubyte lutImage[LUT_HEIGHT][LUT_WIDTH][4];

    // Generature the texture
    cerr << "GenLutTexture" << endl;
	char *fn=strdup("../data/COLOR_LOCS.table");
    Load(fn);

    cerr << "SVLUT: " << LUT_HEIGHT<<"-"  << LUT_WIDTH << endl;
    int i,j,c;
    for(i=0; i<LUT_HEIGHT; i++) {
      for(j=0; j<LUT_WIDTH; j++) {
        lutImage[i][j][0]= (GLubyte) (color[i][0]*255.);
        lutImage[i][j][1]= (GLubyte) (color[i][1]*255.);
        lutImage[i][j][2]= (GLubyte) (color[i][2]*255.);
        lutImage[i][j][3]= (GLubyte) (alpha*255.);
      };
    };
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, LUT_WIDTH,
                LUT_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                lutImage);
#undef LUT_HEIGHT
#undef LUT_WIDTH
}

void svLut::MakeLOCTexture()
{
#define LUT_HEIGHT 256
#define LUT_WIDTH  16 

  static GLubyte lutImage[LUT_HEIGHT][LUT_WIDTH][4];

    // Generature the texture
    cerr << "GenLutTexture" << endl;
	char *fn=strdup("../data/COLOR_LOCS.table");
    Load(fn);

    cerr << "SVLUT: " << LUT_HEIGHT<<"-"  << LUT_WIDTH << endl;
    int i,j,c;
    for(i=0; i<LUT_HEIGHT; i++) {
      for(j=0; j<LUT_WIDTH; j++) {
        lutImage[i][j][0]= (GLubyte) (color[i][0]*255.);
        lutImage[i][j][1]= (GLubyte) (color[i][1]*255.);
        lutImage[i][j][2]= (GLubyte) (color[i][2]*255.);
        lutImage[i][j][3]= (GLubyte) (alpha*255.);
      };
    };
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, LUT_WIDTH,
                LUT_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                lutImage);
#undef LUT_HEIGHT
#undef LUT_WIDTH
}

void svLut::RenderLOC()
{
#define LUT_HEIGHT 256
#define LUT_WIDTH  16 
  /*
   * This is a simple way to darw it
   * however, we dont' have much control over what else to draw
   */
   int win_width = glutGet(GLUT_WINDOW_WIDTH);
   int win_height = glutGet(GLUT_WINDOW_HEIGHT);
   int half_ww = win_width/2;
   int half_wh = win_height/2;

    glRasterPos2i(10,10);
   //glRasterPos2i(300, 300);
   //glRasterPos2i(half_ww, 10);
   //glDrawPixels(LUT_WIDTH, LUT_HEIGHT, GL_RGBA, 
                //GL_UNSIGNED_BYTE, lutImage);

    GLdouble vmat[16], pmat[16];
    GLint    viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
    glGetIntegerv(GL_VIEWPORT, viewport);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,(GLfloat) viewport[2], (GLfloat) viewport[3], 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //glColor3f(1., 1., 0.);
    //void *font1 = GLUT_BITMAP_HELVETICA_18;
    //text("Color lookup table", font1, (float)100, (float)100);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // set material properties
    glShadeModel(GL_SMOOTH);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D, texName);

   //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        //glVertex3f(930, 376, 0);
        glVertex3f(30, 140, 0);

        glTexCoord2f(1,0);
        //glVertex3f(900, 376, 0);
        glVertex3f(10, 140, 0);

        glTexCoord2f(1,1);
        //glVertex3f(900, 120, 0);
        glVertex3f(10, 30, 0);

        glTexCoord2f(0,1);
        //glVertex3f(930, 120, 0);
        glVertex3f(30, 30, 0);
    glEnd();

    glColor3f(1,1,1);
    glBegin(GL_LINE_LOOP);
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);

    glDisable(GL_TEXTURE_2D);
#undef LUT_HEIGHT
#undef LUT_WIDTH
}

void svLut::init()
{
}

svScalar svLut::GetScalar(svScalar v, svBool reverse) 
{
  svInt index = svInt(0.5+255*(v-min_lv)/(max_lv-min_lv));
  if(reverse) index = 255 - index;
  //cerr << "svLUT:GetScalar: index= " << index << endl;
  return scalarLut[index];
}

svScalar svLut::GetScalar(svScalar v, 
           svScalar minv, svScalar maxv, svBool reverse) 
{
  svClamp(v, minv, maxv);
  min_lv = minv; max_lv = maxv;

  svInt index = svInt(0.5+255*(v-minv)/(maxv-minv));
  if(reverse) index = 255 - index;
  return scalarLut[index];
}

void svLut::GetScalarRange(svScalar & r1, svScalar & r2) const
{
  r1 = min_lv;  r2 = max_lv; 
}

void svLut::SetScalarRange(svScalar min, svScalar max)
{
   assert(max_lv >= min_lv);
   min_lv = min; max_lv = max;
}

void svLut::SetLineWidthRange(svScalar min, svScalar max)
{
   assert(min<=max);
   max_lineWidth = max; min_lineWidth = min;

   // reset linewidth
   for(int i=0;i<256;i++) {
    scalarLut[i]=min_lineWidth+
           (svFloat)i/255.*(max_lineWidth-min_lineWidth);
   }
}

svVector4 svLut::GetColor(svScalar v)
{
  return GetColor(v, false);
}

svVector4 svLut::GetColor(svScalar v, svScalar new_min_cv, svScalar new_max_cv, svBool reverse) 
{
  assert(new_min_cv <= new_max_cv);
  max_cv = new_max_cv;
  min_cv = new_min_cv;
  return GetColor(v, reverse);
}

svVector4 svLut::GetColorBlYe(svScalar v) 
{
   svClamp(v, min_lv, max_lv);
   int index = (int)((v-min_lv)/(max_lv-min_lv) * 20.);
   return color[index];
}

svVector4 svLut::GetColor(svScalar v, svBool reverse) 
{
  if(lutModel == SV_BLUE_YELLOW)
    return GetColorBlYe(v);

  svInt index = svInt(0.5 + 255*(v-min_cv)/(max_cv-min_cv));
  svClamp(index, 0, 255);
  if(reverse) index=255-index; 
  // reverse the color - the higher speed the lighter the color
  return color[index];
}


void svLut::GetRange(svScalar & r1, svScalar & r2) const
{
  r1 = min_cv;  r2 = max_cv; 
}

void svLut::SetRange(svScalar r1, svScalar r2)
{
  assert(max_cv > min_cv);
  min_cv = r1;  max_cv = r2; 
}

void svLut::AddColorSegment(svScalar r1, svScalar v1, 
                       svScalar r2, svScalar v2, svInt channel)
{
  assert(r1<r2 && r1>=min_cv && r2<= max_cv);
  assert(channel>=0 && channel<=3);

  svClamp(v1, 0, 1);
  svClamp(v2, 0, 1);

  int i = svInt(floor(255. * (r1-min_cv)/(max_cv-min_cv)));
  int j = svInt(floor(255. * (r2-min_cv)/(max_cv-min_cv)));

  if (j==i) j++;

  for (int k=i; k<=j; k++) {
    color[k][channel] = v1 + (v2-v1)*svScalar(k-i)/svScalar(j-i);
  };
}

void svLut::AddColorSegment(svScalar r1, const svVector4 & v1, 
                       svScalar r2, const svVector4 & v2)
{
	AddColorSegment(r1, v1[0], r2, v2[0], SV_RED);
	AddColorSegment(r1, v1[1], r2, v2[1], SV_GREEN);
	AddColorSegment(r1, v1[2], r2, v2[2], SV_BLUE);
	AddColorSegment(r1, v1[3], r2, v2[3], SV_ALPHA);
}

void svLut::Load(char* filename)
{
  ifstream  inf(filename);
  char      lineptr[256];
  int       i=0;
  
  if(inf.fail()) {
    cerr << "SVLUT: file ("<< filename << ")not found\n" << endl;
    exit(-1);
  }
    //cerr << "DEBUG: load color look up table..." << endl;
  while(inf.getline(lineptr, 256)!=NULL)
  {
    svFloat c1,c2,c3;
    assert(i<256);
    sscanf(lineptr, "%f %f %f", &c1,&c2,&c3);
    //TODO: this is a hack for the JET model -- must change
    //color[i][0]=c1/255.; color[i][1]=c2/255.; color[i][2]=c3/255.;
    color[i][0]=c1; color[i][1]=c2; color[i][2]=c3;
    color[i][3]=alpha;
    i++;
  }; // getline
  inf.close();
}

}

