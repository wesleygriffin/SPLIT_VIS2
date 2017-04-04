/* rendering routine of a vector field using glyphs */

#ifndef __SV_SPLIT_VECTORS_CYLINDER_H
#define __SV_SPLIT_VECTORS_CYLINDER_H

#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"
#include "svCylinderGlyph.h"
#include "svUtil.h"
#ifndef PI
#define PI 3.1415926
#endif


#ifndef CYLINDERSLICE 
#define CYLINDERSLICE 6
#endif 

namespace __svl_lib {


class svSplitVectorsCylinder : public svCylinderGlyph{

public:
   svSplitVectorsCylinder(svVectorField *f);

   virtual ~svSplitVectorsCylinder(){};

   virtual void Generate(SplitVectorsProperty &property,ViewProperty &viewproperty);
   virtual void Generate(SplitVectorsProperty &property,ViewProperty &viewproperty, int index);
  

   virtual void Render();

};


}

#endif
