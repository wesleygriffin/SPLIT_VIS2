#ifndef __SV_ARROW_GLYPH_H
#define __SV_ARROW_GLYPH_H

#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"
#include "svGlyph.h"

#ifndef PI
#define PI 3.1415926
#endif


#ifndef ARROWSLICE 
#define ARROWSLICE 4
#endif 

#ifndef CYLINDERSLICE
#define CYLINDERSLICE 6
#endif

#ifndef SPHERESLICE
#define SPHERESLICE 6
#endif

#ifndef SPHERESTACK
#define SPHERESTACK 6
#endif

namespace __svl_lib {


class svArrowGlyph : public svGlyph{

public:
   svArrowGlyph(svVectorField *f);

   virtual ~svArrowGlyph(){cleanup();}

   virtual void Clean();
   virtual void CleanData();
   virtual void CleanIndex(); 
   virtual void Update(); 
   virtual void UpdateData();
   virtual void UpdateIndex();
   void SetTubeRadius(svScalar r){glyphTubeRadius = r;}
   svScalar GetTubeRadius(){return glyphTubeRadius;}

   virtual void UpdateIndexVBO();
   virtual void UpdateArrowVBO();
   virtual void UpdateTubeVBO();

   virtual void GenerateVBO();
   virtual void RenderVBO();
 
   virtual void Generate();
   virtual void Render();

protected: 
   void cleanup();
   svScalar glyphTubeRadius;

   GLuint ARROW_VBO;
   GLuint ARROW_IVBO;
   GLuint TUBE_VBO;
   GLuint TUBE_IVBO;
   GLuint ARROW_BASE_VBO;
   GLuint ARROW_BASE_IVBO;
   GLuint TUBE_BASE_VBO;  
   GLuint TUBE_BASE_IVBO; 

   VBOVertex *arrow_vertices;
   VBOVertex *arrow_base_vertices;
   int arrow_indices_size;
   int tube_indices_size;
   int arrow_vertices_size;
   int tube_vertices_size;
   int arrow_base_indices_size;
   int arrow_base_vertices_size;
   int tube_base_indices_size; 
   int tube_base_vertices_size;
   VBOVertex *tube_vertices;
   VBOVertex *tube_base_vertices;
   VBOVertex1 *legend_vertices;
   GLuint *arrow_indices;
   GLuint *tube_indices;
   GLuint *arrow_base_indices;
   GLuint *tube_base_indices;
};
}

#endif

