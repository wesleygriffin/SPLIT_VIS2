/**
basic functions to read data file
input file:
	sorted data file
		raw/contour
	clustering file
	data format
output file:
	roi file
	symmetry file
**/

#ifndef __SV_READ_DATA_H
#define __SV_READ_DATA_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <vector>

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"

#define ARROW_BASE 0.25
#define NUM_SEG 4
#define SPHERE_STACK 6
#define SPHERE_SLICE 6

using namespace std;

namespace __svl_lib {

class svArrowGlyph 
{
 public:
  // by default: generate uniform scaled line glyphs
  svArrowGlyph(char *data_file, char *format_file);  
  ~svArrowGlyph(){cleanup();}


  void SetData(char *file); 
  void ReadDataFormat(char *file);
  void SetInbox(bool *in);
  void SetSample(int Frequency, bool raw_data);

  void SetArrows();
  void SetColors(svVector4 *color);	

  void SetFontColor(svVector3 c);
  void SetScalar(int s){SCALE = s;}
  void SetEncodeMode(int m){encode_mode=m;}

//http://math.hws.edu/eck/cs424/notes2013/opengl-c/color-cube-of-spheres.c
  void createSphereArraysAndVBOs();
  void drawSphereDirectWithDataFromArrays() ;
  void RenderArrow(float a);

  void CallList();
 protected:

  //int d_scalar;

  svVector3Array  *glyphPos;   
  svVector3Array  *glyphDir;   // direction
  svScalar   *glyphMag;   // magnitute
  svScalar *glyphExp;
  svScalar *glyphCoe;
  svVector3Array *glyphLegend;
  svBool *glyphInbox;
  svBool *markedData;
  int glyphFrequency;
  vector<vector<vector<int> > > rawFormat;
  bool *dataSample;
  int dataSize;

  int SCALE;

  int displayList;

  svScalar *glyphradius;
  svVector4Array *glyphColors; //Colors of arrows
  svVector3Array  *glyphArrowPos; //position of arrow heads
  svVector3Array  *glyphArrowNorm; //normalized vectors of arrows

  float *sphereVertexArray; // Pointer to the vertex coords, for use with glDrawArrays
  float *sphereNormalArray; // Pointer to the normal vectors, for use with glDrawArrays

  int encode_mode;//0 is direct mapping; 1 is splitvector; 2 is uniform
  svVector3 font_color;
  double maxmag;

  int selectedIndex;

	//void cleanformat();
	void cleanup();
};

}
#endif // __SV_ARROW_GLYPH_H
