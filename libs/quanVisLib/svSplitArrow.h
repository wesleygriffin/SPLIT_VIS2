
/* rendering routine of a vector field using glyphs */

#ifndef __SV_SPLIT_ARROW_H
#define __SV_SPLIT_ARROW_H

//#include "svArray.h"
//#include "svVectorField.h"
//#include "svParticle.h"
//#include "svPrimitive.h"
//#include "svLut.h"
//#include "svGlyph.h"

#include "svArrowGlyph.h"

#ifndef PI
#define PI 3.1415926
#endif

#define REPEAT_TEXTURE 2


namespace __svl_lib {

class svSplitArrow : public svArrowGlyph {
  public:
    svSplitArrow(svChar *inf);
    svSplitArrow(svVectorField *f);
	
    virtual ~svSplitArrow(){};
    // void SetNumPower(svInt power){numPower = power;}

  
    virtual void Render();
    virtual void GenerateArrows();
    virtual void GenerateTubes();
    virtual void GenerateLegend();
    virtual void GenerateIndex();
    //virtual void GenerateVBO();

    //virtual void RenderVBO();
    virtual void RenderLegend();
    virtual void RenderSample();

    virtual void GenerateSpring();

    virtual void Generate();
    virtual void GenerateTexture();
    virtual void GenerateSample(int frequency);
//(int contour, svScalar z1, svScalar z2, int z, int frequency);
    virtual void SetES(svScalar s){expScale = s;}
    virtual void SetCS(svScalar s){coeScale = s;}

  private:

    void RenderStore() const; 
 //   svScalar   glyphSize;
//    svScalar   glyphScale;
//    svScalar   glyphRadius;
   svScalar expScale;
   svScalar coeScale;
   //svInt numPower;
  protected:
   // virtual void BuildDisplayListFromStore();
};

}


#endif // __SV_DIRECT_ARROW_H
