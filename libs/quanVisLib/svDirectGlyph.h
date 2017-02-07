
/* rendering routine of a vector field using glyphs */

#ifndef __SV_DIRECT_GLYPH_H
#define __SV_DIRECT_GLYPH_H

#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"
#include "svGlyph.h"

#ifndef PI
#define PI 3.1415926
#endif

namespace __svl_lib {

class svDirectGlyph : public svGlyph {
  public:
    svDirectGlyph(svChar *inf);
    virtual ~svDirectGlyph();

    virtual void Render();
    //virtual void Generate();

	void SetScalar(svScalar s){glyphScale =s;}

	void geometryDraw(int atom, double base);
	void DrawSpring(svVector3 ptStart, svVector3 ptEnd, 
		double Radius, double ringHeight, double Rings);

	void Generate(svVector3 *pos3in, svVector3 *vec3in, svScalar *denin,
		svVector4 *color4in, svInt dataSize, svInt *dataAtom,
						float alpha);

  private:
    void RenderStore() const; 
    svScalar   glyphSize;
    svScalar   glyphScale;

  protected:
    virtual void BuildDisplayListFromStore();
};

}


#endif // __SV_DIRECT_GLYPH_H
