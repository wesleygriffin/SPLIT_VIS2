#ifndef __SV_BEND_BAR_H
#define __SV_BEND_BAR_H

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

typedef struct BendProperty{
  svScalarArray bendRange;
  svScalarArray bendHeight;
  svScalarArray bendWidth;
  svScalarArray bendLegendRange;
  svScalarArray bendLegendHeight;
  svScalarArray bendLegendWidth;  
  svScalarArray bendLegendValue;
  svVector3Array groupcolor;
} BendProperty;

class svBendBar : public svBarGlyph{

public:
   svBendBar(svVectorField *f);

   virtual ~svBendBar(){};

   virtual void SetLegendList(svInt list){legend_list = list;}

   virtual void GenerateLegend(BendProperty &property, svVector3 startPos, svVector3 dir1, svVector3 dir2,
                       svScalar stepD1, svScalar stepD2,
                       svInt stepN1, svInt stepN2, svVector3 planeDir);
   virtual void Generate(BendProperty &property, ViewProperty &viewproperty,
                            svVector3 planeDir);
   virtual void Generate(BendProperty &property, ViewProperty &viewproperty,
                            svVector3 planeDir, int indexx);   
   virtual void SaveToOBJ(char *fname,BendProperty &property,svVector3 planeDir);
   virtual void Render();

   svVector3 GetEnd(BendProperty &property, ViewProperty &viewproperty,
               svVector3 planeDir,
                int seed, int index);
   virtual void RenderLegend(BendProperty &property);
protected:
private:
   svInt legend_list;
};
}

#endif

