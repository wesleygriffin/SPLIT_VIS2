#include "svMarker.h"

using namespace std;

namespace __svl_lib {

svMarker::svMarker(svVectorField *f):svGlyph()
{
   field = f;
}

void svMarker::Render(svVector3 start, svVector3 end, svVector3 dir)
{
   glEnable(GL_NORMALIZE);
   
   
   glSolidCone(1,4, 10, 2);
}

}
