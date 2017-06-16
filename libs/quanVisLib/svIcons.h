#ifndef __SV_ICONS_H
#define __SV_ICONS_H


#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"
#include "svUtil.h"
#include <string.h>

namespace __svl_lib{


void DrawArrow1(float arrowbase,
                float arrowheight,
                float height,
                svVector3 pos);
void DrawArrow2(float arrowbase,
                float arrowheight,
                float height,
                svVector3 pos);

void DrawDownButtons(float w, float h, svVector3 pos);
void DrawUpButtons(float w, float h, svVector3 pos);

void DrawStackBars(svScalar level, 
                  float w, float h, svVector3 *pos,
                  svVector3 end,
                  int tophisto, svScalarArray *histovalues);

void DrawHorizonBars(float w, float h, svVector3 *pos,
                   int tophisto, int * histovalues, int num);

void DrawArrowDownButton(int type,
                     float w, float h,
                     svVector3 pos,
                     svVector4 color);
void DrawArrowUpButton(int type, 
                     float w, float h,
                     svVector3 pos,
                     svVector4 color);

void DrawArrowButton(float w, float h,
                     svVector3 pos,
                     int type);

void DrawEncodeDownButton(char **text,
                     float w, float h,
                     svVector3 pos);
void DrawEncodeUpButton(char **text,
                     float w, float h,
                     svVector3 pos);

void DrawTextUpButton(char *text,
                     float w, float h,
                     svVector3 pos);
void DrawTextDownButton(char *text,
                     float w, float h,
                     svVector3 pos);
}
#endif
