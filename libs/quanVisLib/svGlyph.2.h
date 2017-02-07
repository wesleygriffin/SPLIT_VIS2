
#ifndef __SV_GLYPH_H
#define __SV_GLYPH_H

#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"
#include "svColors.h"
//#define DEFAULT_GLYPH_RADIUS 0.02
#define DEFAULT_GLYPH_HEIGHT 0.08
#define DEFAULT_GLYPH_FREQUENCY 1
#define DEFAULT_GLYPH_SECTION_NUM 5

// TODO: make good glyphs
namespace __svl_lib {

class svGlyph : public svPrimitive
{
 public:
  svGlyph(svScalarArray xpos, svScalarArray ypos, 
          svScalarArray zpos, svVector3Array dir);

  svGlyph(svVector3Array *vec3in, svVector4Array *color4in, svInt _seed_num){};
  svGlyph(svVector3Array *vec3in, svInt _seed_num){};
  svGlyph(svChar *inf, svInt _seed_num);
  svGlyph(svChar *dir, svChar *inf1, svChar *inff2, 
            svVectorField* f, int numPlane);
  svGlyph(svVector3 _lbbox, svVector3 _rbbox, svChar *inf, svInt _seed_num);
  svGlyph(svVectorField* f, int numPlane);
  
  virtual void New(svChar *dir, svChar *inf1, svChar *inff2,
            svVectorField* f, int numPlane);

//  virtual void SetRawDatainDir(char *dir){sprintf(rawDir,"%s",dir);}
//  virtual void SetVTKinDir(char *dir){sprintf(vtkDir,"%s",dir);}
//  virtual void SetSaveinDir(char *dir){sprintf(tmpDir,"%s",dir);}
  
  virtual void SaveToFile(char *fname); // save for later rendering
//  virtual void SaveToFile(); // save for later rendering
  
//  virtual void SetDimension(int dim){dimension = dim;}  

  virtual void EnableContour(){isContour = true;}
  virtual void EnableRaw(){isContour = false;}
//  virtual void EnableNormalize(){isNormalize = true;}

  virtual void ResetData(int seed);
  
//  virtual void SetData(); 
//  virtual void SetRawData(int layer);
  virtual void SetData(char *infName, int seed);
  
//  virtual void SetGlyphInFile(char *glyphfname){sprintf(glyphFile, "%s", glyphfname);}

  /*-------------contour--------------*/
 // virtual void SaveContourToFile(char *file);
 // virtual void SetContourInFile(char *contour){sprintf(contourFile, "%s",contour);}
  
  void SetContourProperty(ContourProperty & property);

 // virtual void ResetContour(int layer); //delect values;
 // virtual void ResetContourLabel(int layer); // set to false
  
 // virtual void AddContour(int layer, float contourValue);
 // virtual void AddContour(char *valueFile); 

//  virtual void ComputeContour(int layer, float contourValue);
  virtual void GenerateContours();
  /*----------------------------------*/

  /*-------------clustering-----------*/

  void SetKmeansProperty(KmeansProperty & property)
  {
          kmeansproperty = property;
  }

  virtual void generateClusters();
  virtual void SetClusterLabel();
  
//  virtual void SetClusterWeight(svScalarArray weight); //->
  
//  virtual void EnableIsWhole(){isWhole = true;}
//  virtual void DisableIsWhole(){isWhole = false;}
  
 // virtual void SetKMeanInFile(char *cluster){sprintf(clusterFile,"%s",cluster);}
//  virtual void SetByWhole();
//  virtual void SetByLayer(int layer);
//  virtual void SetByEvenLayer();
//  virtual void SetByOddLayer();
//  virtual void SetByLayer(svIntArray layerArray);

//  virtual void ComputeClusters(int numCluster);

//  virtual void ComputeClustersByWhole(int numCluster);
//  virtual void ComputeClustersByLayer(int numCluster);
//  virtual void ComputeClusterByLayer(svIntArray layerArray, int numCluster);
//  virtual void ComputeClusterByEvenLayer(int numCluster);
//  virtual void ComputeClusterByOddLayer(int numCluster);
  /*---------------------------------*/
  
  /*
  svGlyph(svVectorField* f, svParticle* sd, 
               svInt sd_num, svUint mode=SV_DISPLAYLIST);
  svGlyph(svVectorField* f, 
               svVector3 _lbbox, svVector3 _rbbox,
               svParticle* sd, 
               svInt sd_num, svUint mode=SV_DISPLAYLIST);
	       */
		   
 // Update
	//virtual void New(int numPlane);
	//virtual void New(svVectorField* f, int numPlane);
 //
//	virtual void SetData(char *datafname); //not implemented yet
 //
  virtual void SetROI(); //default ROI
  virtual void SetROI(svScalar mag1, svScalar mag2);
  virtual void SetLineColorFromVec3(svVector3Array *vec3in);

  virtual ~svGlyph();
  void   clean(); // clean memory

// Generate
  virtual void Generate(); 
  virtual void Generate(svVector3Array *vec3in);
  virtual void Generate(svVector3Array *vec3in, svVector4Array *color4in);

// Rendering
  virtual void Render();

  virtual void RemovePoint(svInt lineIndex);
  // Not implemented: remove a glyph

  
  // Enables
  virtual void EnableColor();
  virtual void EnableColor(svInt colorModel, svFloat minValue, svFloat maxValue);
  virtual void DisableColor();
  virtual void SetColorByCluster();
  // colorModel: SV_GRAY, SV_LOCS
  
  virtual void EnableLineWidth(svFloat minlw, svFloat maxlw);

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
    
   svVector3Array *GetGlyphs() const {return glyph;}

   void DisableContour()
   {
      isContour = false;
      Generate();
   }

 private:

 protected:
  virtual void SaveToFile();
  virtual void SetKmeansProperty();
  virtual void SetContourProperty();

  virtual void RenderStore() const;     // using vertex array
  virtual void RenderStoreDirect() const;
  //virtual void RenderStoreTube() const;
  //virtual void  BuildDisplayListFromStore();

  svVector3Array *glyph; // positions
  svVector3Array *dir;   // direction
  svScalarArray  *mag;   // magnitude

  svIntArray    *clusterLabel;   // store cluster result
  svIntArray    *roiLabel; //inside roi; boolean
  svIntArray    *visibleLabel;
//  svIntArray    *contourLabel; //->

//  svIntArray clusterLayer; //->
                           //-> the same index: clustering together
                           //-> 
//  int clusterDataSize; //->
//  svScalarArray clusterWeight; //->

  svVector4Array *glyphColors;
  svScalarArray  *glyphWidth;
 
  KmeansProperty kmeansproperty;
  ContourProperty contourproperty;
 
//  svScalarArray *contourValues; //->
                              
  svScalar alpha;
//  svBool isWhole;
//  int dimension;//for clustering;
  svBool isContour;
//  svBool isNormalize;
  
 // svInt numGlyph;

  svChar *fileDir;
 // svChar *rawDir;
 // svChar *vtkDir;

  svChar *fileName1;
 // svChar *contourFile;
 // svChar *clusterFile;
  svChar *fileName2;

 // svChar *cInFile;
 // svChar *kMeanInFile;
};

}

#endif // __SV_STREAMLINE_H
