#ifndef __SV_SUMMARY_GLYPH_H
#define __SV_SUMMARY_GLYPH_H

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

class svSummaryGlyph : public svGlyph {
  public:
    svSummaryGlyph(svChar *inf);
    svSummaryGlyph();

    void SetNumPower(svInt power){numPower = power;}

    virtual ~svSummaryGlyph();
 
    virtual void SetColors();
    virtual void SetScaling(svInt s){scaling =s;}

    virtual void Generate(float alpha);
    virtual void Render();
protected:
    void cleanup();
    void RenderAnnulus(svVector3 pos, svVector3 vel,
                                svScalar x, svScalar H, svScalar r);
private:
    svVector3Array summaryPos;
    svVector3Array summaryDir;
    svScalarArray  summaryDen;
    svIntArray     summaryLabel;
    svVector4Array summaryColor;
    svVector4Array summaryDenColor;

    svInt scaling;
    svInt numPower; 

    svScalarArray *denDistribute;
};

}

#endif // __SV_SUMMARY_GLYPH_H

