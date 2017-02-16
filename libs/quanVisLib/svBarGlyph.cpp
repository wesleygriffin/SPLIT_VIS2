#include "svBarGlyph.h"

using namespace std;

namespace __svl_lib {

bool compare_sortbar(SortBar a, SortBar b){return (a.mag<b.mag);}


  svBarGlyph::svBarGlyph(svVectorField *f) : svGlyph()
  {
      field = f;
  }

  void svBarGlyph::Generate()
  {

  }

  void svBarGlyph::Render()
  {

  }

}
