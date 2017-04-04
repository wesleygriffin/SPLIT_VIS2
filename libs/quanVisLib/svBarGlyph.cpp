#include "svBarGlyph.h"

using namespace std;

namespace __svl_lib {

bool compare_sortbar(SortBar a, SortBar b){return (a.mag<b.mag);}


  svBarGlyph::svBarGlyph(svVectorField *f) : svGlyph()
  {
      seed_num =0;
      field = f;

       barEnd1 = new svVector3Array[1];
       barEnd2 = new svVector3Array[1];
       barEnd3 = new svVector3Array[1];
       barEnd4 = new svVector3Array[1];
  }

  void svBarGlyph::Generate()
  {

  }

  void svBarGlyph::Render()
  {

  }

  svVector3 svBarGlyph::GetVerticalDir(int seed, int index, svVector3 planeDir, ViewProperty &property)
  {
             svVector3 v1 = dir[seed][index];
             svVector3 v2 = planeDir;
             v2.normalize();
             svVector3 v = cross(v1, v2);
             v.normalize();

          svVector3 shiftv = normalize(cross(dir[seed][index], v));
             svVector3 eyev  = property.eye; eyev.normalize();
             if(dot(shiftv, eyev)<0)shiftv=-shiftv;

             return shiftv; 
   }

  void svBarGlyph::cleanup()
  {
       if(barEnd1 == NULL)
       {
             for(int i=0;i<seed_num;i++)
             {
                  barEnd1[i].free();
             }
             delete [] barEnd1;
             barEnd1 = NULL;
       }
       if(barEnd2 == NULL)
       {
             for(int i=0;i<seed_num;i++)
             {
                  barEnd2[i].free();
             }
             delete [] barEnd2;
             barEnd2 = NULL;
       }
       if(barEnd3 == NULL)
       {
             for(int i=0;i<seed_num;i++)
             {
                  barEnd3[i].free();
             }
             delete [] barEnd3;
             barEnd3 = NULL;
       }
       if(barEnd4 == NULL)
       {
             for(int i=0;i<seed_num;i++)
             {
                  barEnd4[i].free();
             }
             delete [] barEnd4;
             barEnd4 = NULL;
       }

  }

}
