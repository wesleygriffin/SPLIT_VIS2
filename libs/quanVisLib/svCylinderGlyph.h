#ifndef __SV_CYLINDER_GLYPH_H
#define __SV_CYLINDER_GLYPH_H

#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"
#include "svGlyph.h"

#ifndef PI
#define PI 3.1415926
#endif


#ifndef CYLINDERSLICE 
#define CYLINDERSLICE 10
#endif 

namespace __svl_lib {

typedef struct CylinderLegendProperty{
  svInt vSegment;
  svInt hSegment;
  svVector4 color;
  svVector3 vPos;//vertical
  svVector3 vDir;
  svVector3 hPos;
  svVector3 hDir;
  svScalar vUnit;
  svScalar hUnit;
} CylinderLegendProperty;


class svCylinderGlyph : public svGlyph{

public:
   svCylinderGlyph(svVectorField *f);

   virtual ~svCylinderGlyph(){};

   virtual void SetLegendProperty(CylinderLegendProperty &legendproperty){lproperty = legendproperty;}

   virtual void Draw_InnerLegend();
   virtual void Draw_WidthLegend();


protected:
   CylinderLegendProperty lproperty;

};
}

#endif

