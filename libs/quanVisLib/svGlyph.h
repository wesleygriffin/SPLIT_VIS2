
#ifndef __SV_GLYPH_H
#define __SV_GLYPH_H
//#include "dual_depth_peeling.h"
#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"
#include "svColors.h"
#include "svList.h"
//#define DEFAULT_GLYPH_RADIUS 0.02
#define DEFAULT_GLYPH_HEIGHT 0.08
#define DEFAULT_GLYPH_FREQUENCY 1
#define DEFAULT_GLYPH_SECTION_NUM 5
#define DEFAULT_GLYPH_SCALE  50000
#define DEFAULT_GLYPH_SIZE  1
#define DEFAULT_GLYPH_RADIUS 300
#define DEFAULT_DISPLAYLIST 5

// TODO: make good glyphs
namespace __svl_lib {


typedef struct DirectProperty{
  svScalar UnitHeight;
  svScalar width;
  svScalar shift;
}DirectProperty;


typedef struct SplitVectorsProperty{
  svScalar shiftexp;
  svScalar expHeight;
  svScalar coeHeight;
  svScalar expWidth;
  svScalar coeWidth;
  svVector4 coeColor;
  svVector4 expColor;
} SplitVectorsProperty;


class svGlyph : public svPrimitive
{
 public:
  svGlyph(svScalarArray xpos, svScalarArray ypos, 
          svScalarArray zpos, svVector3Array dir);

  svGlyph(svVector3Array *vec3in, svVector4Array *color4in, svInt _seed_num){};
  svGlyph(svVector3Array *vec3in, svInt _seed_num){};
  svGlyph(svChar *inf, svInt _seed_num);
  svGlyph();//svChar *dir, svChar *inf1, svChar *inff2, 
            //svVectorField* f, int numPlane);
  svGlyph(svVector3 _lbbox, svVector3 _rbbox, svChar *inf, svInt _seed_num);
  
  virtual void New(svVectorField* f, int numPlane);//svChar *indir, svChar *inf1, svChar *inf2,
            

  virtual void SaveToFile(char *fname); // save for later rendering

  virtual void ResetData(int seed);
  virtual void ResetCluster();
  
  virtual void SetData(char *infName, int seed);
  virtual void SetFormat(char *infname, int seed);
  virtual void SetSampleData(int size);

  virtual void SetSampling(svInt frequency); 
  virtual void SetSampling(vector<int> symmetrytype, svInt frequency);
 
  //void SetContourProperty(ContourProperty & property);
  //void SetKmeansProperty(KmeansProperty & property);

  virtual void ResetVisible();
  virtual void SetVisible(int contour);
  virtual void SetVisible(int contour, svScalar z1, svScalar z2, int frequency);
  virtual void SetSymmetryVisible(vector<int> type); //"OR" function instead of "AND"
  virtual void SetVisible(svScalar z1, svScalar z2);
  
  virtual void SetContourLabel();


  virtual void SetROI(); //default ROI
  virtual void SetROI(svScalar mag1, svScalar mag2);

  virtual void SetLineColorFromVec3(svVector3Array *vec3in);

  virtual ~svGlyph();
  void   clean(); // clean memory

// Generate
  virtual void Generate();
  virtual void GenerateClusters(svChar *inf);
  virtual void GenerateClusters(svIntArray *cluster);
  virtual void GenerateContours(ContourProperty & property);// const;
  virtual void GenerateClustersBySymmetry(KmeansProperty & property,
                     SymmetryProperty & sproperty,
                      vector<int> symmetrytype);
  virtual void GenerateClusters(KmeansProperty & property);// const;
  virtual void GenerateSymmetry(SymmetryProperty & property);
  virtual void Generate(svVector3Array *vec3in);
  virtual void Generate(svVector3Array *vec3in, svVector4Array *color4in);

// Rendering
  virtual void Render();
  virtual void RenderSample(){};
  virtual void DrawGrid(svVector3 startPos, svVector3 dir1, svVector3 dir2,
                       svScalar stepD1, svScalar stepD2,
                       svInt stepN1, svInt stepN2);
  virtual void DrawSilkPlane(svVector3 planeDir);

  virtual void RemovePoint(svInt lineIndex);
  // Not implemented: remove a glyph
  
  // Enables
  virtual void EnableColor();
  virtual void EnableColor(svInt colorModel, svFloat minValue, svFloat maxValue);
  virtual void DisableColor();
  virtual void SetColor(svVector4 color);
  virtual void SetColorByCluster();
  virtual void SetColor(int index1, int index2, svVector4 color1, svVector4 color2);
  virtual void SetColorByCluster(svIntArray index, svVector4 c);
  virtual void SetColorByCluster(svIntArray cluster);
  virtual void SetColorByClusterMag();
  virtual void SetColorByPower();
   // colorModel: SV_GRAY, SV_LOCS
  virtual void SetScaling(svInt s){scaling =s;}
  virtual void SetNumPower(svInt power){numPower = power;}

  virtual void EnableLineWidth(svFloat minlw, svFloat maxlw);
  virtual void SetBBox();
  virtual void SetLut(const svLut & newlut)
  {}; // Not implemented
  virtual void GetColor(svVector4Array *colorArray) const 
  {}; // Not implemented
  virtual void GetLineWidth(svScalarArray *lineWidth) const 
  {}; // Not implemented
  virtual void GetPosition(svVector3Array *glyphpos) const 
  {};// Not implemented

  virtual void Replace(svUint glyph_num, 
                svScalar x, svScalar y, svScalar z){}; 
  // Not imeplemented
  
   void SetXdistance(svScalar x){xdistance =  x;}
   void SetYdistance(svScalar y){ydistance = y;}
   void SetZdistance(svScalar z){zdistance = z;}
   virtual svInt GetContourListSize(){return contourList.size();}
   svVector3 GetLb(){return lbbox;}
   svVector3 GetRb(){return rbbox;}
   void GetBoundary(int index, svVector3 &l, svVector3 &r);
   svVector3 GetCenter(int index); 
   svVector3Array *GetGlyphs() const {return glyph;}
   svVector3 GetGlyph(int seed, int index){return glyph[seed][index];}
   svScalar GetMag(int seed, int index){return mag[seed][index];}
   virtual void SetAlpha(svScalar a){alpha =a;}
   virtual void SetRadius(svScalar radius){glyphRadius = radius;}
   virtual void SetScale(svScalar scale){glyphScale = scale;}
   virtual svScalar GetRadius(){return glyphRadius;}
   virtual svScalar GetScale(){return glyphScale;}
   virtual svIntArray* GetClusterLabels(){return clusterLabel;}
 private:

 protected:
  virtual void SaveToFile(char *outputfile, svIntArray clusterLayer, 
                     int dimension, int clusterIndex);
  //virtual void SetKmeansProperty();
  //virtual void SetContourProperty();
  virtual void SetContourLabel(int index, int layer, int in);

  virtual void GenerateContour(char *contourfile, char *vtkdir, int layer, float contourValue);
  
  virtual void GenerateCluster(char *clusterfile, char *immefile, bool isNormalize,
                                 int dimension, svScalarArray clusterWeight,
								  int numCluster);
  virtual void SetClusterLabel(char *clusterfile, svIntArray clusterLayer, 
                         int clusterIndex);
  virtual void SetClusterLabelBySymmetry(char *clusterfile, svIntArray clusterLayer,
                         int clusterIndex, vector<int> symmetrytype);
  virtual void RenderStore() const;     // using vertex array
  virtual void RenderStoreDirect() const;

  svVector3Array *glyph; // positions
  svVector3Array *dir;   // direction
  svScalarArray  *mag;   // magnitude
  svScalarArray *exp;
  svScalarArray *coe;

  
  svInt dataSize;
  svInt glyphSize;
  svScalar   glyphScale;
  svScalar   glyphRadius;
 
  svIntArray *glyphFormat; 

  int maxClusterLabel;
  svIntArray clusterLabelbymag;

  svScalarArray contourList;
  svIntArray    *contourLabel;
  svIntArray    *clusterLabel;   // store cluster result
  svIntArray    *roiLabel; //inside roi; boolean
  svIntArray    *visibleLabel;
  svIntArray    sampleLabel;
 
  svVector4Array *glyphColors;
  svScalarArray  *glyphWidth;
 
  //KmeansProperty  kmeansproperty;
  //ContourProperty contourproperty;
                              
  svScalar alpha;
  svInt scaling;
  svInt numPower;

  svScalar xdistance;
  svScalar ydistance;
  svScalar zdistance;

 //svPeeling *peeling;

  svChar *infile;
  svList *symmetrylist[6];
  //svChar *glyphDir;
  //svChar *glyphFile;
  //svChar *fileName1;
  //svChar *fileName2;
};

}

#endif // __SV_STREAMLINE_H
