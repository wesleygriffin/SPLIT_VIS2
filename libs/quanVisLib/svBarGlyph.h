#ifndef __SV_BAR_GLYPH_H
#define __SV_BAR_GLYPH_H

#include "svGlyph.h"


#include "svUtil.h"
#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"

#ifndef PI
#define PI 3.1415926
#endif


namespace __svl_lib {

typedef struct BarProperty{
  svScalar scalex;
  svScalar scaley;
  svScalar halowidth;
  svScalar circleradius;//circle on the base
  svVector4 circlecolor;
  svScalar legend;// how many units in a legend line
  svVector4 legendcolor;
  svScalar legendh;
  svScalar legendw;
  svScalar dirbarh;
}BarProperty;

typedef struct SortBar{
    int index;
    int rangeindex;
    double mag;
} SortBar;

bool compare_sortbar(SortBar a, SortBar b);//{return (a.mag<b.mag);}

class svBarGlyph : public svGlyph{

public:
   svBarGlyph(svVectorField *f);

   virtual ~svBarGlyph(){cleanup();};

   virtual void SetBarProperty(BarProperty &bp){barproperty = bp;}

   virtual void Generate();
   virtual void Render();

   svVector3 GetVerticalDir(int seed, int index,svVector3 planeDir, ViewProperty &property);

   svVector3Array *getBarEnd1(){return barEnd1;}
   svVector3Array *getBarEnd2(){return barEnd2;}
   svVector3Array *getBarEnd3(){return barEnd3;}
   svVector3Array *getBarEnd4(){return barEnd4;}

   svVector3 getBarEnd1(int seed, int index){return barEnd1[seed][index];}
   svVector3 getBarEnd2(int seed, int index){return barEnd2[seed][index];}
   svVector3 getBarEnd3(int seed, int index){return barEnd3[seed][index];}
   svVector3 getBarEnd4(int seed, int index){return barEnd4[seed][index];}


protected:
   void cleanup();

   BarProperty barproperty;
   svVector3Array *barEnd1;
   svVector3Array *barEnd2;
   svVector3Array *barEnd3;
   svVector3Array *barEnd4;
};
}

#endif

