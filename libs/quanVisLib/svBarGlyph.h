#ifndef __SV_BAR_GLYPH_H
#define __SV_BAR_GLYPH_H
#include "svUtil.h"
#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"
#include "svGlyph.h"

#ifndef PI
#define PI 3.1415926
#endif


namespace __svl_lib {

typedef struct BarProperty{
  svScalar scalex;
  svScalar scaley;
  svScalar halowidth;
  svScalar circleradius;//circle on the base
  svVector4 circlecolor;
  svScalar legend;// how many units in a legend line
  svVector4 legendcolor;
  svScalar legendh;
  svScalar legendw;
  svScalar dirbarh;
}BarProperty;

typedef struct SortBar{
    int index;
    int rangeindex;
    double mag;
} SortBar;

bool compare_sortbar(SortBar a, SortBar b);//{return (a.mag<b.mag);}

class svBarGlyph : public svGlyph{

public:
   svBarGlyph(svVectorField *f);

   virtual ~svBarGlyph(){};

   virtual void SetBarProperty(BarProperty &bp){barproperty = bp;}

   virtual void Generate();
   virtual void Render();

protected:

   BarProperty barproperty;
private:
};
}

#endif

