/* rendering routine of a vector field using glyphs */

#ifndef __SV_DIRECT_CYLINDER_H
#define __SV_DIRECT_CYLINDER_H

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


class svDirectCylinder : public svCylinderGlyph{

public:
   svDirectCylinder(svVectorField *f);

   virtual ~svDirectCylinder(){};

   virtual void Generate(DirectProperty &property,ViewProperty &viewproperty);
   virtual void Generate(DirectProperty &property,ViewProperty &viewproperty, int index);
  

   virtual void Render();

};


}

#endif
