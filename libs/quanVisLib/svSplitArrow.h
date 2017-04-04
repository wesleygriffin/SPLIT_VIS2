
/* rendering routine of a vector field using glyphs */

#ifndef __SV_SPLIT_ARROW_H
#define __SV_SPLIT_ARROW_H

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

class svSplitArrow : public svGlyph {
  public:
    svSplitArrow(svChar *inf);
    svSplitArrow(svVectorField *f);
	
    virtual ~svDirectArrow(){};
  
    virtual void Render();
    virtual void Generate();

    virtual void SetES(svScalar s){expScale = s;}
    virtual void SetCS(svScalar s){coeScale = s;}

  private:

    void RenderStore() const; 
 //   svScalar   glyphSize;
//    svScalar   glyphScale;
//    svScalar   glyphRadius;
   svScalar expScale;
   svScalar coeScale;
  protected:
   // virtual void BuildDisplayListFromStore();
};

}


#endif // __SV_DIRECT_ARROW_H
