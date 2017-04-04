#ifndef __SV_DIRECT_BAR_H
#define __SV_DIRECT_BAR_H

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

/*typedef struct DirectProperty{
  svScalar UnitHeight;
  svScalar width; 
}DirectProperty; 
  */
  
class svDirectBar : public svBarGlyph{
public:
   svDirectBar(svVectorField *f);

   virtual ~svDirectBar(){};

   virtual void Generate(DirectProperty &property, ViewProperty &viewproperty,
                            svVector3 planeDir);

   virtual void Render();
   svVector3 GetEnd(DirectProperty &property, ViewProperty &viewproperty,
               svVector3 planeDir,
                int seed, int index);

protected:
private:

};
}

#endif

