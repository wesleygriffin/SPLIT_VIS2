/** 
 *  svParticle
 */

#ifndef __SV_PARTICLE_H
#define __SV_PARTICLE_H

#include "svType.h"
#include "svVectorMatrix.h"

namespace __svl_lib {
class svVectorField;

class svParticle {
 public:
  svParticle() 
    {  time = 0.0; }
  svParticle(svScalar t, const svVector3 & p)
    {  time = t; position = p;  }
  svParticle(svScalar t, svScalar x, svScalar y, svScalar z)
    {  time = t; position[0] = x; position[1] = y; position[2] = z;  }
  svParticle(const svParticle& p)
    {  *this = p;  }
  svParticle & operator=(const svParticle& par);

  svVector3 GetPosition() const
    {  return position;  }

  void SetPosition(const svVector3 & p)
    {  position = p;  }

  void GetPosition(Scalar& x, Scalar& y, Scalar& z) const
    {  x = position[0]; y = position[1]; z = position[2];  }

  void SetPosition(const Scalar x, const Scalar y, const Scalar z)
    {  position[0] = x; position[1] = y; position[2] = z;  }

  svScalar GetTime() const
    {  return time;  }
 
  void SetTime(const Scalar t)
    {  time = t;  }

  svVectorField * GetField() const
    {  return field;  }

  void SetField(svVectorField * f)
    {  field = f;  }

 public:
  svVector3 position;       // the current particle position
  svScalar  time;           // the current time stamp
 
  // the vector field associated with the particle
  svVectorField * field; 
};

class svParticleInUnstructured: public svParticle {
 public:
  svParticleInUnstructured() : svParticle()
    {}

  svParticleInUnstructured(svScalar t, const svVector3 & p)
    : svParticle(t, p)
    {} 

  svParticleInUnstructured(svScalar t, svScalar x, svScalar y, svScalar z)
    : svParticle(t, x, y, z)
    {}

  svParticleInUnstructured(const svParticle& p)
    : svParticle(p)
    {}

  svUint GetCell() const
    {  return cell;  }

  void SetCell(svUint c)
    {  cell = c;  }

 public:
  svUint cell;      // the cell particle belongs to
};
}
#endif // __SV_PARTICLE_H
