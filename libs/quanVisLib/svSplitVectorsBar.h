#ifndef __SV_SPLIT_VECTORS_BAR_H
#define __SV_SPLIT_VECTORS_BAR_H

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

typedef struct SplitVectorsProperty{
  svScalar shiftexp;
  svScalar expHeight;
  svScalar coeHeight;
  svScalar expWidth;
  svScalar coeWidth;
  svVector4 coeColor;
  svVector4 expColor;
} SplitVectorsProperty;

class svSplitVectorsBar : public svBarGlyph{

public:
   svSplitVectorsBar(svVectorField *f);

   virtual ~svSplitVectorsBar(){};

   virtual void Generate(SplitVectorsProperty &property, 
                            svVector3 planeDir);

   virtual void Render();

protected:
private:

};
}

#endif
