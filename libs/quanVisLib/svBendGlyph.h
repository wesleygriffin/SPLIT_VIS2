/* rendering routine of a vector field using glyphs */

#ifndef __SV_BEND_GLYPH_H
#define __SV_BEND_GLYPH_H

#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"
#include "svCylinderGlyph.h"

#ifndef PI
#define PI 3.1415926
#endif


#ifndef CYLINDERSLICE 
#define CYLINDERSLICE 6
#endif 

namespace __svl_lib {

typedef struct BendProperty{
      svScalarArray bendRange;
      svScalarArray bendHeight;
      svScalarArray bendWidth;
   } BendProperty;


class svBendGlyph : public svCylinderGlyph{

public:
   svBendGlyph(svChar *inf);
   svBendGlyph(svVectorField *f);

   virtual ~svBendGlyph(){};

   virtual void Render();
   virtual void Generate(BendProperty &property);

};


}

#endif
