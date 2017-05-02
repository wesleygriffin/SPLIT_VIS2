
/* rendering routine of a vector field using glyphs */

#ifndef __SV_DIRECT_ARROW_H
#define __SV_DIRECT_ARROW_H

#include "svArrowGlyph.h"

//#include "svArray.h"
//#include "svVectorField.h"
//#include "svParticle.h"
//#include "svPrimitive.h"
//#include "svLut.h"

#ifndef PI
#define PI 3.1415926
#endif




namespace __svl_lib {

class svDirectArrow : public svArrowGlyph {
  public:
    svDirectArrow(svChar *inf);
    svDirectArrow(svVectorField *f);
	
    virtual ~svDirectArrow(){};

    virtual void SaveToFile(char *fname);
 
    virtual void GenerateArrows();
    virtual void GenerateTubes();
    virtual void GenerateIndex();

    virtual void RenderSample();

    virtual void Render();
    //virtual void RenderSample();
    virtual void Generate();
    virtual void GenerateSample(int frequency);
  private:
    void RenderStore() const; 
 //   svScalar   glyphSize;
//    svScalar   glyphScale;
//    svScalar   glyphRadius;

  protected:
   // virtual void BuildDisplayListFromStore();
};

}


#endif // __SV_DIRECT_ARROW_H
