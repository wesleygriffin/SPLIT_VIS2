#ifndef __SV_MARKER__H
#define __SV_MARKER_H

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


class svMarker : public svGlyph{

public:
   svMarker(svVectorField *f);

   virtual ~svMarker(){};

   virtual void Render(svVector3 start, svVector3 end, svVector3 dir);
};
}

#endif

