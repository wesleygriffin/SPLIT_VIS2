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

protected:
	svVector4Array *colorModel;
        svVector4Array spiralColor;
};
}


#endif // __SV_COLORS_H

