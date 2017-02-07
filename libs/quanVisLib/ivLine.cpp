/*  ivLine
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <string>
#include "ivLine.h"
#include "svException.h"
#include "svUtil.h"

#include <string.h>
#include "readinputsvl.h"

#define DEFAULT_LINE_LENGTH 0.2

namespace __svl_lib {

void text(char* string, void* font, float x, float y)
{
   char* p; // Temp pointer to the string varible passed in

   // Specify the initial position on the screen where the text
   // will be diaplayed
   glRasterPos2f(x, y);

   // Parse the string passed in and display to screen one char at a time
   for (p = string; *p; p++)
      glutBitmapCharacter(font , *p);
}

using namespace std;

ivLine::ivLine(svVector3 _lbbox, svVector3 _rbbox,svChar *inf, svInt _seed) : svPrimitive(_seed)
{
  lbbox=_lbbox;
  rbbox=_rbbox;

  infile=strdup(inf);

  streamline = NULL;
  streamlineColors = NULL;
  streamlineWidth = NULL;

  ng=0; dis=0;

  LABEL_SET = false;
  DOWNSTROKE_SET = false;
  length = DEFAULT_LINE_LENGTH;
}

ivLine::ivLine(svChar *inf, svInt _seed) : svPrimitive(_seed)
{
  lbbox=svVector3(-1, -1,-1);
  rbbox=svVector3(1,1,1);

  infile=strdup(inf);

  streamline = NULL;
  streamlineColors = NULL;
  streamlineWidth = NULL;
  ng=0; dis=0;

  LABEL_SET = false;
  DOWNSTROKE_SET = false;
  length = DEFAULT_LINE_LENGTH;
}

/*
ivLine::ivLine(svVectorField* f, 
              svParticle* sd, svInt sd_num, svUint mode)
  : svPrimitive(f,sd, sd_num, mode)
{
  streamline   = NULL;
  streamlineColors = NULL;
  streamlineWidth = NULL;
}
*/

/*
ivLine::ivLine(svVectorField* f, 
              svVector3 _lbbox, svVector3 _rbbox,
              svParticle* sd, svInt sd_num, svUint mode)
  : svPrimitive(f,sd, sd_num, mode)
{
  lbbox=_lbbox;rbbox=_rbbox;
  field->SetPhysicalDimension(_lbbox,_rbbox);

  streamline   = NULL;
  streamlineColors = NULL;
  streamlineWidth = NULL;
}
*/

ivLine::~ivLine()
{
  cleanStreamLine();
  cleanDisplayList(SV_IMMEDIATE);
}

void ivLine::cleanStreamLine()
{
  if (streamline != NULL) {
    for (int i=0; i<seed_num; i++)
	streamline[i].free();
    delete [] streamline;
  };
  if (streamlineWidth != NULL) {
    for (int i=0; i<seed_num; i++)  
	streamlineWidth [i].free();
    delete [] streamlineWidth;
  }
  if (streamlineColors != NULL) {
    for (int i=0; i<seed_num; i++)  
	streamlineColors[i].free();
    delete [] streamlineColors;
  };
}

void ivLine::BuildDisplayListFromStore()
{
  STILL_UPDATE = true;
  // build the display list
  cleanDisplayList(SV_IMMEDIATE);
  display_list = glGenLists(1);
  glNewList(display_list, GL_COMPILE);
 
  if(DOWNSTROKE_SET)
  {
    glPushMatrix();

    for(int i=0; i<seed_num; i++) {
      svInt ss = streamline[i].size();
      if(num_of_downstroke[i]>ss) // this isn't right
      {
        cerr << "ERROR: ivLine::BuildDisplayListFromStore:num_of_downstroke>ss"<<endl;
        exit(-1);
      };

      svVector3 pos;
  //    if(dis!=0 & ng!=0)
   //   {
        pos = grouped_streamline[i][num_of_downstroke[i]].getValue();
    //  }
     // else
      //{
       // pos = streamline[i][num_of_downstroke[i]].getValue();
      //}; // end if(dis)
      float offset = length*2;
      glColor3f(1, 0, 0);
      glLineWidth(2.0);
        glBegin(GL_LINES);
          glVertex3f(pos[0], pos[1]-offset, pos[2]);
          glVertex3f(pos[0], pos[1]+offset, pos[2]);
        glEnd();
      glLineWidth(1.0);
    };

    glPopMatrix();
  }; // end if(DOWNSTROKE_SET)
  
  glPushMatrix();

  for(int i=0; i<seed_num; i++) {
    glBegin(GL_LINE_STRIP);
      svInt ss = streamline[i].size();
      //glTranslatef(0, 0, 2); 
      for(int j=0; j<ss; j++)
      {
        if(ENABLE_LINEWIDTH) glLineWidth((GLfloat)streamlineWidth[i][j]);
        if(ENABLE_COLOR) glColor4fv(streamlineColors[i][j].getValue());
	else 
	  glColor3fv(render_property.color.getValue());

        //if(dis!=0 & ng!=0)../Preprocessing/svl_3D/bkcs_0_1_2_3.svl ../data/body_referenced_gravity_down_sort_cut_interval

        {
          glVertex3fv(grouped_streamline[i][j].getValue());
        }
        //else
        //{
          //glVertex3fv(streamline[i][j].getValue());
        //}
      }; // loop of ss;
    glEnd();
  };// loop of seed_num

  glPopMatrix();

/*
  glPushMatrix();
  for(int i=0; i<seed_num; i++)
  {
    glColor3f(1, 0, 0);
    glPointSize(5);
    glBegin(GL_POINTS);
      glVertex3f(locx[i], locy[i], 0);
    glEnd();

    glColor3f(0, 0, 1);
    glBegin(GL_POINTS);
      glVertex3f(cenx[i]+locx[i], ceny[i]+locy[i], cenz[i]);
    glEnd();
    glPointSize(1);

    glLineWidth(1.0);
    glColor3f(0.8, 0.8, 0);
    glBegin(GL_LINES);
      glVertex3f(locx[i], locy[i], 0);
      glVertex3f(cenx[i]+locx[i], ceny[i]+locy[i], cenz[i]);
    glEnd();
  }
  glPopMatrix();
*/

  glEndList();

  display_mode = SV_DISPLAYLIST;
  cerr << "STREAMLINE: display list created" << endl;
  STILL_UPDATE = false;
}

void ivLine::SetCoordAndLabels(int nf, float *_locx, float *_locy, float sx, float sy, 
           char ** _labels, char ** _speed, char ** _mass)
{
  num_of_items = nf;
  locx = new float[nf];
  locy = new float[nf];

  for(int i=0; i<nf; i++) 
  {
    locx[i]=_locx[i]*sx;
    locy[i]=_locy[i]*sy;
  }

  label = new char * [nf];
  speed = new char*[nf];
  mass = new  char*[nf];
  for(int i=0; i<nf; i++)
  {
    label[i]= strdup(_labels[i]);
    speed[i]=strdup( _speed[i]);
    mass[i]=strdup( _mass[i]);
  };

  LABEL_SET = true;
}


void ivLine::SetCoordAndLabels(int nf, float *_locx, float *_locy, float sx, float sy, char ** _labels)
{
  num_of_items = nf;
  locx = new float[nf];
  locy = new float[nf];

  for(int i=0; i<nf; i++) 
  {
    locx[i]=_locx[i]*sx;
    locy[i]=_locy[i]*sy;
  }

  label = new char * [nf];
  for(int i=0; i<nf; i++)
  {
    label[i]= strdup(_labels[i]);
  };

  LABEL_SET = true;
}

void ivLine::GroupStreamlines()
{
  int streamlineCounter=0;
 
  for(int i=0; i<seed_num; i++)
  {
    int ss=streamline[i].size();
    for(int j=0; j<ss; j++) 
    {
      //grouped_streamline[i][j][0]= streamline[i][j][0]+locx[i]; 
      //grouped_streamline[i][j][1]= streamline[i][j][1]+locy[i]; 
      grouped_streamline[i][j][0]= streamline[i][j][0]/*+nfg[i]*dis*/; 
      grouped_streamline[i][j][1]= streamline[i][j][1]+nfg[i]*dis; 
    }; // end for(j)
  };// end for(i)
}

/*
void ivLine::GroupStreamlines()
{
  int streamlineCounter=0;
 
  for(int i=0; i<seed_num; i++)
  {
    int ss=streamline[i].size();
      int k;
      bool found=false;

      // find which group "i" is in
      for(k=0; k<ng-1; k++)
      {
        if(i>=nfg[k] &&  i<nfg[k+1])
        {
          found=true;
          break;
        }; // end if
      }; // end for(k)
      if (k== ng-2 && found!=true) // the last group
        k=ng-1;

      // line up along one direction
      //float cur_dis = dis*k;
      //grouped_streamline[i][j][1]= streamline[i][j][1]+cur_dis; 

      // Dec 22: change to grid
      //int cx = (int)((float)(k)/(float)num_in_line-0.5); // find the location on the grid
      //int cy = (int)( k - cx * num_in_line);
      int cx = k/num_in_line;
      int cy = k%num_in_line;
      cerr << "k = " << k << "cx = " << cx << " cy = " << cy <<  endl;
      float cur_dis_x = dis*cx;
      float cur_dis_y = dis*cy;

    for(int j=0; j<ss; j++) 
    {
      grouped_streamline[i][j][0]= streamline[i][j][0]+cur_dis_x; 
      grouped_streamline[i][j][1]= streamline[i][j][1]+cur_dis_y; 
    }; // end for(j)
  };// end for(i)
}
*/

void ivLine::GenerateFromFile()
{
 svPrimitive::STILL_UPDATE = true;

 cleanStreamLine();

 streamline = new svVector3Array[seed_num];
 grouped_streamline = new svVector3Array[seed_num];
 streamlineColors = new svVector4Array[seed_num];

  cenx = new float[seed_num];
  ceny = new float[seed_num];
  cenz = new float[seed_num];

 //readinputsvl2(infile, streamline, streamlineColors, &seed_num);
 readinputsvl4(infile, streamline, streamlineColors, &seed_num, 
               cenx, ceny, cenz  );
 
 // for the bat program
 // move the starting point to 0
 for(int i=0; i<seed_num; i++)
 {
   int ss = streamline[i].size(), j;
   cerr << i << ": " << cenx[i]<<" "<<ceny[i]<<" "<<cenz[i]<<endl;


   svVector3 firstPoint = streamline[i][0].getValue();
   for(int j=0; j<ss; j++) {
     //svVector3 curV = streamline[i][j].getValue();
     //streamline[i][j][1]=streamline[i][j][1]-firstPoint[1];

     // initialize grouped_streamline
     svVector3  v = streamline[i][j].getValue();
     grouped_streamline[i].add(v);
   };// end for(j)
 };// end for(i)

 // then update them to groups
 //if(dis!=0 && ng!=0)
 //{
   GroupStreamlines();
 //}; // end if(dis&ng)

 BuildDisplayListFromStore();

/*
 if(ENABLE_LINEWIDTH) {
   streamlineWidth  = new svScalarArray[seed_num];
   SetLineWidthFromVec3(streamline);
 };
*/

 // write to display list
 cleanStreamLine();
 display_mode = SV_DISPLAYLIST;

 svPrimitive::STILL_UPDATE = false;
}

/*
void ivLine::GenerateFromFile()
{
 svPrimitive::STILL_UPDATE = true;

 cleanStreamLine();

 streamline = new svVector3Array[seed_num];
 readinputsvl(infile, streamline, &seed_num);

 if(ENABLE_COLOR) {
   streamlineColors = new svVector4Array[seed_num];
   SetLineColorFromVec3(streamline);
 };
 if(ENABLE_LINEWIDTH) {
   streamlineWidth  = new svScalarArray[seed_num];
   SetLineWidthFromVec3(streamline);
 };

 // write to display list
 BuildDisplayListFromStore();
 cleanStreamLine();
 display_mode = SV_DISPLAYLIST;

 svPrimitive::STILL_UPDATE = false;
}
*/

//TODO
void ivLine::RemovePoint(svInt seedIndex)
{
}

// generate a new seeding point
void ivLine::SetPoint(svVector3 newp) 
{
  STILL_UPDATE = true;

  // update seeding information
  svInt oldSeedNum = seed_num;
  seed_num = seed_num+1;

  svParticle *newseed = new svParticle[seed_num];
  svInt i=0, j=0;
  for(i=0;i<oldSeedNum;i++) { 
      newseed[i]=seed[i];
  }
  delete [] seed;
  newseed[i].position=newp;
  newseed[i].field = field;
  newseed[i].time = 0.;

  seed = new svParticle[seed_num];
  for(i=0;i<seed_num;i++) { 
      seed[i]=newseed[i];
  }
  delete [] newseed;

// debug
  cerr << "===> ivLine:SetPoint: seeding points: " <<endl;
  for(i=0;i<seed_num;i++) { 
    cerr << seed[i].position[0] 
         << seed[i].position[1]
	 << seed[i].position[2]<<endl;
  }
  cerr << "===> ivLine:SetPoint: end seeding points: " <<endl;

  STILL_UPDATE=false;
}


/*
void ivLine::EnableLineWidth(svFloat minlw, svFloat maxlw)
{
  lut.SetLineWidthRange(minlw,maxlw);
  lut.SetScalarRange(0.0007, 1.5);

  cleanStreamLine();
  cleanDisplayList(SV_IMMEDIATE);

  ENABLE_LINEWIDTH = true;
}
*/

void ivLine::EnableColor()
{
  ENABLE_COLOR=true;
}

void ivLine::RenderGroups()
{
  void *font1 = GLUT_BITMAP_HELVETICA_10;

  if(STILL_UPDATE) return;
  if(ENABLE_COLOR) lut.Render();

  glColor3fv(render_property.color.getValue());
  if(display_mode == SV_IMMEDIATE) {
    Generate();
  }
  else if(display_mode == SV_DISPLAYLIST && glIsList(display_list)) {
    glCallList(display_list);
  }
  else if (display_mode == SV_STORE) {
    if(ENABLE_COLOR==true || ENABLE_LINEWIDTH==true)
      RenderStoreDirect();
    else
      RenderStore();
  };

  for(int i=0; i<num_of_items; i++)
  {
    void *font1 = GLUT_BITMAP_HELVETICA_10;
    GLdouble vmat[16], pmat[16], sx, sy, sz;
    GLint    viewport[4];


    glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
    glGetIntegerv(GL_VIEWPORT, viewport);

    gluProject((double)(cenx[i]+locx[i]), (double)(ceny[i]+locy[i]), (double)0., vmat, pmat, viewport, &sx,&sy,&sz);
    //gluProject((double)locx[i], (double)locy[i], (double)0., vmat, pmat, viewport, &sx,&sy,&sz);
    sy = (float)viewport[3]-(float)sy;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,(GLfloat) viewport[2], (GLfloat) viewport[3], 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glColor3f(1, 0.8, 0.1);

    
       text(label[i], font1, (float)sx, (float)sy);
       text(speed[i], font1, (float)sx, (float)sy+10);
       text(mass[i], font1, (float)sx, (float)sy+20);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);
  };;// end for(i)

}

void ivLine::SetDividingLines(int nfiles, int *frameIndex)
{
/*
  if(nfiles!=seed_num) 
  { 
    cerr << "ERROR: ivline:SetDividingLines:seed_num != nfiles " << endl;
    cerr << "ERROR: nfiles = " << nfiles << " seed_num = " << seed_num << endl;
    exit(-1);
  }
*/
  num_of_downstroke =  new int[seed_num];
  for(int i=0; i<seed_num; i++) 
  {
    num_of_downstroke[i]=frameIndex[i];
  };
  DOWNSTROKE_SET = true;
}

void ivLine::SetSpeciesLables(int nfiles, char ** _labels) 
{
  if(nfiles != ng)
  {
    cerr << "ERROR: ivline:SetSpeciesLables:ng!= nfiles" << endl;
    exit(-1);
  };
  label = new  char*[ng];
  for(int i=0; i<nfiles; i++)
  {
    int l = strlen( _labels[i] );
    label[i]= new char[l];
    strcpy(label[i], _labels[i]);
  };
  LABEL_SET = true;
}

void ivLine::SetSeparationBy(int _ng, int * _nfg, int num_in_one_line, float _dis)
{
  //  set the parameters
  ng=_ng;  
  if(nfg!=NULL) delete nfg; // make sure we can change the distance later
  nfg = new int[seed_num];
  for(int i=0; i<seed_num; i++)
  {
    nfg[i]=_nfg[i];
  }; // end for(i)

  num_in_line = num_in_one_line;

  // aggregate nfg;
  for(int i=ng-1; i>=0; i--)
  {
    for(int j=0; j<i; j++)
      nfg[i] = nfg[i]+nfg[j]; 
  }; // end for(i)
  dis = _dis;
}

void ivLine::SetSeparationBy(int _ng, int * _nfg, float _dis)
{
  //  set the parameters
  ng=_ng;  
  if(nfg!=NULL) delete nfg; // make sure we can change the distance later
  nfg = new int[seed_num];
  for(int i=0; i<seed_num; i++)    
  {
    nfg[i]=_nfg[i];
  }; // end for(i)

/*
  // aggregate nfg;
  for(int i=ng-1; i>=0; i--)
  {
    for(int j=0; j<i; j++)
      nfg[i] = nfg[i]+nfg[j]; 
  }; // end for(i)
*/

  dis = _dis;
}

void ivLine::Render()
{
  if(STILL_UPDATE) return;
  if(ENABLE_COLOR) lut.Render();

  glColor3fv(render_property.color.getValue());
  if(display_mode == SV_IMMEDIATE) {
    Generate();
  }
  else if(display_mode == SV_DISPLAYLIST && glIsList(display_list)) {
    glCallList(display_list);
  }
  else if (display_mode == SV_STORE) {
    if(ENABLE_COLOR==true || ENABLE_LINEWIDTH==true)
      RenderStoreDirect();
    else
      RenderStore();
  };
}

void ivLine::RenderStoreDirect() const
{
    for (int i=0; i<seed_num; i++) 
    {
      svInt ss = streamline[i].size()-1;
      for(int j=0; j<ss; j++)
      {
        if(ENABLE_LINEWIDTH)
	  glLineWidth((GLfloat)streamlineWidth[i][j]);
        if(ENABLE_COLOR) 
          glColor3fv(streamlineColors[i][j].getValue());
	else  glColor3fv(render_property.color.getValue());

        glBegin(GL_LINE_STRIP);
        glVertex3fv(streamline[i][j].getValue());
        glVertex3fv(streamline[i][j+1].getValue());
        glEnd();
      }; // loop of ss
    }; // loop of seed_num
}

void ivLine::RenderStore() const
{
  for (int i=0; i<seed_num; i++) 
  {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof (svVector3), 
                    &streamline[i][0]);
    glDrawArrays(GL_LINE_STRIP, 0, streamline[i].size());
    glDisableClientState(GL_VERTEX_ARRAY);
  };
}

void ivLine::Print()
{
  for(int i=0; i<seed_num; i++)
  {
    int s = streamline[i].size();
    cerr << s << endl;
    for(int j=0; j<s; j++)
    {
      cerr <<"(" << i<<", " << j<<"): "<< streamline[i][j][0] << "  "
           << streamline[i][j][1] << "  "
	   << streamline[i][j][2] << endl;
    };
  }
}

/*
void ivLine::SaveToFile(char *fname, int freq, bool saveColor)
{
  ofstream outf(fname);
  outf << seed_num << endl;

  if(saveColor)
  {
   for(int i=0; i<seed_num; i++)
   {
    int s = (streamline[i].size())/freq;
    outf << s << endl;
    for(int j=0; j<s; j=j+freq)
    {
      outf << streamline[i][j][0] << "  "
           << streamline[i][j][1] << "  "
	   << streamline[i][j][2] << "  "
           << streamlineColors[i][j][0] <<  "  "
           << streamlineColors[i][j][1] <<  "  "
           << streamlineColors[i][j][2] <<  "  "
           << streamlineColors[i][j][3] << endl;
    };
   }; // end for (i)
  } else {
   for(int i=0; i<seed_num; i++)
   {
    int s = (streamline[i].size())/freq;
    outf << s << endl;
    for(int j=0; j<s; j=j+freq)
    {
      outf << streamline[i][j][0] << "  "
           << streamline[i][j][1] << "  "
	   << streamline[i][j][2] << endl;
    };
   }; // end for (i)
  }; // end if(saveColor)
  outf.close();
}
*/

}
