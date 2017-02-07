/**
 *  This file implements svPrimitive
 */

#include <GL/glut.h>
#include "svPrimitive.h"
#include "svException.h"

namespace __svl_lib {
//Constructor
svPrimitive::svPrimitive(svInt  _seed_num)
{
  field = NULL;
  seed = NULL;
  seed_num = _seed_num;
  //display_mode = SV_DISPLAYLIST;
  display_mode = SV_STORE;

  svColor4 color(0.1, 0.1, 0.9, 0.5);
  render_property.color = color;
  render_property.line_width = 1.0;

  STILL_UPDATE = false;
  ENABLE_SEGMENT_FREQ = false;
  ENABLE_LINEWIDTH = false;

  //_colors = NULL;
  ENABLE_COLOR = false;

  lbbox=svVector3(-1,-1,-1); 
  rbbox=svVector3(1,1,1);
}

svPrimitive::svPrimitive(svVectorField* f, svParticle* sd, svInt sd_num, svUint mode)
{
  field    = f;
  seed     = sd;
  seed_num = sd_num;

// JC
/*
  if (field!=NULL) {
    if(field->GetTimeStepNum() == 1)  //static field
      field->SetFreezeTime(0.0);
    else if(field->GetTimeStepNum() > 1) //unstatic field
      field->SetFreezeTime(seed[0].GetTime());
  }
*/

//JC
 // max_advection_time = DEFAULT_MAX_ADVECTION_TIME;
//  segment_freq = DEFAULT_SEGMENT_FREQ;
 // direction = SV_FORWARD;

  display_mode = mode;
  display_list = 0;	
  
  svColor4 color(0.1, 0.1, 0.9, 0.5);
  render_property.color = color;
  render_property.line_width = 1.0;

  STILL_UPDATE = false;
  ENABLE_SEGMENT_FREQ = false;
  ENABLE_LINEWIDTH = false;

  //_colors = NULL;
  ENABLE_COLOR = false;

  lbbox=svVector3(-1,-1,-1); 
  rbbox=svVector3(1,1,1);
}

svPrimitive::~svPrimitive()
{
  if (glIsList(display_list)){
    glDeleteLists(display_list,1);
	display_list = 0;
  }
  if(seed!=NULL) 
    delete [] seed;
}


void svPrimitive::DrawOutline( svBool set_color_flag )
{
  if (set_color_flag)
    glColor3f(1., 1., 1.);
  glLineWidth(1.0);
  if(field!=NULL)
       field->GetBoundingBox(&lbbox,&rbbox);

  glBegin(GL_LINE_LOOP);
  glVertex3f(lbbox[0],lbbox[1],lbbox[2]);
  glVertex3f(rbbox[0],lbbox[1],lbbox[2]);
  glVertex3f(rbbox[0],rbbox[1],lbbox[2]);
  glVertex3f(lbbox[0],rbbox[1],lbbox[2]);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex3f(lbbox[0],lbbox[1],rbbox[2]);
  glVertex3f(rbbox[0],lbbox[1],rbbox[2]);
  glVertex3f(rbbox[0],rbbox[1],rbbox[2]);
  glVertex3f(lbbox[0],rbbox[1],rbbox[2]);
  glEnd();

  glBegin(GL_LINES);
  glVertex3f(lbbox[0],lbbox[1],lbbox[2]);
  glVertex3f(lbbox[0],lbbox[1],rbbox[2]);
  glVertex3f(rbbox[0],lbbox[1],lbbox[2]);
  glVertex3f(rbbox[0],lbbox[1],rbbox[2]);
  glVertex3f(rbbox[0],rbbox[1],lbbox[2]);
  glVertex3f(rbbox[0],rbbox[1],rbbox[2]);
  glVertex3f(lbbox[0],rbbox[1],lbbox[2]);
  glVertex3f(lbbox[0],rbbox[1],rbbox[2]);
  glEnd();
}

void svPrimitive::cleanDisplayList(svUint newMode)
{
  if(glIsList(display_list))
  {
      glDeleteLists(display_list, 1);
      display_mode = newMode;
      display_list = 0;
  };
}

svInt svPrimitive::gotoNextPosition(svParticle &p, svInt &dir,
              svFloat max_time, svBool adv) 
{
//JC
/*
    if(field->pointOutOfBound(p.position) == true)
      return SV_EXP_OUT_OF_RANGE_SPACE;
    if(adv) p.Advect(dir, max_time);
    else  p.Step(dir, max_time);
    return SV_STEP_OK;
*/
}

svInt svPrimitive::gotoNextPosition(svParticle &p, svInt &dir) 
{
    if(field->pointOutOfBound(p.position) == true)
      return SV_EXP_OUT_OF_RANGE_SPACE;
//    p.Step(dir);
    return SV_STEP_OK;
}
}
