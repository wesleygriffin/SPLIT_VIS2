
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
    svMesh(){mesh=new svVector3Array[1];dir = new svVector3Array[1];};

    virtual void New(vector<int> region);

    virtual void SetData(char *file, int index);	
    virtual ~svMesh(){cleanup();};

    virtual void GenerateWireframe(char *dir);
    virtual void GenerateWiresolid(char *dir);
    virtual void GenerateWireframe(int region);
    virtual void GenerateSurface(int region);

    virtual void GenerateWireframe();
    virtual void GenerateSurface();

    virtual void Render();

    
     void SetDisplayList(svInt list){display_list = list;}
  protected:
     void cleanup();

  private:
     svInt display_list;
     svVector3Array *mesh;
     svVector3Array *dir;
     vector<int> meshregion;
};

}


#endif // __SV_MESH_H
