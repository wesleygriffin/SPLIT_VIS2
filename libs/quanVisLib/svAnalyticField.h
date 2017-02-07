/** 
 *  svAnalyticField
 */

#ifndef __SV_ANALYTICFIELD_H
#define __SV_ANALYTICFIELD_H

#include "svType.h"
#include "svArray.h"
#include "svVectorField.h"

namespace __svl_lib {
class svAnalyticField : public svVectorField {
 public:
  svAnalyticField();
  svAnalyticField(svVector3 lbbox, svVector3 rbbox, svScalar mt=0.0);
 
  virtual void Update() {};

//  void EnableTimeVaring()
 // {  flow_type = SV_UNSTEADY;  }

  //void DisableTimeVaring()
  //{  flow_type = SV_STEADY;  }

 protected:
};

}

#endif // __SV_ANALYTICFIELD_H
