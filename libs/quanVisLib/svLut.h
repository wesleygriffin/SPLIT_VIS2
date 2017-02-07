/* 
 * */

#ifndef __SV_LUT_H
#define __SV_LUT_H

#include "svType.h"
#include "svVectorMatrix.h"

#define SV_RED     0
#define SV_GREEN   1
#define SV_BLUE    2
#define SV_ALPHA   3

#define SV_EQ_ILLUMINANCE  40
#define SV_LOCS  50
#define SV_GRAY  60
#define SV_BLUE_YELLOW 70

// svLut -- Lookup Table
namespace __svl_lib {
class svLut {
 public:
  svLut();
  svLut(char *fn);
  ~svLut(){};

  void Render();
  void RenderLOC();
  void RenderBlueYellow();

  void MakeBlueYellowTexture();
  void MakeLOCTexture();
  // draw legend

  void init();
  void AddColorSegment(svScalar r1, svScalar v1, 
                       svScalar r2, svScalar v2, svInt channel);
  void AddColorSegment(svScalar r1, const svVector4 & v1, 
                       svScalar r2, const svVector4 & v2);
  void Load(char* filename);
  void GetRange(svScalar& r1, svScalar& r2) const;
  void SetRange(svScalar r1, svScalar r2);

  svInt GetColorModel() {return lutModel;}
  void  SetColorModel(svInt &newModel);
  void  SetColorModel(svInt &newModel,svScalar lr, svScalar rr);

  svVector4 GetColorBlYe(svScalar v);
  svVector4 GetColor(svScalar v) ;
  svVector4 GetColor(svScalar v, svBool reverse);
  svVector4 GetColor(svScalar v, svScalar new_min_cv, 
                     svScalar new_max_cv, svBool reverse);

// scalar ops
  void      GetScalarRange(svScalar& r1, svScalar& r2) const;
  void      SetScalarRange(svScalar min, svScalar max);
  void      SetLineWidthRange(svScalar r1, svScalar r2);
  svScalar  GetScalar(svScalar v, svBool reverse);
  svScalar  GetScalar(svScalar v, svScalar minv, svScalar maxv, 
                      svBool reverse);

  svUint    GetTextureName() const {return texName;}
  void      SetAlpha(svFloat new_alpha) {alpha = new_alpha; }
  svFloat   GetAlpha() const {return alpha; }

  void      RenderLut(svBool new_state) { render_lut = new_state; }
  void      makeBlueYellow();
  void      makeLoc();

 private:
  svInt     lutModel;      // color lut model
  svUint    texName;       // texture name
  svBool    genTexture;    // texture generated or not
  svBool    render_lut;    // if render the texture image legend

  svInt     entry_num;
  svScalar  max_cv;
  svScalar  min_cv;
  svVector4 color[256];
  svScalar  alpha;

  svScalar  max_lv;
  svScalar  min_lv;

  svScalar  max_lineWidth;
  svScalar  min_lineWidth;

  svInt      scalarLevels;    // segment levels
  svScalar   scalarLut[256];  // lineWidth lookup table

};

}

#endif // __SV_LUT_H
