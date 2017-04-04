#ifndef __SV_COLORS_H
#define __SV_COLORS_H
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "svVectorField.h"

namespace __svl_lib {

class svColors
{
public:
	svColors();
	~svColors();

        svVector4 GetDiscreteColors(int value);
	svVector4 GetDiscreteColors(int color_model, int value);
	svVector4Array GetContinuousColors(svScalarArray & value);//not implemented

        void RGB2LAB(double R, double G, double B, double &l, double &a, double &b);
        void ColorBlindSafe(double r, double g, double b, double &r2, double &g2, double &b2);
        void HSVtoRGB( double &r, double &g, double &b, double h, double s, double v );
        void LAB2RGB(double l, double a, double b, double &R, double &G, double &B);

protected:
	svVector4Array *colorModel;
        svVector4Array spiralColor;
};
}


#endif // __SV_COLORS_H

