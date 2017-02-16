/**
Render 3D arrows
Input data:
	data file: 
		pos, vel, den, exp, coe
	format file
	color
	frequency
	enode mode: linear (1), splitvectors (2), uniform (3)
	arrow_radius, arrow_base
	inbox: bool array
	font_color
	scaling value
**/

#ifndef __SV_ARROW_GLYPH_H
#define __SV_ARROW_GLYPH_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <vector>

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"

//#define ARROW_BASE 0.25
#define NUM_SEG 6
#define SPHERE_STACK 6
#define SPHERE_SLICE 6

using namespace std;

namespace __svl_lib {

class svArrowGlyph : public svGlyph
{
 public:
  // by default: generate uniform scaled line glyphs
  svArrowGlyph(svVector3 *pos, svVector3 *vel,
		svScalar *den, svScalar *exp, svScalar *coe, svInt *atom,
		svInt size, vector<vector<vector<int> > > f);
  svArrowGlyph(svVector3Array *posIn, svVector3Array *velIn,
	       svScalarArray  *den, svScalarArray  *exp,  svScalarArray *coe,
	       svIntArray  *atom, 
	       svInt size, vector<vector<vector<int> > > f);

  svArrowGlyph(char *data_file, char *format_file);  
  ~svArrowGlyph(){cleanup();}

  void SetData(svVector3 *pos, svVector3 *vel,
		svScalar *den, svScalar *exp, svScalar *coe,
		svInt *atom,
		svInt num);
  void SetData(vector< vector<svVector3> > pos, vector< vector<svVector3> > vel,
		vector< vector<svScalar> > den, vector< vector<svScalar> > exp, vector< vector<svScalar> > coe,
		vector< vector<svInt> > atom, 
		svInt num);
  void SetData(char *file); 
  void ReadDataFormat(vector<vector<vector<int> > > f);
  void ReadDataFormat(char *file);
  void SetInbox(bool *in);
  void SetExpand(bool *in);
  void SetSample(int Frequency, bool raw_data);

  void setLength(double l);
  void SetArrows();
  void SetColors(svVector4 *color);	

  void SetFontColor(svVector3 c);
  void SetScalar(int s);
  void setArrowBase(double b){ARROW_BASE = b;}
  void SetEncodeMode(int m){encode_mode=m;}

  void DrawSpring(svVector3 ptStart, svVector3 ptEnd, double Radius, double ringHeight, double Rings);
//http://math.hws.edu/eck/cs424/notes2013/opengl-c/color-cube-of-spheres.c
  void createSphereArraysAndVBOs();
  void drawSphereDirectWithDataFromArrays() ;
  void RenderArrow(float a, bool layer, float zvalue);//add a shape 
  void svArrowGlyphRenderArrow(float a);
  void geometryDraw(int atom, double base);
  void extendLength(double length);
  void extendRadius(double r);

  void setMaxMag(double m){maxmag = m;}

  void CallList();

  int displayList;
 protected:

  //int d_scalar;

  svVector3Array  *glyphPos;   
  svVector3Array  *glyphDir;   // direction
  svInt *dataAtom;
  svScalar   *glyphMag;   // magnitute
  svScalar *glyphExp;
  svScalar *glyphCoe;
  svVector3Array *glyphLegend;
  svBool *glyphInbox;
  svBool *glyphExpand;
  svBool *markedData;
  int glyphFrequency;
  vector<vector<vector<int> > > rawFormat;
  bool *dataSample;
  int dataSize;
  double ARROW_BASE;

  double min_length;

  int SCALE;

 // int displayList;

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
