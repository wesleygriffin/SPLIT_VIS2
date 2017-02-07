/** 
 *  svParticle
 */

#include "svParticle.h"
#include "svVectorField.h"

namespace __svl_lib {
svParticle & svParticle::operator=(const svParticle& par)
{  
    if(this == &par) 
      return *this;

    position = par.position;
    time = par.time;
    field = par.field;

    return *this;
};

}
