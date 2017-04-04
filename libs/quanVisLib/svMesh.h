
/* rendering routine of a vector field using glyphs */

#ifndef __SV_MESH_H
#define __SV_MESH_H


#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"



namespace __svl_lib {

class svMesh {
  public:
    svMesh(){};
	
    virtual ~svMesh(){};

    virtual void GenerateWireframe(char *dir);
    virtual void GenerateWiresolid(char *dir);

    virtual void Render();

    void SetDisplayList(svInt list){display_list = list;}

  private:
     svInt display_list;
};

}


#endif // __SV_MESH_H
