
/* rendering routine of a vector field using glyphs */

#ifndef __SV_READ_CONFIG_H
#define __SV_READ_CONFIG_H


#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"



namespace __svl_lib {

class svReadConfig {
  public:
    svReadConfig(char *file);
   
    char *LookupString(char *name);
    bool LookupBool(char *name);
    int LookupBool(char *name);
    double LookupDouble(char *name);
    svVector3 LookupsvVector3(char *name);
    int *LookupIntArray(char *name);
    double *LookupDoubleArray(char *name);
};

}


#endif // __SV_READ_CONFIG_H
