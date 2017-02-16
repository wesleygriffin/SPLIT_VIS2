#ifndef __SV_BEND_BAR_H
#define __SV_BEND_BAR_H

#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"
#include "svBarGlyph.h"

#ifndef PI
#define PI 3.1415926
#endif


namespace __svl_lib {

typedef struct BendProperty{
  svScalarArray bendRange;
  svScalarArray bendHeight;
  svScalarArray bendWidth;
  svScalarArray *bendLegend;  
} BendProperty;

class svBendBar : public svBarGlyph{

public:
   svBendBar(svVectorField *f);

   virtual ~svBendBar(){};

   virtual void Generate(BendProperty &property, ViewProperty &viewproperty,
                            svVector3 planeDir);

   virtual void Render();

protected:
private:
   
};
}

#endif

