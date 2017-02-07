#ifndef __SV_BASE_COLOR_H
#define __SV_BASE_COLOR_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <string.h>
#include <assert.h>
#include <vector>
#include <algorithm>

#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"

#include "../jcUtil/MGL.h"
#include "../jcUtil/MArray.h"
#include "../jcUtil/MVectorMatrix.h"

#include "CIEColor.h"

#ifndef M_PI
#define M_PI 3.1415926
#endif

using namespace std;

namespace __svl_lib{

/* the base class of color */
class svColor {
public:
	svColor();
	~svColor(){void cleanup();}
	
	svVector4 *getColors(bool toggleBlind, bool toggle, 
				int level, int *cluster, int size,
				svVector3 font_color);
	svVector4 *getColors(bool toggleBlind, bool toggle, int size);

	svVector4 * getColors(bool toggleBlind,int *cluster,svVector3 font_color,int size);
	svVector4 * getColors(bool toggleBlind,vector< vector<int> > cluster,svVector3 font_color,int size);

	void setHC(vector< vector<double> > mag);

	void SetD3Color();
	void SetSpiralColor();
	void GetSpiralColor(int *cluster, svVector4 * color, int num, bool blindsafe); //cluster of mag
	void GetD3Color(int *cluster, svVector4 *color, int num, bool blindsafe); //cluster of vel: contour,plane,raw
	
	void SetSpiralColor(int level, double *range_min, double *range_max);
	void SetD3Color(int *cluster, svVector3 *vel, int level, int num);//need to be fixed
	
	void RGB2LAB(double R, double G, double B, double &l, double &a, double &b);
	void ColorBlindSafe(double r, double g, double b, double &r2, double &g2, double &b2);
	void HSVtoRGB( double &r, double &g, double &b, double h, double s, double v );
	void LAB2RGB(double l, double a, double b, double &R, double &G, double &B);
	
	void SetFontColor(svVector3 c){font_color = c;}
	void RenderSpiralColorLegend(bool blindsafe, float scalar);
	void RenderD3ColorLegend(bool blindsafe, int selected_p);
	void RenderD3ColorLegend(svVector3 *vel, int *cluster, int size, bool blindsafe);

	void RenderHCLegend(bool blindsafe, bool enable_color);

	void Mouse(int mx, int my);
	
	void cleanup();
protected:
        svVector3 *dataVel;
	svInt *dataCol;
	svScalar *range[2];
	
	svVector4 *spiralcolor;
	svVector4 *spiralcolor_blindsafe;
	svVector4 *d3color;
	svVector4 *d3color_blindsafe;
	
	int dataSize;
	
	int spiral_level;
	int d3_level;
	svVector3 font_color;

	vector< vector<double> > hc_magrange;
	vector<bool> hc_index;
};
}
#endif //__SV_BASE_COLOR_H
